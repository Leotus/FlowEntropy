////////////////////////////////////////////
	#include <stdio.h>
	#include <texture_fetch_functions.h>

	#if	0	// DEL-BY-LEETEN 2009/12/17-BEGIN
		// ADD-BY-LEETEN 2009/11/25-BEGIN
		#define CHECK_ERROR_CONVERGENCE_BY_CUDPP	0

		#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
			#include "cudpp/cudpp.h"
			#pragma comment (lib, "cudpp32.lib")
		#endif
		// ADD-BY-LEETEN 2009/11/25-END

		#include "cuda_macro.h"

		// ADD-BY-LEETEN 12/14/2009-BEGIN
		#include "liblog.h"
		using namespace std;

		#define	M_PI	3.1415926535897932384626433832795f
		// ADD-BY-LEETEN 12/14/2009-END

		#include "libbuf.h"

	////////////////////////////////////////////
		#define PRINT_FLOW_FUSION_TIMING	1
		#define USE_SHARED_MEMORY			0

		// ADD-BY-LEETEN 12/14/2009-BEGIN
		#define SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING	1	
		// ADD-BY-LEETEN 12/14/2009-END

		// ADD-BY-LEETEN 11/04/2009-BEGIN
							// if this preprocessor is non zero, the volume is scanned via a for loop on the host
		#define DIFFUSION_BY_FOR_LOOP_ON_HOST		0
		// ADD-BY-LEETEN 11/04/2009-END

		// ADD-BY-LEETEN 10/02/2009-BEGIN
		#define BLOCK_DIM_X	16
		#define BLOCK_DIM_Y	12
		// ADD-BY-LEETEN 10/02/2009-END
	#endif	// DEL-BY-LEETEN 2009/12/17-END


	// ADD-BY-LEETEN 12/07/2009-BEGIN
	#include "FlowDiffusion_cuda.h"
	#include "FlowDiffusion.h"
	// ADD-BY-LEETEN 12/07/2009-END	

////////////////////////////////////////////
	static cudaExtent cVolumeExtent;
	static float4 *pf4Volume_host;
	static cudaPitchedPtr pcVolumePtrs_global[2];
	static texture<float4, 2, cudaReadModeElementType> t2dSrc;
	// ADD-BY-LEETEN 10/02/2009-BEGIN
						// array and texture reference for the weight and offset volume
	static cudaExtent cVolumeExtent_array;
	static float4 *pf4WeightOffsetVolume_host;
	static cudaArray *cWeightOffsetVolume_array = 0;
	static texture<float4, 3, cudaReadModeElementType> t3dWeightOffset;
    static cudaChannelFormatDesc cWeightOffsetChannelDesc = cudaCreateChannelDesc<float4>();
	// ADD-BY-LEETEN 10/02/2009-END

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
    static CUDPPConfiguration cConfig;
    static CUDPPHandle cScanplan = 0;
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP

	static cudaExtent cErrorVolumeExtent;
	static cudaPitchedPtr cErrorVolume_device;
	static cudaPitchedPtr cErrorSum_device;
	// ADD-BY-LEETEN 2009/11/25-END

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	int iNrOfVoxels;

	static texture<int, 2, cudaReadModeElementType> t2dAngleMap;
	static cudaArray *pcAngleMap_array;	// cuda array to hold the Gaussian kernels

	static cudaExtent cBinVolumeExtent;
	#if	0	// MOD-BY-LEETEN 12/17/2009-FROM:
		static cudaPitchedPtr cSrcBinVolumePtr_global;
		static cudaPitchedPtr cDstBinVolumePtr_global;
	#else	// MOD-BY-LEETEN 12/17/2009-TO:
	static cudaPitchedPtr cSrcBinVolume_pitched;
	static cudaPitchedPtr cDstBinVolume_pitched;
	#endif	// MOD-BY-LEETEN 12/17/2009-END

	#if	0	// MOD-BY-LEETEN 2009/12/17-FROM:
		static int *piSrcAngleBinVolume_host;
		static int *piDstAngleBinVolume_host;
	#else	// MOD-BY-LEETEN 2009/12/17-TO:
						// a temp. volume in the host side to stored the 
	static int *piBinVolume_host;
	#endif	// MOD-BY-LEETEN 2009/12/17-END

						// a texture tha represents a vector field to be converted into bins 
	static texture<float4, 2, cudaReadModeElementType> t2dVectorVolume;
	// ADD-BY-LEETEN 12/14/2009-END

// ADD-BY-LEETEN 12/07/2009-BEGIN
#if	0	// MOD-BY-LEETEN 2009/12/17-FROM:
	__global__ 
	static 
	void 
	_FlowDiffusion2D_kernel
	(
		// INPUT
		float fAttenuation,

		int iVolumeWidth,
		int iVolumeHeight,

		cudaPitchedPtr cDstPitchedPtr,

		// ADD-BY-LEETEN 2009/11/25-BEGIN
		cudaPitchedPtr cErrorPitchedPtr
		// ADD-BY-LEETEN 2009/11/25-END
	)
	{
 		int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
		int iVoxelY = blockIdx.y * blockDim.y + threadIdx.y;

		// compute the central differnece
		float4 f4Value	= tex2D(t2dSrc, iVoxelX,		iVoxelY);
		float4 f4PX		= tex2D(t2dSrc, min(iVoxelX + 1, iVolumeWidth - 1),	iVoxelY);
		float4 f4NX		= tex2D(t2dSrc, max(iVoxelX - 1, 0),				iVoxelY);
		float4 f4PY		= tex2D(t2dSrc, iVoxelX,		min(iVoxelY + 1, iVolumeHeight - 1));
		float4 f4NY		= tex2D(t2dSrc, iVoxelX,		max(iVoxelY - 1, 0)				);

		float4 f4WeightOffset = tex3D(t3dWeightOffset, iVoxelX,	iVoxelY, 0);

		float4 f4Result;

		f4Result = make_float4(
			f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x - 4.0f * f4Value.x) * fAttenuation + f4WeightOffset.x,
			f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y - 4.0f * f4Value.y) * fAttenuation + f4WeightOffset.y,
			f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z - 4.0f * f4Value.z) * fAttenuation + f4WeightOffset.z,
			0);

		if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
		{
			*ADDRESS_2D(
				float4, cDstPitchedPtr.ptr, 
				sizeof(float4), cDstPitchedPtr.pitch, 
				iVoxelX, iVoxelY) = f4Result;

			float4 f4Diff;
			f4Diff.x = f4Value.x - f4Result.x;
			f4Diff.y = f4Value.y - f4Result.y;
			f4Diff.z = f4Value.z - f4Result.z;
			float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
			*ADDRESS_2D(
				float, cErrorPitchedPtr.ptr, 
				sizeof(float), cErrorPitchedPtr.pitch, 
				iVoxelX, iVoxelY) = fDiff;
		}
	}
#else	// MOD-BY-LEETEN 2009/12/17-TO:
	#include "FlowDiffusion2D_kernel.cu"
#endif	// MOD-BY-LEETEN 2009/12/17-END

// ADD-BY-LEETEN 12/07/2009-END

#if	0		// DEL-BY-LEETEN 2009/12/17-BEGIN
	// ADD-BY-LEETEN 12/14/2009-BEGIN
	__global__ 
	static 
	void 
	// MOD-BY-LEETEN 12/07/2009-FROM:
		// _FlowFusion_kernel
	// TO:
	_Vector3DToVolume_kernel
	// MOD-BY-LEETEN 12/07/2009-END
	(
		int iVolumeWidth,
		int iVolumeHeight,
		int iVolumeDepth,

		int iNrOfYBlocks,
		int iBlockZSize,

		// texture<float4, 2, cudaReadModeElementType> t2dSrc,
		cudaPitchedPtr cBinVolumePtr_global
	)
	{
 		int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
		int iVoxelY = (blockIdx.y % iNrOfYBlocks) * blockDim.y + threadIdx.y;
		int iBeginZ = (blockIdx.y / iNrOfYBlocks) * iBlockZSize;
		int iEndZ = min(iBeginZ + iBlockZSize, iVolumeDepth);

		for(int z = iBeginZ; z < iEndZ; z++)
		{
			float4 f4Vector = tex2D(t2dVectorVolume, iVoxelX,		iVoxelY + z				* iVolumeHeight);;
			
			float fLength	= sqrt(f4Vector.x * f4Vector.x + f4Vector.y * f4Vector.y + f4Vector.z * f4Vector.z);
			float fTheta	= 0.0f;
			float fPhi		= 0.0f;
			int iBin = 0;

			if( 0 < fLength )
			{
				f4Vector.x /= fLength;
				f4Vector.y /= fLength;
				f4Vector.z /= fLength;
				fTheta = ( 0.0f == f4Vector.x && 0.0f == f4Vector.y )?0.0f:(M_PI+(atan2(f4Vector.y, f4Vector.x)));
				fTheta /= 2.0f * M_PI;

				float fLength2D = sqrt(f4Vector.x * f4Vector.x + f4Vector.y * f4Vector.y);
				fPhi = ((0.0f == fLength2D)&&(0.0f == f4Vector.z))?0.0f:fabs(M_PI/2.0f-(atan2(f4Vector.z, fLength2D)));
				fPhi /= M_PI;
				iBin = tex2D(t2dAngleMap, fPhi, fTheta);
			}

			if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
			{
				*ADDRESS_2D(
					int,			cBinVolumePtr_global.ptr, 
					sizeof(int),	cBinVolumePtr_global.pitch, 
					iVoxelX, iVoxelY + z * iVolumeHeight) = iBin;
			}
		}
	}
	// ADD-BY-LEETEN 12/14/2009-END
#endif	// DEL-BY-LEETEN 2009/12/17-END

// ADD-BY-LEETEN 12/17/2009-BEGIN
////////////////////////////////////////////

#include "EntropyField_kernel.cu"

__host__
static 
int 
IMirrorCoord_host(int iCoord, int iSize)
{
	int iMirroredCoord = iCoord;
	if( iCoord < 0 )
		iMirroredCoord = -iCoord;
	if( iCoord > iSize - 1 )
		iMirroredCoord = iSize - (iCoord - (iSize - 1));
	return iMirroredCoord;
}

static 
void 
_UpdateSliceToHistogram_host
(
	int iU,
	int iV,

	int3 i3Center, 
	int	iUpdateDir, 
	int iHisotgramOp, 

	int3 i3UDir, 
	int3 i3VDir,
	int iUKernelSize, 
	int iVKernelSize,

	int3 i3VolumeSize,
	int *piBinVolume_host,

	int iNrOfBins,
	int *piHistorgram_host
)
{
	int3 i3Point;
	i3Point.x = i3Center.x + (iU - iUKernelSize) * i3UDir.x + (iV - iVKernelSize) * i3VDir.x;
	i3Point.y = i3Center.y + (iU - iUKernelSize) * i3UDir.y + (iV - iVKernelSize) * i3VDir.y;
	i3Point.z = i3Center.z + (iU - iUKernelSize) * i3UDir.z + (iV - iVKernelSize) * i3VDir.z;

	// read the bin
	int3 i3TexCoord;
	i3TexCoord.x = IMirrorCoord_host(i3Point.x, i3VolumeSize.x);
	i3TexCoord.y = IMirrorCoord_host(i3Point.y, i3VolumeSize.y);
	i3TexCoord.z = IMirrorCoord_host(i3Point.z, i3VolumeSize.z);

	int iSrcBin = piBinVolume_host[i3TexCoord.x + i3TexCoord.y * i3VolumeSize.x + i3TexCoord.z * i3VolumeSize.x * i3VolumeSize.y];

	// update the histogram
	piHistorgram_host[iSrcBin] += iHisotgramOp; 
}

static 
void
_BuildHistogram_host
(
	int3 i3Center,
						// res. of the neighboring region
	int3 i3KernelSize,	
						// the bin volume
	int3 i3VolumeSize,	
	int *piBinVolume_host,								
						// res. of the joint histogram
	int iNrOfBins,		
	int *piHistorgram_host
)
{
	int3 i3Point = i3Center;
	i3Point.x -= i3KernelSize.x;
	for(int xi = 0; xi < 2 * i3KernelSize.x + 1; xi++, i3Point.x++)
		for(int		iV = 0; iV < 2 * i3KernelSize.z + 1; iV++)
			for(int iU = 0; iU < 2 * i3KernelSize.y + 1; iU++)
				_UpdateSliceToHistogram_host(
					iU,
					iV,

					i3Point , 
					UPDATE_DIR_X, 
					HISTOGRAM_OP_ADD_SLICE, 

					make_int3(0, 1, 0), 
					make_int3(0, 0, 1),
					i3KernelSize.y, 
					i3KernelSize.z,

					i3VolumeSize,
					piBinVolume_host,

					iNrOfBins,
					piHistorgram_host
				);
}

static 
void 
_UpdateSliceToHistogram_host
(
	int3 i3Point,										// the coordinate to be computed

	int iUpdateDir, 
	int iDir,

	int3 i3KernelSize,	// res. of the neighboring region

	int3 i3VolumeSize,	// res. of the volume
	int *piBinVolume_host,								


	int iNrOfBins,		// res. of the joint histogram
	int *piHistogram_host
)
{
	int3 i3Prev = i3Point;
	int3 i3Next = i3Point;
	int3 i3UDir;
	int3 i3VDir;
	int iUKernelSize ;
	int iVKernelSize ;

						// according the direction to divide the new slice into blocks
	switch(iUpdateDir)
	{
	case UPDATE_DIR_X:	
		i3UDir = make_int3(0, 1, 0);
		i3VDir = make_int3(0, 0, 1);
		iUKernelSize = i3KernelSize.y;
		iVKernelSize = i3KernelSize.z;
		i3Prev.x -= iDir * i3KernelSize.x;
		i3Next.x += iDir * i3KernelSize.x;
		break;
	case UPDATE_DIR_Y:	
		i3UDir = make_int3(1, 0, 0);
		i3VDir = make_int3(0, 0, 1);
		iUKernelSize = i3KernelSize.x;
		iVKernelSize = i3KernelSize.z;
		i3Prev.y -= iDir * i3KernelSize.y;
		i3Next.y += iDir * i3KernelSize.y;
		break;
	case UPDATE_DIR_Z:	
		i3UDir = make_int3(1, 0, 0);
		i3VDir = make_int3(0, 1, 0);
		iUKernelSize = i3KernelSize.y;
		iVKernelSize = i3KernelSize.z;
		i3Prev.z -= iDir * i3KernelSize.z;
		i3Next.z += iDir * i3KernelSize.z;
		break;
	} // switch

	for(int		iV = 0; iV < 2 * iVKernelSize + 1; iV++)
		for(int iU = 0; iU < 2 * iUKernelSize + 1; iU++)
		{
			_UpdateSliceToHistogram_host
			(
				iU,
				iV,

				i3Next, 
				iUpdateDir, 
				HISTOGRAM_OP_ADD_SLICE, 

				i3UDir, 
				i3VDir,
				iUKernelSize, 
				iVKernelSize,

				i3VolumeSize,
				piBinVolume_host,

				iNrOfBins,
				piHistogram_host
			);

			_UpdateSliceToHistogram_host
			(
				iU,
				iV,

				i3Prev, 
				iUpdateDir, 
				HISTOGRAM_OP_SUB_SLICE, 

				i3UDir, 
				i3VDir,
				iUKernelSize, 
				iVKernelSize,

				i3VolumeSize,
				piBinVolume_host,

				iNrOfBins,
				piHistogram_host
			);
		}
}

static 
void
_ComputeEntropy_host
(
	int3 i3Point,
	int3 i3KernelSize,

						// the histogram
	int iNrOfBins,
	int *piHistogram_host,

	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	float fEntropy = 0.0f;
	for(int b = 0; b < iNrOfBins; b++)
	{
		if( 0 == piHistogram_host[b] )
			continue;

		float fProb = float(piHistogram_host[b]) / float(i3KernelSize.x * i3KernelSize.y * i3KernelSize.z);
		fEntropy += fProb * log2f(fProb);
	}

	CUDA_SAFE_CALL(
		cudaMemcpy(
			ADDRESS_2D(
				float, cEntropyVolume_pitched.ptr, 
				sizeof(float), cEntropyVolume_pitched.pitch, 
				i3Point.x, i3Point.y + i3Point.z * i3VolumeSize.y),
			&fEntropy,
			sizeof(fEntropy),
			cudaMemcpyHostToDevice) );
}

void 
_ComputeEntropyVolume
(
						// res. of the neighboring region
	int3 i3KernelSize,

						// the histogram
	int iNrOfBins,
	int *piHistogram_global,
	float *pfLogHistogram_global,

    CUDPPHandle cScanPlanSum,

						// res. of the volume																
	int3 i3VolumeSize,	
						// bin volume																	
	cudaPitchedPtr		cBinVolume_pitched,								
	cudaPitchedPtr		cEntropyVolume_pitched
)
{
CLOCK_INIT(_ComputeEntropyVolume_PRINT_TIMING, __FUNCTION__ ": ");

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	int *piHistogram_host;
	piHistogram_host = (int*)calloc(sizeof(piHistogram_host[0]), iNrOfBins);
	assert(piHistogram_host);

	int *piBinVolume_host;
	piBinVolume_host = (int*)calloc(sizeof(piBinVolume_host[0]), i3VolumeSize.x * i3VolumeSize.y * i3VolumeSize.z);
	assert(piBinVolume_host);

	_GetSrcBinVolume(piBinVolume_host);

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	int xdir = +1;
	int ydir = +1;
	int zdir = +1;
	int x = 0;
	int y = 0;
	int z = 0;

	_BuildHistogram_host
	(
		make_int3(x, y, z),
							// res. of the neighboring region
		i3KernelSize,	
							// the bin volume
		i3VolumeSize,	
		piBinVolume_host,
							// the joint histogram
		iNrOfBins,		
		piHistogram_host
	);
CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	for(int				zi = 0; zi < i3VolumeSize.z;	zi++, z += zdir)
	{
		for(int			yi = 0; yi < i3VolumeSize.y;	yi++, y += ydir)
		{
			for(int		xi = 0; xi < i3VolumeSize.x;	xi++, x += xdir)
			{
				// fprintf(stderr, "%d, %d, %d\n", x, y, z);
				// (x,y,z): center of the volume
				// before enter this loop, the histogram in the region centering at (x, y, z) should have been available
				_ComputeEntropy_host
				(
					make_int3(x, y, z),
					i3KernelSize,

										// the joint histogram
					iNrOfBins,
					piHistogram_host,

					i3VolumeSize,
					cEntropyVolume_pitched
				);

				if( xi < i3VolumeSize.x - 1 )
					_UpdateSliceToHistogram_host(
						make_int3(x + xdir, y, z), 
						UPDATE_DIR_X,	
						xdir, 

						i3KernelSize, 

											// the bin volume
						i3VolumeSize,	
						piBinVolume_host,
											// the joint histogram
						iNrOfBins,		
						piHistogram_host
						);
			}
			x -= xdir;
			xdir *= -1;
			if( yi < i3VolumeSize.y - 1 )
				_UpdateSliceToHistogram_host(
					make_int3(x, y + ydir, z), 
					UPDATE_DIR_Y,	
					ydir, 
					
					i3KernelSize, 

										// the bin volume
					i3VolumeSize,	
					piBinVolume_host,
										// the joint histogram
					iNrOfBins,		
					piHistogram_host
					);
		}
		y -= ydir;
		ydir *= -1;
		if( zi < i3VolumeSize.z - 1 )
			_UpdateSliceToHistogram_host(
				make_int3(x, y, z + zdir), 
				UPDATE_DIR_Z,	
				zdir, 

				i3KernelSize, 

											// the bin volume
				i3VolumeSize,	
				piBinVolume_host,
									// the joint histogram
				iNrOfBins,		
				piHistogram_host
			);
	}
CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);
	free(piHistogram_host);
	free(piBinVolume_host);
CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

CUDPPHandle 
CInitEntropyField
(
	int iNrOfBins
)
{
	CUDPPHandle cScanPlan = 0;

	CUDPPConfiguration	cConfig;
	cConfig.op =		CUDPP_ADD;
	cConfig.datatype =	CUDPP_FLOAT;
	cConfig.algorithm =	CUDPP_SCAN;
	cConfig.options =	CUDPP_OPTION_BACKWARD | CUDPP_OPTION_INCLUSIVE;

	assert( CUDPP_SUCCESS  == cudppPlan(&cScanPlan,	cConfig, iNrOfBins, 1, 0) );

	return cScanPlan;
}

void
_GetSrcEntropyVolume
(
	int iNrOfBins,
	int iKernelWidth, int iKernelHeight, int iKernelDepth
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

	CUDPPHandle hScanPlanEntropy = CInitEntropyField(iNrOfBins);

	_ComputeEntropyVolume
	(
						// res. of the neighboring region
		make_int3(iKernelWidth, iKernelHeight, iKernelDepth),

						// the histogram
		iNrOfBins,
		piHistogram_global,
		pfLogHistogram_global,

		hScanPlanEntropy,
						// res. of the volume																
		make_int3(cVolumeExtent_array.width, cVolumeExtent_array.height, cVolumeExtent_array.depth),

						// bin volume																	
		cSrcBinVolume_pitched,
		cEntropyVolume_pitched
	);

	FREE_MEMORY(piHistogram_global);
	FREE_MEMORY(pfLogHistogram_global);
	FREE_MEMORY(cEntropyVolume_pitched.ptr);
	cudppDestroyPlan(hScanPlanEntropy);
}

void
_GetJointEntropyVolume
(
	int iNrOfSrcBins,
	int iNrOfDstBins,
	int iKernelWidth, int iKernelHeight, int iKernelDepth
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

	CUDPPHandle hScanPlanEntropy = CInitEntropyField(iNrOfBins);

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
			cVolumeExtent_array.width, 
			cVolumeExtent_array.height, 
			cVolumeExtent_array.depth),

		cJointBinVolume_pitched
	);
	CUT_CHECK_ERROR("_JointSrcDst_kernel() failed");

	_ComputeEntropyVolume
	(
						// res. of the neighboring region
		make_int3(iKernelWidth, iKernelHeight, iKernelDepth),

						// the histogram
		iNrOfBins,
		piHistogram_global,
		pfLogHistogram_global,

		hScanPlanEntropy,
						// res. of the volume																
		make_int3(cVolumeExtent_array.width, cVolumeExtent_array.height, cVolumeExtent_array.depth),

						// bin volume																	
		cJointBinVolume_pitched,
		cEntropyVolume_pitched
	);

	FREE_MEMORY(cJointBinVolume_pitched.ptr);
	FREE_MEMORY(piHistogram_global);
	FREE_MEMORY(pfLogHistogram_global);
	FREE_MEMORY(cEntropyVolume_pitched.ptr);
	cudppDestroyPlan(hScanPlanEntropy);
}

// ADD-BY-LEETEN 12/17/2009-END

////////////////////////////////////////////
#if	0	// MOD-BY-LEETEN 12/16/2009-FROM:
	__global__ 
	static 
	void 
	// MOD-BY-LEETEN 12/07/2009-FROM:
		// _FlowFusion_kernel
	// TO:
	_FlowDiffusion3D_kernel
	// MOD-BY-LEETEN 12/07/2009-END
	(
		// INPUT
		float fAttenuation,

		int iVolumeWidth,
		int iVolumeHeight,
		int iVolumeDepth,
		int iZ,

		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#if				!DIFFUSION_BY_FOR_LOOP_ON_HOST
		// ADD-BY-LEETEN 11/04/2009-END

		int iNrOfYBlocks,
		int iBlockZSize,

		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#endif // #if	!DIFFUSION_BY_FOR_LOOP_ON_HOST
		// ADD-BY-LEETEN 11/04/2009-END

		// output
		cudaPitchedPtr cDstPitchedPtr,

		// ADD-BY-LEETEN 2009/11/25-BEGIN
		cudaPitchedPtr cErrorPitchedPtr
		// ADD-BY-LEETEN 2009/11/25-END
	)
	{
 		int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#if				DIFFUSION_BY_FOR_LOOP_ON_HOST
		// ADD-BY-LEETEN 11/04/2009-BEGIN

		int iVoxelY = blockIdx.y * blockDim.y + threadIdx.y;

		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		int iVoxelY = (blockIdx.y % iNrOfYBlocks) * blockDim.y + threadIdx.y;
		int iBeginZ = (blockIdx.y / iNrOfYBlocks) * iBlockZSize;
		int iEndZ = min(iBeginZ + iBlockZSize, iVolumeDepth);
		#endif	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		// ADD-BY-LEETEN 11/04/2009-END

		#if	DIFFUSION_BY_FOR_LOOP_ON_HOST

			// compute the central differnece
			float4 f4Value	= tex2D(t2dSrc, iVoxelX,		iVoxelY + iZ * iVolumeHeight);
			float4 f4PX		= tex2D(t2dSrc, iVoxelX + 1,	iVoxelY + iZ * iVolumeHeight);
			float4 f4NX		= tex2D(t2dSrc, iVoxelX - 1,	iVoxelY + iZ * iVolumeHeight);
			float4 f4PY		= tex2D(t2dSrc, iVoxelX,		min(iVoxelY + 1, iVolumeHeight - 1) + iZ * iVolumeHeight);
			float4 f4NY		= tex2D(t2dSrc, iVoxelX,		max(iVoxelY - 1, 0)					+ iZ * iVolumeHeight);
			float4 f4PZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + min(iZ + 1, iVolumeDepth - 1) * iVolumeHeight);
			float4 f4NZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + max(iZ - 1, 0)				* iVolumeHeight);

		#if	0	// MOD-BY-LEETEN 11/04/2009-FROM:
		#else	// MOD-BY-LEETEN 11/04/2009-TO:
			float4 f4WeightOffset = tex3D(t3dWeightOffset, iVoxelX,	iVoxelY, iZ);

			#if	0	// MOD-BY-LEETEN 2009/11/25-FROM:
			#else	// MOD-BY-LEETEN 2009/11/25-TO:
			float4 f4Result;

			#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
			#endif	// DEL-BY-LEETEN 12/07/2009-END

			#if	0	// MOD-BY-LEETEN 12/07/2009-FROM:
				f4Result = make_float4(
					f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuationDividedBy6 + f4WeightOffset.x,
					f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuationDividedBy6 + f4WeightOffset.y,
					f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuationDividedBy6 + f4WeightOffset.z,
					0);
			#else	// MOD-BY-LEETEN 12/07/2009-TO:
			f4Result = make_float4(
				f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuation + f4WeightOffset.x,
				f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuation + f4WeightOffset.y,
				f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuation + f4WeightOffset.z,
				0);
			#endif	// MOD-BY-LEETEN 12/07/2009-END

			if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
			{
				*ADDRESS_2D(
					float4, cDstPitchedPtr.ptr, 
					sizeof(float4), cDstPitchedPtr.pitch, 
					iVoxelX, iVoxelY + iZ * iVolumeHeight) = f4Result;

				float4 f4Diff;
				f4Diff.x = f4Value.x - f4Result.x;
				f4Diff.y = f4Value.y - f4Result.y;
				f4Diff.z = f4Value.z - f4Result.z;
				float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
				*ADDRESS_2D(
					float, cErrorPitchedPtr.ptr, 
					sizeof(float), cErrorPitchedPtr.pitch, 
					iVoxelX, iVoxelY + iZ * iVolumeHeight) = fDiff;
			}
		#endif	// MOD-BY-LEETEN 2009/11/25-END

		#endif	// MOD-BY-LEETEN 11/04/2009-END

		// MOD-BY-LEETEN 11/04/2009-FROM:
			// #else	// MOD-BY-LEETEN 10/02/2009-TO:
		// TO:
		#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		// MOD-BY-LEETEN 11/04/2009-END

		float4 f4Value;
		float4 f4PX;
		float4 f4NX;
		float4 f4PY;
		float4 f4NY;
		float4 f4PZ;
		float4 f4NZ;

		#if	0	// MOD-BY-LEETEN 11/04/2009-FROM:
		#else	// MOD-BY-LEETEN 11/04/2009-TO:

		float4 *pf4DstPtr = ADDRESS_2D(
								float4, cDstPitchedPtr.ptr, 
								sizeof(float4), cDstPitchedPtr.pitch, 
								iVoxelX, iVoxelY + iBeginZ * iVolumeHeight);

		// ADD-BY-LEETEN 2009/11/25-BEGIN
		float *pfErrorPtr = ADDRESS_2D(
								float, cErrorPitchedPtr.ptr, 
								sizeof(float), cErrorPitchedPtr.pitch, 
								iVoxelX, iVoxelY + iBeginZ * iVolumeHeight);
		// ADD-BY-LEETEN 2009/11/25-END

		#endif	// MOD-BY-LEETEN 11/04/2009-END

			#if	0	// MOD-BY-LEETEN 11/04/2009-FROM:
			#else	// MOD-BY-LEETEN 11/04/2009-TO:
			f4Value		= tex2D(t2dSrc, iVoxelX,		iVoxelY + iBeginZ				* iVolumeHeight);
			f4NZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + max(iBeginZ - 1, 0)	* iVolumeHeight);
			#endif	// MOD-BY-LEETEN 11/04/2009-END

			for(int z = iBeginZ; z < iEndZ; z++, f4NZ = f4Value, f4Value = f4PZ)
			{
				// ADD-BY-LEETEN 10/02/2009-BEGIN
				float4 f4WeightOffset = tex3D(t3dWeightOffset, iVoxelX,	iVoxelY, z);
				// ADD-BY-LEETEN 10/02/2009-END
				// f4Value		= tex2D(t2dSrc, iVoxelX,		iVoxelY + z * iVolumeHeight);
				#if	!USE_SHARED_MEMORY
				f4PX		= tex2D(t2dSrc, iVoxelX + 1,	iVoxelY + z * iVolumeHeight);
				f4NX		= tex2D(t2dSrc, iVoxelX - 1,	iVoxelY + z * iVolumeHeight);
				f4PY		= tex2D(t2dSrc, iVoxelX,		min(iVoxelY + 1, iVolumeHeight - 1) + z * iVolumeHeight);
				f4NY		= tex2D(t2dSrc, iVoxelX,		max(iVoxelY - 1, 0)					+ z * iVolumeHeight);

				#else	// #if	!USE_SHARED_MEMORY
				#endif	// #if	!USE_SHARED_MEMORY
				f4PZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + min(z + 1, iVolumeDepth - 1) * iVolumeHeight);
				// f4NZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + max(z - 1, 0)				* iVolumeHeight);

				// store the result back to the dst.
				#if	0	// MOD-BY-LEETEN 2009/11/10-FROM:
				#else	// MOD-BY-LEETEN 2009/11/10-TO:
				float4 f4Result;

				#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
				#endif	// DEL-BY-LEETEN 12/07/2009-END	

					#if	0	// MOD-BY-LEETEN 12/07/2009-FROM:
						f4Result = make_float4(
							f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuationDividedBy6 + f4WeightOffset.x,
							f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuationDividedBy6 + f4WeightOffset.y,
							f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuationDividedBy6 + f4WeightOffset.z,
							0);
					#else	// MOD-BY-LEETEN 12/07/2009-TO:
					f4Result = make_float4(	
						f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuation + f4WeightOffset.x,
						f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuation + f4WeightOffset.y,
						f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuation + f4WeightOffset.z,
						0.0);
					#endif	// MOD-BY-LEETEN 12/07/2009-END
				#endif	// MOD-BY-LEETEN 2009/11/10-TO:

				// ADD-BY-LEETEN 11/04/2009-BEGIN
				if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
				// ADD-BY-LEETEN 11/04/2009-END
				{
					pf4DstPtr[0] = f4Result;

					// ADD-BY-LEETEN 2009/11/25-BEGIN
					float4 f4Diff;
					f4Diff.x = f4Value.x - f4Result.x;
					f4Diff.y = f4Value.y - f4Result.y;
					f4Diff.z = f4Value.z - f4Result.z;
					float fDiff;
					// float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
					// MOD-BY-LEETEN 12/16/2009-FROM:
						// fDiff = max(abs(f4Diff.x), max(abs(f4Diff.y), abs(f4Diff.z)));
					// TO:
					fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
					// MOD-BY-LEETEN 12/16/2009-END
					pfErrorPtr[0] = fDiff;
					// ADD-BY-LEETEN 2009/11/25-END
				}

				pf4DstPtr += iVolumeHeight * cDstPitchedPtr.pitch / sizeof(float4);

				// ADD-BY-LEETEN 2009/11/25-BEGIN
				pfErrorPtr += iVolumeHeight * cErrorPitchedPtr.pitch / sizeof(float);
				// ADD-BY-LEETEN 2009/11/25-END
			}

		#endif	// MOD-BY-LEETEN 10/02/2009-END
	}
#else	// MOD-BY-LEETEN 12/16/2009-TO:
	#include "FlowDiffusion3D_kernel.cu"
#endif	// MOD-BY-LEETEN 12/16/2009-END

extern "C" {

// MOD-BY-LEETEN 12/07/2009-FROM:
	// void _FlowFusionFree()
// TO:
void 
_FlowDiffusionFree()
// MOD-BY-LEETEN 12/07/2009-END
{
	FREE_MEMORY_ON_HOST(pf4Volume_host);
	for(int i = 0; i < 2; i++)
		FREE_MEMORY(pcVolumePtrs_global[i].ptr);
	FREE_ARRAY(cWeightOffsetVolume_array);
	FREE_MEMORY_ON_HOST(pf4WeightOffsetVolume_host);
	// ADD-BY-LEETEN 2009/11/25-BEGIN
	FREE_MEMORY(cErrorVolume_device.ptr);
	FREE_MEMORY(cErrorSum_device.ptr);

	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	cudppDestroyPlan(cScanplan);
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	// ADD-BY-LEETEN 2009/11/25-END

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	FREE_ARRAY(pcAngleMap_array);	

	FREE_MEMORY(cSrcBinVolume_pitched.ptr);
	FREE_MEMORY(cDstBinVolume_pitched.ptr);
	// ADD-BY-LEETEN 12/17/2009-BEGIN
	FREE_MEMORY(cSrcDstBinVolumePtr_global.ptr);
	// ADD-BY-LEETEN 12/17/2009-END

	#if	0	// MOD-BY-LEETEN 12/16/2009-FROM:
		FREE_MEMORY_ON_HOST(piSrcAngleBinVolume_host);
		FREE_MEMORY_ON_HOST(piDstAngleBinVolume_host);
	#else	// MOD-BY-LEETEN 12/16/2009-TO:
	FREE_MEMORY_ON_HOST(piBinVolume_host);
	#endif	// MOD-BY-LEETEN 12/16/2009-END
	// ADD-BY-LEETEN 12/14/2009-END
}

void
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusionInit(
// TO:
_FlowDiffusionInit(
// MOD-BY-LEETEN 12/07/2009-END
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth
)
{
	// MOD-BY-LEETEN 12/07/2009-FROM:
		// CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, "_FlowFusionInit(): ");
	// TO:
	CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, __FUNCTION__ ": ");
	// MOD-BY-LEETEN 12/07/2009-END

	// MOD-BY-LEETEN 12/14/2009-FROM:
		// int iNrOfVoxels = iVolumeWidth * iVolumeHeight * iVolumeDepth;
	// TO:
	iNrOfVoxels = iVolumeWidth * iVolumeHeight * iVolumeDepth;
	// MOD-BY-LEETEN 12/14/2009-END

	cVolumeExtent	= make_cudaExtent(
		iVolumeWidth * sizeof(float4),
		iVolumeHeight,
		iVolumeDepth);
	// allocate a local copy in the CPU side
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&pf4Volume_host,
			sizeof(pf4Volume_host[0]) * iNrOfVoxels) );
	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// Allocate 2 volumes on the  GPU's global memory.
	// in each iteration, one of them will be used as the source
	// 3D texture, and the other will be served as the dst.
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

	for(int i = 0; i < 2; i++)
	{
		CUDA_SAFE_CALL( 
			cudaMalloc3D(&pcVolumePtrs_global[i], cVolumeExtent) );
	}

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	// ADD-BY-LEETEN 2009/12/17-BEGIN
	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
	// ADD-BY-LEETEN 2009/12/17-END
	cErrorVolumeExtent	= make_cudaExtent(
		// MOD-BY-LEETEN 12/16/2009-FROM:
			// iVolumeWidth,
		// TO:
		iVolumeWidth * sizeof(float),
		// MOD-BY-LEETEN 12/16/2009-END
		iVolumeHeight,
		iVolumeDepth);
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cErrorVolume_device,	cErrorVolumeExtent) );
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cErrorSum_device,		cErrorVolumeExtent) );

	// DEL-BY-LEETEN 12/16/2009-BEGIN
	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	// DEL-BY-LEETEN 12/16/2009-END
	cConfig.op = CUDPP_ADD;
	// cConfig.op = CUDPP_MAX;
	cConfig.datatype = CUDPP_FLOAT;
	cConfig.algorithm = CUDPP_SCAN;
	#if	0	// MOD-BY-LEETEN 12/16/2009-FROM:
		cConfig.options = CUDPP_OPTION_FORWARD | CUDPP_OPTION_INCLUSIVE;
		cudppPlan(&cScanplan, cConfig, iVolumeWidth * iVolumeHeight * iVolumeDepth, iVolumeHeight * iVolumeDepth, cErrorVolume_device.pitch);  
	#else	// MOD-BY-LEETEN 12/16/2009-TO:
	cConfig.options = CUDPP_OPTION_BACKWARD | CUDPP_OPTION_INCLUSIVE;
	assert(CUDPP_SUCCESS  == cudppPlan(&cScanplan, cConfig, iVolumeWidth * iVolumeHeight * iVolumeDepth, iVolumeHeight * iVolumeDepth, cErrorVolume_device.pitch) );  
	#endif	// MOD-BY-LEETEN 12/16/2009-END
	// DEL-BY-LEETEN 2009/12/17-BEGIN
	// #endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	// DEL-BY-LEETEN 2009/12/17-END
	// ADD-BY-LEETEN 2009/12/17-BEGIN
	#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	// ADD-BY-LEETEN 2009/12/17-END
	// ADD-BY-LEETEN 2009/11/25-END

	// ADD-BY-LEETEN 10/02/2009-BEGIN
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
	// ADD-BY-LEETEN 10/02/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	CLOCK_PRINT(PRINT_FLOW_FUSION_TIMING);
}

// ADD-BY-LEETEN 12/14/2009-BEGIN
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
			iNrOfPhis * sizeof(piAngleMap), 
			iNrOfPhis * sizeof(piAngleMap), 
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

	// ADD-BY-LEETEN 12/17/2009-BEGIN
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
	// ADD-BY-LEETEN 12/17/2009-END


	// allocate the volume of bins in the host side
	#if	0	// MOD-BY-LEETEN 2009/12/17-FROM:
		CUDA_SAFE_CALL(
			cudaMallocHost(
				(void**)&piSrcAngleBinVolume_host,
				sizeof(piSrcAngleBinVolume_host[0]) * iNrOfVoxels) );

		CUDA_SAFE_CALL(
			cudaMallocHost(
				(void**)&piDstAngleBinVolume_host,
				sizeof(piDstAngleBinVolume_host[0]) * iNrOfVoxels) );
	#else	// MOD-BY-LEETEN 2009/12/17-TO:
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&piBinVolume_host,
			sizeof(piBinVolume_host[0]) * iNrOfVoxels) );
	#endif	// MOD-BY-LEETEN 2009/12/17-END

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
	_Vector3DToVolume_kernel<<<v3Grid, v3Blk, 0>>>
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
	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr		= cSrcBinVolume_pitched;
	cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		// MOD-BY-LEETEN 2009/12/17-FROM:
			// (void*)piSrcAngleBinVolume_host, 
		// TO:
		(void*)piBinVolume_host, 
		// MOD-BY-LEETEN 2009/12/17-END
		cVolumeExtent_array.width * sizeof(int),	// cSrcBinVolume_pitched.width, 
		cVolumeExtent_array.width * sizeof(int), 
		cVolumeExtent_array.height);
	cCopyParamsDeviceToHost.extent		= cBinVolumeExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	// MOD-BY-LEETEN 2009/12/17-FROM:
		// memcpy(piBinVolume, piSrcAngleBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);
	// TO:
	memcpy(piBinVolume, piBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);
	// MOD-BY-LEETEN 2009/12/17-END
}

void 
_GetDstBinVolume(int *piBinVolume)
{
	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr		= cDstBinVolume_pitched;
	cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		// MOD-BY-LEETEN 2009/12/17-FROM:
			// (void*)piDstAngleBinVolume_host,		
		// TO:
		(void*)piBinVolume_host, 
		// MOD-BY-LEETEN 2009/12/17-END

		cVolumeExtent_array.width * sizeof(int),	// cDstBinVolume_pitched.width, 
		cVolumeExtent_array.width * sizeof(int), 
		cVolumeExtent_array.height);
	cCopyParamsDeviceToHost.extent		= cBinVolumeExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	// MOD-BY-LEETEN 2009/12/17-FROM:
		// memcpy(piBinVolume, piDstAngleBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);
	// TO:
	memcpy(piBinVolume, piBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);
	// MOD-BY-LEETEN 2009/12/17-END
}

// ADD-BY-LEETEN 12/14/2009-END

void
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusion(
// TO:
_FlowDiffusion(
// MOD-BY-LEETEN 12/07/2009-END
	float fAttenuation,
	int iNrOfIterations,
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	// ADD-BY-LEETEN 10/02/2009-BEGIN
	int iBlockZSize,
	float *pfWeightVolume,
	// MOD-BY-LEETEN 2009/11/10-FROM:
		// float *pfOffsetVolume,
	// TO:
	float *pfXOffsetVolume,
	float *pfYOffsetVolume,
	float *pfZOffsetVolume,
	// MOD-BY-LEETEN 2009/11/10-END
	// ADD-BY-LEETEN 10/02/2009-END
	float *pfSrcVolume,
	float *pfDstVolume,
	// ADD-BY-LEETEN 2009/11/10-BEGIN
	int *piFlagVolume
	// ADD-BY-LEETEN 2009/11/10-END
)
{
	// ADD-BY-LEETEN 12/07/2009-BEGIN
	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
	iNrOfIterations = 4 * max(max(iVolumeWidth, iVolumeHeight), iVolumeDepth);
	// LOG(printf("Warning! iNrOfIterations is changed to %d", iNrOfIterations));
	#endif
	// ADD-BY-LEETEN 12/07/2009-END
	

	// MOD-BY-LEETEN 12/07/2009-FROM:
		// CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, "_FlowFusion(): ");
	// TO:
	// MOD-BY-LEETEN 12/14/2009-FROM:
		// CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, __FUNCTION__);
	// TO:
	CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, __FUNCTION__ ": ");
	// MOD-BY-LEETEN 12/07/2009-END

	// MOD-BY-LEETEN 10/02/2009-FROM:
		// dim3 v3Blk = dim3(16, 8);
	// TO:
	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	// MOD-BY-LEETEN 10/02/2009-END
	dim3 v3Grid = dim3(
		(unsigned int)ceilf((float)iVolumeWidth	 / (float)v3Blk.x),
		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#if					DIFFUSION_BY_FOR_LOOP_ON_HOST	
		// ADD-BY-LEETEN 11/04/2009-END

		(unsigned int)ceilf((float)iVolumeHeight / (float)v3Blk.y));

		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#else	// #if		DIFFUSION_BY_FOR_LOOP_ON_HOST
		unsigned int(ceilf( float(iVolumeHeight) / float(v3Blk.y) ) ) *
		unsigned int(ceilf( float(iVolumeDepth) /  float(iBlockZSize) ) ) );
		#endif	// #if		DIFFUSION_BY_FOR_LOOP_ON_HOST
		// ADD-BY-LEETEN 11/04/2009-END

	// convert the #channels in the src volume from 3 to 4
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pf4Volume_host[v].x = pfSrcVolume[v * 3 + 0];
				pf4Volume_host[v].y = pfSrcVolume[v * 3 + 1];
				pf4Volume_host[v].z = pfSrcVolume[v * 3 + 2];

				// ADD-BY-LEETEN 2009/11/10-BEGIN
				if( piFlagVolume )
					pf4Volume_host[v].w = float(piFlagVolume[v]);
				else
				// ADD-BY-LEETEN 2009/11/10-END
				pf4Volume_host[v].w = 0.0f;
			}

	// ADD-BY-LEETEN 10/02/2009-BEGIN
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				#if	0	// MOD-BY-LEETEN 2009/11/10-FROM:
				#else	// MOD-BY-LEETEN 2009/11/10-TO:
				// ADD-BY-LEETEN 12/07/2009-BEGIN
				if( pfXOffsetVolume )
				// ADD-BY-LEETEN 12/07/2009-END
				pf4WeightOffsetVolume_host[v].x = pfXOffsetVolume[v];
				// ADD-BY-LEETEN 12/07/2009-BEGIN
				if( pfYOffsetVolume )
				// ADD-BY-LEETEN 12/07/2009-END
				pf4WeightOffsetVolume_host[v].y = pfYOffsetVolume[v];
				// ADD-BY-LEETEN 12/07/2009-BEGIN
				if( pfZOffsetVolume )
				// ADD-BY-LEETEN 12/07/2009-END
				pf4WeightOffsetVolume_host[v].z = pfZOffsetVolume[v];
				pf4WeightOffsetVolume_host[v].w = 1.0f - pfWeightVolume[v];
				#endif	// MOD-BY-LEETEN 2009/11/10-END
			}
	// ADD-BY-LEETEN 10/02/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// Upload the input volume to the first volume on the GPU
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

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

	// ADD-BY-LEETEN 10/02/2009-BEGIN
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
	// ADD-BY-LEETEN 10/02/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// calcuate the fusion operator on GPUs
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

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


		// ADD-BY-LEETEN 12/07/2009-BEGIN
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
		// ADD-BY-LEETEN 12/07/2009-END

		// launch the kernel to compute the diffusion operator for 1 iteration
		// MOD-BY-LEETEN 11/04/2009-FROM:
			// #if	0	// MOD-BY-LEETEN 10/02/2009-FROM:
		// TO:
		#if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		// MOD-BY-LEETEN 11/04/2009-END

			for(int z = 0; z < iVolumeDepth; z++)
				// MOD-BY-LEETEN 12/07/2009-FROM:
					// _FlowFusion_kernel<<<v3Grid, v3Blk, 0>>>
				// TO:
				_FlowDiffusion3D_kernel<<<v3Grid, v3Blk, 0>>>
				// MOD-BY-LEETEN 12/07/2009-END
				(
					fAttenuation,
					iVolumeWidth,
					iVolumeHeight,
					iVolumeDepth,
					z,

					// output
					pcVolumePtrs_global[1 - iSrc],
					// ADD-BY-LEETEN 2009/11/25-BEGIN
					cErrorVolume_device
					// ADD-BY-LEETEN 2009/11/25-END
				);	
		// MOD-BY-LEETEN 11/04/2009-FROM:
			// #else	// MOD-BY-LEETEN 10/02/2009-TO:
		// TO:
		#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		// MOD-BY-LEETEN 11/04/2009-END

		// _FlowFusion_kernel<<<v3Grid, v3Blk, v3Blk.x * v3Blk.y * sizeof(float4)>>>
		// MOD-BY-LEETEN 12/07/2009-FROM:
			// _FlowFusion_kernel<<<v3Grid, v3Blk, 0>>>
		// TO:
		_FlowDiffusion3D_kernel<<<v3Grid, v3Blk, 0>>>
		// MOD-BY-LEETEN 12/07/2009-END
		(
			fAttenuation,
			iVolumeWidth,
			iVolumeHeight,
			iVolumeDepth,
			0,
			// ADD-BY-LEETEN 11/04/2009-BEGIN
			int(ceilf(float(iVolumeHeight) / float(BLOCK_DIM_Y))),
			iBlockZSize,
			// ADD-BY-LEETEN 11/04/2009-END
			// output
			pcVolumePtrs_global[1 - iSrc],
			// ADD-BY-LEETEN 2009/11/25-BEGIN
			cErrorVolume_device
			// ADD-BY-LEETEN 2009/11/25-END
		);
		#endif	// MOD-BY-LEETEN 10/02/2009-END
		// ADD-BY-LEETEN 12/07/2009-BEGIN
		}
		// ADD-BY-LEETEN 12/07/2009-END
		CUT_CHECK_ERROR("_FlowFusion_kernel() failed");

		// ADD-BY-LEETEN 2009/11/25-BEGIN
		#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
		cudppScan(cScanplan, cErrorSum_device.ptr, cErrorVolume_device.ptr, iVolumeWidth * iVolumeHeight * iVolumeDepth);

		float fError;
		#if	0	// MOD-BY-LEETEN 2009/12/17-FROM:
			CUDA_SAFE_CALL( 
				cudaMemcpy(
					&fError, 
					ADDRESS_2D(
						float, cErrorSum_device.ptr, 
						sizeof(float), cErrorSum_device.pitch, 
						iVolumeWidth - 1, iVolumeHeight - 1 + (iVolumeDepth - 1 ) * iVolumeHeight),
					sizeof(float), 
					cudaMemcpyDeviceToHost));
		#else	// MOD-BY-LEETEN 2009/12/17-TO:
	    CUDA_SAFE_CALL_NO_SYNC( 
			cudaMemcpy(
				&fError, 
				cErrorSum_device.ptr, 
				sizeof(float), 
				cudaMemcpyDeviceToHost));
		#endif	// MOD-BY-LEETEN 2009/12/17-END

		/*
		fError /= float(iVolumeWidth * iVolumeHeight * iVolumeDepth * 3);
		fError = sqrtf(fError);
		*/
		#if	0	// MOD-BY-LEETEN 2009/12/17-FROM:
			fprintf(stderr, "Error: %f\n", fError);
			if( fError < 1e-10 )
				break;
		#else	// MOD-BY-LEETEN 2009/12/17-TO:
		fError /= float(iVolumeWidth * iVolumeHeight * iVolumeDepth * 3);
		fError = sqrtf(fError);
		// fprintf(stderr, "Error: %f\n", fError);

		static float fPrevError = -1.0f;
		float fErrorDif = fabsf(fPrevError - fError);
		if( i > max(max(iVolumeWidth, iVolumeHeight), iVolumeDepth) && (fErrorDif/fPrevError < 0.01f || fErrorDif < 0.000001f) )
		{
			printf("\t#iters = %d;", i);
			break;
		}
		fPrevError = fError;
		#endif	// MOD-BY-LEETEN 2009/12/17-END

		#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
		// ADD-BY-LEETEN 2009/11/25-END

	}

	// ADD-BY-LEETEN 12/14/2009-BEGIN
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
	// ADD-BY-LEETEN 12/14/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	int iDst = iSrc;

	// read the result from latest iteration back to the CPU side
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

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

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// convert the #channels in the dst volume from 4 to 3
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pfDstVolume[v * 3 + 0] = pf4Volume_host[v].x;
				pfDstVolume[v * 3 + 1] = pf4Volume_host[v].y;
				pfDstVolume[v * 3 + 2] = pf4Volume_host[v].z;
			}
	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	CLOCK_PRINT(PRINT_FLOW_FUSION_TIMING);
}

} // extern "C"

/*

$Log: not supported by cvs2svn $
Revision 1.2  2009/12/15 20:05:57  leeten

[12/15/2009]
1. [ADD] Define new function _FlowDiffusionSetAngleMap() to setup the lookup table that converts a vector to a discrete bin.
2. [ADD] Define new function _GetSrcBinVolume() to download the bin of the orginal input vector field.
3. [ADD] Define new function _GetDstBinVolume() to download the bin for the diffused vector field.
4. [ADD] Define new function _ComputeSrcBinVolume() to download the bin of the orginal input vector field.
5. [ADD] Define a host function _Vector3DToVolume to convert each vector in a vector field into a bin.
6. [ADD] Define a kernel function _Vector3DToVolume_kernel() to convert each vector in a vector field into a bin on GPUs.

2. [ADD] Define new functions

Revision 1.1.1.1  2009/12/07 20:04:02  leeten

[12/07/2009]
1. [1ST] First time checkin.

Revision 1.3  2009/12/05 21:19:51  leeten

[12/05/2009]
1. [ADD] Add a 3D texture glWeightOffset to store the weights and offsets.
2. [ADD] Specify the flag donochange to the w channel in the 3D textures.
3. [ADD] Output the error to a 3D volume.

Revision 1.2  2009/11/04 19:11:13  leeten

[2009/11/04]
1. [ADD] Add a new preprocessor USE_SHARED_MEMORY to decide whether the shared memory is utilized.
2. [ADD] Add a new preprocessor DIFFUSION_BY_FOR_LOOP_ON_HOSTto decide whether the diffusion is executed via a for loop on the host side to scan throught all XY layers.
If it is not zero, the volume will be divided along the Z direction as well, and each block will be assigned as a single block on CUDA.
3. [ADD] Add 2 new preprocessors BLOCK_DIM_X and BLOCK_DIM_Y to control the block dimension.
4. [ADD] Add 1 array and 1 3D textrue for the weight/offset volume.

Revision 1.1.1.1  2009/11/02 15:30:56  leeten

[11/02/2009]
1 [1ST] Firs time checkin.


*/
