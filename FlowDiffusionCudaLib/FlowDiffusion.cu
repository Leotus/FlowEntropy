////////////////////////////////////////////
	#include <stdio.h>
	#include <texture_fetch_functions.h>

	#include "FlowDiffusion_cuda.h"
	#include "FlowDiffusion.h"

////////////////////////////////////////////
	static cudaExtent cVolumeExtent;
	static float4 *pf4Volume_host;
	static cudaPitchedPtr pcVolumePtrs_global[2];
	static texture<float4, 2, cudaReadModeElementType> t2dSrc;
						// array and texture reference for the weight and offset volume
	static cudaExtent cVolumeExtent_array;
	static float4 *pf4WeightOffsetVolume_host;
	static cudaArray *cWeightOffsetVolume_array = 0;
	static texture<float4, 3, cudaReadModeElementType> t3dWeightOffset;
    static cudaChannelFormatDesc cWeightOffsetChannelDesc = cudaCreateChannelDesc<float4>();

	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
    static CUDPPConfiguration cConfig;
    static CUDPPHandle cScanplan = 0;
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP

	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP				
	static cudaExtent cErrorVolumeExtent;
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	

	static cudaPitchedPtr cErrorVolume_device;
	static cudaPitchedPtr cErrorSum_device;

	int iNrOfVoxels;

	static texture<int, 2, cudaReadModeElementType> t2dAngleMap;
	static cudaArray *pcAngleMap_array;	// cuda array to hold the Gaussian kernels

	static cudaExtent cBinVolumeExtent;
	static cudaPitchedPtr cSrcBinVolume_pitched;
	static cudaPitchedPtr cDstBinVolume_pitched;

						// a temp. volume in the host side to stored the 
	static int *piBinVolume_host;

						// a texture tha represents a vector field to be converted into bins 
	static texture<float4, 2, cudaReadModeElementType> t2dVectorVolume;

////////////////////////////////////////////
	#include "FlowDiffusion2D_kernel.cu"

////////////////////////////////////////////

#include "EntropyField_kernel.cu"

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_HOST
		#include	"EntropyField_host.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_CUDA
		#include	"EntropyField_cuda.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_SORTING_CUDA	
		#include	"EntropyFieldWithSorting_cuda.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA
		#include	"EntropyFieldPerScanline_cuda.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_SORT_PER_VOXEL_CUDA	
		#include	"EntropyFieldSortPerVoxel_cuda.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_ON_SPARSE_HISTOGRAM_CUDA	
		#include	"EntropyFieldOnSparseHistogram_cuda.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_MARGINAL_HISTOGRAM	
		#include	"EntropyFieldWithMarginalHistogram_cuda.cu"
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_SORTING_IN_VECTORS_CUDA	
		#include	"EntropyFieldWithSortingInVectors_cuda.cu"
	#endif

////////////////////////////////////////////

void
_ComputeSrcEntropyVolume
(
	int iNrOfBins,
	int iKernelWidth, int iKernelHeight, int iKernelDepth,
	float *pfEntropyVolume_host
)
{
	int *piHistogram_global;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&piHistogram_global, sizeof(piHistogram_global[0]) * iNrOfBins) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset(	(void*)piHistogram_global, 0, sizeof(piHistogram_global[0]) * iNrOfBins) );

	float *pfLogHistogram_global;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&pfLogHistogram_global, sizeof(pfLogHistogram_global[0]) * iNrOfBins) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset(	(void*)pfLogHistogram_global, 0, sizeof(pfLogHistogram_global[0]) * iNrOfBins) );

	cudaExtent cEntropyVolumeExtent	= make_cudaExtent(
		cVolumeExtent_array.width * sizeof(float),
		cVolumeExtent_array.height,
		cVolumeExtent_array.depth);
	cudaPitchedPtr cEntropyVolume_pitched;
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cEntropyVolume_pitched, cEntropyVolumeExtent) );

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_CUDA
		_ComputeEntropyVolume_cuda
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_HOST
		_ComputeEntropyVolume_host
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_SORTING_CUDA
		_ComputeEntropyVolumeWithSorting_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA	
		_ComputeEntropyVolumePerScanline_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_SORT_PER_VOXEL_CUDA	
		_ComputeEntropyVolumePerVoxel_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_ON_SPARSE_HISTOGRAM_CUDA
		_ComputeEntropyVolumeOnSparseHistogram_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_SORTING_IN_VECTORS_CUDA
		_ComputeEntropyVolumeWithSortingInVectors_cuda
	#endif	

	(
						// res. of the neighboring region
		make_int3(iKernelWidth, iKernelHeight, iKernelDepth),

						// the histogram
		iNrOfBins,
		piHistogram_global,
		pfLogHistogram_global,

						// res. of the volume																
		make_int3(int(cVolumeExtent_array.width), int(cVolumeExtent_array.height), int(cVolumeExtent_array.depth)),

						// bin volume																	
		cSrcBinVolume_pitched,
		cEntropyVolume_pitched
	);

	if( NULL != pfEntropyVolume_host )
		CUDA_SAFE_CALL(
			cudaMemcpy2D(
				pfEntropyVolume_host, 
				cVolumeExtent_array.width * sizeof(pfEntropyVolume_host[0]),
				cEntropyVolume_pitched.ptr,
				cEntropyVolume_pitched.pitch,
				cVolumeExtent_array.width * sizeof(pfEntropyVolume_host[0]),
				cVolumeExtent_array.height * cVolumeExtent_array.depth,
				cudaMemcpyDeviceToHost) );

	FREE_MEMORY(piHistogram_global);
	FREE_MEMORY(pfLogHistogram_global);
	FREE_MEMORY(cEntropyVolume_pitched.ptr);
}	

void
_ComputeJointEntropyVolume
(
	int iNrOfSrcBins,
	int iNrOfDstBins,
	int iKernelWidth, int iKernelHeight, int iKernelDepth,
	float *pfEntropyVolume_host
)
{
	int iNrOfBins = iNrOfSrcBins * iNrOfDstBins;

	int *piHistogram_global;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&piHistogram_global, sizeof(piHistogram_global[0]) * iNrOfBins) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset(	(void*)piHistogram_global, 0, sizeof(piHistogram_global[0]) * iNrOfBins) );

	float *pfLogHistogram_global;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&pfLogHistogram_global, sizeof(pfLogHistogram_global[0]) * iNrOfBins) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset(	(void*)pfLogHistogram_global, 0, sizeof(pfLogHistogram_global[0]) * iNrOfBins) );

	cudaExtent cEntropyVolumeExtent	= make_cudaExtent(
		cVolumeExtent_array.width * sizeof(float),
		cVolumeExtent_array.height,
		cVolumeExtent_array.depth);
	cudaPitchedPtr cEntropyVolume_pitched;
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cEntropyVolume_pitched, cEntropyVolumeExtent) );

	// bind the bin volume as a 2D texture
	t2dSrcBinVolume.addressMode[0] = cudaAddressModeClamp;
	t2dSrcBinVolume.addressMode[1] = cudaAddressModeClamp;
	t2dSrcBinVolume.filterMode =	cudaFilterModePoint;
	t2dSrcBinVolume.normalized =	false;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dSrcBinVolume, 
			cSrcBinVolume_pitched.ptr, 
			cudaCreateChannelDesc<int>(),
			cVolumeExtent_array.width, 
			cVolumeExtent_array.height * cVolumeExtent_array.depth, 
			cSrcBinVolume_pitched.pitch) );

	t2dDstBinVolume.addressMode[0] = cudaAddressModeClamp;
	t2dDstBinVolume.addressMode[1] = cudaAddressModeClamp;
	t2dDstBinVolume.filterMode =	cudaFilterModePoint;
	t2dDstBinVolume.normalized =	false;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dDstBinVolume, 
			cDstBinVolume_pitched.ptr, 
			cudaCreateChannelDesc<int>(),
			cVolumeExtent_array.width, 
			cVolumeExtent_array.height * cVolumeExtent_array.depth, 
			cDstBinVolume_pitched.pitch) );

	// bind the histogram as a 1D texture
	// ...
	cudaPitchedPtr cJointBinVolume_pitched;
	CUDA_SAFE_CALL( 
		cudaMalloc3D(
			&cJointBinVolume_pitched, 
			make_cudaExtent(
				cVolumeExtent_array.width * sizeof(int),
				cVolumeExtent_array.height,
				cVolumeExtent_array.depth) ) );

	dim3 v3Blk, v3Grid;
	v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	v3Grid = dim3(
		size_t(ceilf(float(cVolumeExtent_array.width) / float(v3Blk.x))),
		size_t(ceilf(float(cVolumeExtent_array.height * cVolumeExtent_array.depth) / float(v3Blk.y)))
	);

	_JointSrcDst_kernel<<<v3Grid, v3Blk, 0>>>
	(
		iNrOfSrcBins, 
		iNrOfDstBins,

		make_int3(
			int(cVolumeExtent_array.width), 
			int(cVolumeExtent_array.height), 
			int(cVolumeExtent_array.depth)),

		cJointBinVolume_pitched
	);
	CUT_CHECK_ERROR("_JointSrcDst_kernel() failed");

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_CUDA
		_ComputeEntropyVolume_cuda
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_HOST
		_ComputeEntropyVolume_host
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_SORTING_CUDA
		_ComputeEntropyVolumeWithSorting_cuda
	#endif

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA	
		_ComputeEntropyVolumePerScanline_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_SORT_PER_VOXEL_CUDA	
		_ComputeEntropyVolumePerVoxel_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_ON_SPARSE_HISTOGRAM_CUDA
		_ComputeEntropyVolumeOnSparseHistogram_cuda
	#endif	 

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_MARGINAL_HISTOGRAM
		_ComputeEntropyVolumeWithMarginalHistogram_cuda
	#endif	

	#if	COMPUTE_ENTROPY_VOLUME == COMPUTE_ENTROPY_VOLUME_WITH_SORTING_IN_VECTORS_CUDA
		_ComputeEntropyVolumeWithSortingInVectors_cuda
	#endif	
	(
						// res. of the neighboring region
		make_int3(iKernelWidth, iKernelHeight, iKernelDepth),

						// the histogram
		iNrOfBins,
		piHistogram_global,
		pfLogHistogram_global,

						// res. of the volume																
		make_int3(int(cVolumeExtent_array.width), int(cVolumeExtent_array.height), int(cVolumeExtent_array.depth)),

						// bin volume																	
		cJointBinVolume_pitched,
		cEntropyVolume_pitched
	);

	if( NULL != pfEntropyVolume_host )
		CUDA_SAFE_CALL_NO_SYNC(
			cudaMemcpy2D(
				pfEntropyVolume_host, 
				cVolumeExtent_array.width * sizeof(pfEntropyVolume_host[0]),
				cEntropyVolume_pitched.ptr,
				cEntropyVolume_pitched.pitch,
				cVolumeExtent_array.width * sizeof(pfEntropyVolume_host[0]),
				cVolumeExtent_array.height * cVolumeExtent_array.depth,
				cudaMemcpyDeviceToHost) );

	FREE_MEMORY(cJointBinVolume_pitched.ptr);
	FREE_MEMORY(piHistogram_global);
	FREE_MEMORY(pfLogHistogram_global);
	FREE_MEMORY(cEntropyVolume_pitched.ptr);
}

////////////////////////////////////////////
	#include "FlowDiffusion3D_kernel.cu"

extern "C" {

void 
_FlowDiffusionFree()
{
	FREE_MEMORY_ON_HOST(pf4Volume_host);
	for(int i = 0; i < 2; i++)
		FREE_MEMORY(pcVolumePtrs_global[i].ptr);
	FREE_ARRAY(cWeightOffsetVolume_array);
	FREE_MEMORY_ON_HOST(pf4WeightOffsetVolume_host);
	FREE_MEMORY(cErrorVolume_device.ptr);
	FREE_MEMORY(cErrorSum_device.ptr);

	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	cudppDestroyPlan(cScanplan);
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP

	FREE_ARRAY(pcAngleMap_array);	

	FREE_MEMORY(cSrcBinVolume_pitched.ptr);
	FREE_MEMORY(cDstBinVolume_pitched.ptr);
	FREE_MEMORY(cSrcDstBinVolumePtr_global.ptr);
}

void
_FlowDiffusionInit(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth
)
{
	CLOCK_INIT(PRINT_FLOW_DIFFUSION_TIMING, __FUNCTION__ ": ");

	iNrOfVoxels = iVolumeWidth * iVolumeHeight * iVolumeDepth;

	cVolumeExtent	= make_cudaExtent(
		iVolumeWidth * sizeof(float4),
		iVolumeHeight,
		iVolumeDepth);

	// allocate a local copy in the CPU side
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&pf4Volume_host,
			sizeof(pf4Volume_host[0]) * iNrOfVoxels) );
	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	// Allocate 2 volumes on the  GPU's global memory.
	// in each iteration, one of them will be used as the source
	// 3D texture, and the other will be served as the dst.
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);

	for(int i = 0; i < 2; i++)
	{
		CUDA_SAFE_CALL( 
			cudaMalloc3D(&pcVolumePtrs_global[i], cVolumeExtent) );
	}

	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
	cErrorVolumeExtent	= make_cudaExtent(
		iVolumeWidth * sizeof(float),
		iVolumeHeight,
		iVolumeDepth);
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cErrorVolume_device,	cErrorVolumeExtent) );
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cErrorSum_device,		cErrorVolumeExtent) );

	cConfig.op = CUDPP_ADD;
	// cConfig.op = CUDPP_MAX;
	cConfig.datatype = CUDPP_FLOAT;
	cConfig.algorithm = CUDPP_SCAN;
	cConfig.options = CUDPP_OPTION_BACKWARD | CUDPP_OPTION_INCLUSIVE;
	unsigned int uPlanStatus = cudppPlan(&cScanplan, cConfig, iVolumeWidth * iVolumeHeight * iVolumeDepth, iVolumeHeight * iVolumeDepth, cErrorVolume_device.pitch);
	assert(CUDPP_SUCCESS  == uPlanStatus);  
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP

	cVolumeExtent_array = make_cudaExtent(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth);

	CUDA_SAFE_CALL( 
		cudaMalloc3DArray(&cWeightOffsetVolume_array, &cWeightOffsetChannelDesc, cVolumeExtent_array) );

	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&pf4WeightOffsetVolume_host,
			sizeof(pf4WeightOffsetVolume_host[0]) * iNrOfVoxels) );

	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	CLOCK_PRINT(PRINT_FLOW_DIFFUSION_TIMING);
}

void 
_FlowDiffusionSetAngleMap(int *piAngleMap, int iNrOfPhis, int iNrOfThetas)
{
	// allocate an array on the GPU side

	// upload the angle map to the araray

	// setup the texture for the angle map
	/*
    t2dAngleMap.addressMode[0] =	cudaAddressModeClamp;
    t2dAngleMap.addressMode[1] =	cudaAddressModeClamp;
    t2dAngleMap.filterMode =		cudaFilterModePoint;
    t2dAngleMap.normalized =		true;
	*/
	SETUP_ARRAY(pcAngleMap_array, iNrOfPhis, iNrOfThetas, 32, 0, 0, 0, cudaChannelFormatKindSigned);	
	BIND_ARRAY_AS_TEXTURE(
		t2dAngleMap, pcAngleMap_array, 
		cudaAddressModeClamp, cudaAddressModeClamp, cudaFilterModePoint, true);	

    CUDA_SAFE_CALL( 
		cudaMemcpy2DToArray(
			pcAngleMap_array, 
			0, 
			0, 
			(void*)piAngleMap, 
			iNrOfPhis * sizeof(piAngleMap[0]), 
			iNrOfPhis * sizeof(piAngleMap[0]), 
			iNrOfThetas, 
			cudaMemcpyHostToDevice) );

	// check whether the volume extent has been setup
	assert( NULL != pcVolumePtrs_global[0].ptr );

	// allocate the volume of bins in the global memorty on GPUs
	cBinVolumeExtent	= make_cudaExtent(
		cVolumeExtent_array.width * sizeof(int),
		cVolumeExtent_array.height,
		cVolumeExtent_array.depth);
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cSrcBinVolume_pitched, cBinVolumeExtent) );
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cDstBinVolume_pitched, cBinVolumeExtent) );

	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cSrcDstBinVolumePtr_global, cBinVolumeExtent) );

	t2dSrcBinVolume.addressMode[0] = cudaAddressModeClamp;
	t2dSrcBinVolume.addressMode[1] = cudaAddressModeClamp;
	t2dSrcBinVolume.filterMode =	cudaFilterModePoint;
	t2dSrcBinVolume.normalized =	false;

	t2dDstBinVolume.addressMode[0] = cudaAddressModeClamp;
	t2dDstBinVolume.addressMode[1] = cudaAddressModeClamp;
	t2dDstBinVolume.filterMode =	cudaFilterModePoint;
	t2dDstBinVolume.normalized =	false;

	// allocate the volume of bins in the host side

	// setup the src. texture for bin lookup
	t2dVectorVolume.addressMode[0] = cudaAddressModeClamp;
	t2dVectorVolume.addressMode[1] = cudaAddressModeClamp;
	t2dVectorVolume.filterMode =	cudaFilterModePoint;
	t2dVectorVolume.normalized =	false;
}

void
_Vector3DToVolume(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	cudaPitchedPtr cVolumePtr_global,	// input
	cudaPitchedPtr cBinVolumePtr_global	// output
)
{
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dVectorVolume, 
			cVolumePtr_global.ptr, 
			cudaCreateChannelDesc<float4>(),
			iVolumeWidth, 
			iVolumeHeight * iVolumeDepth, 
			cVolumePtr_global.pitch) );

	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	dim3 v3Grid = dim3(
		(unsigned int)ceilf((float)iVolumeWidth	 / (float)v3Blk.x),
		unsigned int(ceilf( float(iVolumeHeight) / float(v3Blk.y) ) ) *
		unsigned int(ceilf( float(iVolumeDepth) /  float(iBlockZSize) ) ) );
	_Vector3DToBinVolume_kernel<<<v3Grid, v3Blk, 0>>>
	(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth,
		int(ceilf(float(iVolumeHeight) / float(BLOCK_DIM_Y))),
		iBlockZSize,
		cBinVolumePtr_global
	);
	CUT_CHECK_ERROR("_Vector3DToVolume_kernel() failed");
}

void
_ComputeSrcBinVolume
(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	float *pfSrcVolume
)
{
	CLOCK_INIT(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, __FUNCTION__ ": ");

	CLOCK_BEGIN(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pf4Volume_host[v].x = pfSrcVolume[v * 3 + 0];
				pf4Volume_host[v].y = pfSrcVolume[v * 3 + 1];
				pf4Volume_host[v].z = pfSrcVolume[v * 3 + 2];
				pf4Volume_host[v].w = 0.0f;
			}
	CLOCK_END(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, false);

	CLOCK_BEGIN(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
    cudaMemcpy3DParms cCopyParamsHostToDevice = {0};

    cCopyParamsHostToDevice.srcPtr   = make_cudaPitchedPtr(
		(void*)pf4Volume_host, 
		cVolumeExtent.width, 
		iVolumeWidth * sizeof(float4), 
		iVolumeHeight);
    cCopyParamsHostToDevice.dstPtr		= pcVolumePtrs_global[0];
    cCopyParamsHostToDevice.extent		= cVolumeExtent;
    cCopyParamsHostToDevice.kind		= cudaMemcpyHostToDevice;
    CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsHostToDevice) );  

	CLOCK_END(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, false);

	CLOCK_BEGIN(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
	_Vector3DToVolume(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth,
		iBlockZSize,
		pcVolumePtrs_global[0],
		cSrcBinVolume_pitched);
	CLOCK_END(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, false);

	CLOCK_PRINT(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
}

void 
_GetSrcBinVolume(int *piBinVolume)
{
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&piBinVolume_host,
			sizeof(piBinVolume_host[0]) * iNrOfVoxels) );

	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr		= cSrcBinVolume_pitched;
	cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		(void*)piBinVolume_host, 
		cVolumeExtent_array.width * sizeof(int),	// cSrcBinVolume_pitched.width, 
		cVolumeExtent_array.width * sizeof(int), 
		cVolumeExtent_array.height);
	cCopyParamsDeviceToHost.extent		= cBinVolumeExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	memcpy(piBinVolume, piBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);

	FREE_MEMORY_ON_HOST(piBinVolume_host);
}

void 
_GetDstBinVolume(int *piBinVolume)
{
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&piBinVolume_host,
			sizeof(piBinVolume_host[0]) * iNrOfVoxels) );


	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr		= cDstBinVolume_pitched;
	cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		(void*)piBinVolume_host, 

		cVolumeExtent_array.width * sizeof(int),	// cDstBinVolume_pitched.width, 
		cVolumeExtent_array.width * sizeof(int), 
		cVolumeExtent_array.height);
	cCopyParamsDeviceToHost.extent		= cBinVolumeExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	memcpy(piBinVolume, piBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);

	FREE_MEMORY_ON_HOST(piBinVolume_host);
}

void
_FlowDiffusion(
	float fAttenuation,
	int iNrOfIterations,
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	float *pfWeightVolume,
	float *pfXOffsetVolume,
	float *pfYOffsetVolume,
	float *pfZOffsetVolume,
	float *pfSrcVolume,
	float *pfDstVolume,
	int *piFlagVolume
)
{
	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	iNrOfIterations = 9 * max(max(iVolumeWidth, iVolumeHeight), iVolumeDepth);
	// LOG(printf("Warning! iNrOfIterations is changed to %d", iNrOfIterations));
	#endif
	
	CLOCK_INIT(PRINT_FLOW_DIFFUSION_TIMING, __FUNCTION__ ": ");

	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	dim3 v3Grid = dim3(
		(unsigned int)ceilf((float)iVolumeWidth	 / (float)v3Blk.x),
		#if					DIFFUSION_BY_FOR_LOOP_ON_HOST	

		(unsigned int)ceilf((float)iVolumeHeight / (float)v3Blk.y));

		#else	// #if		DIFFUSION_BY_FOR_LOOP_ON_HOST
		unsigned int(ceilf( float(iVolumeHeight) / float(v3Blk.y) ) ) *
		unsigned int(ceilf( float(iVolumeDepth) /  float(iBlockZSize) ) ) );
		#endif	// #if		DIFFUSION_BY_FOR_LOOP_ON_HOST

	// convert the #channels in the src volume from 3 to 4
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pf4Volume_host[v].x = pfSrcVolume[v * 3 + 0];
				pf4Volume_host[v].y = pfSrcVolume[v * 3 + 1];
				pf4Volume_host[v].z = pfSrcVolume[v * 3 + 2];

				if( piFlagVolume )
					pf4Volume_host[v].w = float(piFlagVolume[v]);
				else
				pf4Volume_host[v].w = 0.0f;
			}

	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				if( pfXOffsetVolume )
				pf4WeightOffsetVolume_host[v].x = pfXOffsetVolume[v];
				if( pfYOffsetVolume )
				pf4WeightOffsetVolume_host[v].y = pfYOffsetVolume[v];
				if( pfZOffsetVolume )
				pf4WeightOffsetVolume_host[v].z = pfZOffsetVolume[v];
				pf4WeightOffsetVolume_host[v].w = 1.0f - pfWeightVolume[v];
			}

	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	// Upload the input volume to the first volume on the GPU
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);

    cudaMemcpy3DParms cCopyParamsHostToDevice = {0};

    cCopyParamsHostToDevice.srcPtr   = make_cudaPitchedPtr(
		(void*)pf4Volume_host, 
		cVolumeExtent.width, 
		iVolumeWidth * sizeof(float4), 
		iVolumeHeight);
    cCopyParamsHostToDevice.dstPtr		= pcVolumePtrs_global[0];
    cCopyParamsHostToDevice.extent		= cVolumeExtent;
    cCopyParamsHostToDevice.kind		= cudaMemcpyHostToDevice;
    CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsHostToDevice) );  

    cudaMemcpy3DParms cCopyParamsHostToArray = {0};
    cCopyParamsHostToArray.srcPtr   = make_cudaPitchedPtr(
		(void*)pf4WeightOffsetVolume_host, 
		iVolumeWidth * sizeof(float4), 
		iVolumeWidth, 
		iVolumeHeight);
    cCopyParamsHostToArray.dstArray = cWeightOffsetVolume_array;
    cCopyParamsHostToArray.extent   = cVolumeExtent_array;
    cCopyParamsHostToArray.kind     = cudaMemcpyHostToDevice;
    CUDA_SAFE_CALL( 
		cudaMemcpy3D(&cCopyParamsHostToArray) );  

    t3dWeightOffset.addressMode[0] = cudaAddressModeClamp;
    t3dWeightOffset.addressMode[1] = cudaAddressModeClamp;
    t3dWeightOffset.addressMode[2] = cudaAddressModeClamp;
    t3dWeightOffset.filterMode		= cudaFilterModePoint;
    t3dWeightOffset.normalized		= false; 

    // bind array to 3D texture
    CUDA_SAFE_CALL(
		cudaBindTextureToArray(t3dWeightOffset, cWeightOffsetVolume_array, cWeightOffsetChannelDesc));

	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	// calcuate the fusion operator on GPUs
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);

    t2dSrc.addressMode[0] = cudaAddressModeClamp;
    t2dSrc.addressMode[1] = cudaAddressModeClamp;
    t2dSrc.filterMode =		cudaFilterModePoint;
    t2dSrc.normalized =		false;

	int iSrc = 0;

	for(int i = 0; 
			i < iNrOfIterations; 
			i++,				iSrc = 1 - iSrc)
	{
		// bind the src. volume as the 3D texture

		cudaChannelFormatDesc cChannelDesc =
			cudaCreateChannelDesc<float4>();

		CUDA_SAFE_CALL_NO_SYNC(
			cudaBindTexture2D(
				0, 
				t2dSrc, 
				pcVolumePtrs_global[iSrc].ptr, 
				cChannelDesc,
				iVolumeWidth, 
				iVolumeHeight * iVolumeDepth, 
				pcVolumePtrs_global[iSrc].pitch) );


		if( iVolumeDepth <= 2 )
			_FlowDiffusion2D_kernel<<<v3Grid, v3Blk, 0>>>
			(
				fAttenuation,
				iVolumeWidth,
				iVolumeHeight,
				pcVolumePtrs_global[1 - iSrc],
				cErrorVolume_device
			);	
		else
		{

		// launch the kernel to compute the diffusion operator for 1 iteration
		#if	DIFFUSION_BY_FOR_LOOP_ON_HOST

			for(int z = 0; z < iVolumeDepth; z++)
				_FlowDiffusion3D_kernel<<<v3Grid, v3Blk, 0>>>
				(
					fAttenuation,
					iVolumeWidth,
					iVolumeHeight,
					iVolumeDepth,
					z,

					// output
					pcVolumePtrs_global[1 - iSrc],
					cErrorVolume_device
				);	
		#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST

		// _FlowFusion_kernel<<<v3Grid, v3Blk, v3Blk.x * v3Blk.y * sizeof(float4)>>>
		_FlowDiffusion3D_kernel<<<v3Grid, v3Blk, 0>>>
		(
			fAttenuation,
			iVolumeWidth,
			iVolumeHeight,
			iVolumeDepth,
			0,
			int(ceilf(float(iVolumeHeight) / float(BLOCK_DIM_Y))),
			iBlockZSize,
			// output
			pcVolumePtrs_global[1 - iSrc],
			cErrorVolume_device
		);
		#endif
		}
		CUT_CHECK_ERROR("_FlowFusion_kernel() failed");

		#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
		cudppScan(cScanplan, cErrorSum_device.ptr, cErrorVolume_device.ptr, iVolumeWidth * iVolumeHeight * iVolumeDepth);

		float fError;
	    CUDA_SAFE_CALL_NO_SYNC( 
			cudaMemcpy(
				&fError, 
				cErrorSum_device.ptr, 
				sizeof(float), 
				cudaMemcpyDeviceToHost));
		/*
		fError /= float(iVolumeWidth * iVolumeHeight * iVolumeDepth * 3);
		fError = sqrtf(fError);
		*/
		fError /= float(iVolumeWidth * iVolumeHeight * iVolumeDepth * 3);
		fError = sqrtf(fError);
		// fprintf(stderr, "Error: %f\n", fError);

		static float fPrevError = -1.0f;
		float fErrorRate = fError/fPrevError;
		if( i > max(max(iVolumeWidth, iVolumeHeight), iVolumeDepth) && 
			(DIFFUSION_CONVERGE_THRESHOLD <= fErrorRate && fErrorRate <= 1.00f ) )
		{
			printf("\t#iters = %d;", i);
			break;
		}
		if( i > 1 && fErrorRate > 1.0f )
		{
			printf("Error increase. Stop.");
			break;
		}
		fPrevError = fError;

		#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	}

	if( iVolumeDepth > 2 )
	{
		_Vector3DToVolume(
			iVolumeWidth,
			iVolumeHeight,
			iVolumeDepth,
			iBlockZSize,
			pcVolumePtrs_global[iSrc],
			cDstBinVolume_pitched);
			// piDstAngleBinVolume_host);
	}

	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	int iDst = iSrc;

	// read the result from latest iteration back to the CPU side
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);

	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
    cCopyParamsDeviceToHost.srcPtr		= pcVolumePtrs_global[iDst];
    cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		(void*)pf4Volume_host, 
		cVolumeExtent.width, 
		iVolumeWidth * sizeof(float4), 
		iVolumeHeight);
    cCopyParamsDeviceToHost.extent		= cVolumeExtent;
    cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	// convert the #channels in the dst volume from 4 to 3
	CLOCK_BEGIN(PRINT_FLOW_DIFFUSION_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pfDstVolume[v * 3 + 0] = pf4Volume_host[v].x;
				pfDstVolume[v * 3 + 1] = pf4Volume_host[v].y;
				pfDstVolume[v * 3 + 2] = pf4Volume_host[v].z;
			}
	CLOCK_END(PRINT_FLOW_DIFFUSION_TIMING, true);

	CLOCK_PRINT(PRINT_FLOW_DIFFUSION_TIMING);
}

} // extern "C"

