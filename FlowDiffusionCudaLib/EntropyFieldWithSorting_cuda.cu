#if	COMPUTE_ENTROPY_VOLUME_WITH_SORTING_CUDA

__global__ 
void
_ComputeEntropyOnSortedNeighbors_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	unsigned int *puActiveVoxelSortedNeighbors_global,
	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);
	float fNrOfNeighbors = float(iNrOfNeighbors);

	float fEntropy = 0.0f;
	unsigned int uPrevBin = 0;
	float fCount = 0.0f; 

	unsigned int uNrOfActiveVoxels = gridDim.x * blockDim.x * gridDim.y * blockDim.y;
	unsigned int uTexIndex = iNrOfNeighbors * iActiveVoxelId;

	for(int n = 0; n < iNrOfNeighbors; n++)
	{
		float fTexCoordX = float(uTexIndex % uNrOfActiveVoxels);
		float fTexCoordY = float(uTexIndex / uNrOfActiveVoxels);
		unsigned int uBin = tex2D(t2dActiveVoxelSortedNeighbors, fTexCoordX, fTexCoordY);
		uTexIndex++;

		if( n == 0 || uBin == uPrevBin )
		{
			fCount += 1.0f;
		}
		else
		{
			if( fCount > 0.0f )
				fEntropy += fCount * log2(fCount);
			
			fCount = 1.0f;
		}
		uPrevBin = uBin;
	}
	if( fCount > 0.0f )
		fEntropy += fCount * log2(fCount);

	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	fEntropy = max(fEntropy, 0.0f);

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z )
		*ADDRESS_2D(
			float, cEntropyVolume_pitched.ptr, 
			sizeof(float), cEntropyVolume_pitched.pitch, 
			iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = fEntropy;
}

__global__ 
void 
_CollectNeighbors_kernel
(
	int3 i3BlockCorner,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	{
		for(int	iOffset = 0,iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
			for(int			iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
				for(int		iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++, iOffset++)
				{
					int iX, iY, iZ;
					iX = iVoxelX + iXOffset;
					iY = iVoxelY + iYOffset;
					iZ = iVoxelZ + iZOffset;

					int3 i3TexCoord;
					i3TexCoord.x = IMirrorCoord(iX, i3VolumeSize.x);
					i3TexCoord.y = IMirrorCoord(iY, i3VolumeSize.y);
					i3TexCoord.z = IMirrorCoord(iZ, i3VolumeSize.z);

					int iSrcBin = tex2D(t2dSrcBinVolume, i3TexCoord.x, i3TexCoord.y + i3TexCoord.z * i3VolumeSize.y);

					*ADDRESS_2D(
						unsigned int,			cActiveVoxelNeighbors_pitched.ptr, 
						sizeof(unsigned int),	cActiveVoxelNeighbors_pitched.pitch, 
						iActiveVoxelId, iOffset) = unsigned int(iActiveVoxelId * iNrOfBins + iSrcBin);
				}
	}
}

void 
_ComputeEntropyVolumeWithSorting_cuda
(
						// res. of the neighboring region
	int3 i3KernelSize,

						// the histogram
	int iNrOfBins,
	int *piHistogram_global,
	float *pfLogHistogram_global,

						// res. of the volume																
	int3 i3VolumeSize,	
						// bin volume																	
	cudaPitchedPtr		cBinVolume_pitched,								
	cudaPitchedPtr		cEntropyVolume_pitched
)
{
CLOCK_INIT(_ComputeEntropyVolume_PRINT_TIMING, __FUNCTION__ ": ");

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);
	const int iNrOfRows = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	/*
	const unsigned int iMaxMemorySpace = MAX_MEMORY_SIZE;
	int iMaxNrOfBlocks = int(floorf(
		float(iMaxMemorySpace/2) / 
		float(sizeof(int) * iNrOfRows * v3Blk.x * v3Blk.y)));
	*/
	int iNrOfXBlocks = int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x)));
	int iNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	int iMaxNrOfThreads = int(floor(double(1<<(RADIX_SORT_BITS-1)) / double(iNrOfBins)));
	int iMaxNrOfBlocks	= int(floorf(float(iMaxNrOfThreads) / float(v3Blk.x * v3Blk.y)));
	iMaxNrOfBlocks = min(iMaxNrOfBlocks, iNrOfXBlocks * iNrOfYBlocks);
	iNrOfXBlocks = min(iNrOfXBlocks, iMaxNrOfBlocks);
	iNrOfYBlocks = int(ceilf(float(iMaxNrOfBlocks) / float(iNrOfXBlocks)));
	dim3 v3Grid = dim3(iNrOfXBlocks, iNrOfYBlocks);
	fprintf(stderr, "#BLOCKS = %d x %d\n", v3Grid.x, v3Grid.y);

	cudaPitchedPtr cActiveVoxelNeighbors_pitched;
	cActiveVoxelNeighbors_pitched.xsize = v3Grid.x * v3Grid.y * v3Blk.x * v3Blk.y;
	cActiveVoxelNeighbors_pitched.ysize = iNrOfRows;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocPitch(
			(void**)&cActiveVoxelNeighbors_pitched.ptr, 
			&cActiveVoxelNeighbors_pitched.pitch,
			cActiveVoxelNeighbors_pitched.xsize * sizeof(int), 
			cActiveVoxelNeighbors_pitched.ysize)	);

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset2D(
			cActiveVoxelNeighbors_pitched.ptr, 
			cActiveVoxelNeighbors_pitched.pitch,
			0, 
			cActiveVoxelNeighbors_pitched.xsize * sizeof(int), 
			cActiveVoxelNeighbors_pitched.ysize)	);

	// bind the input vin volume to the texture that represents the src. bin volume 
	t2dSrcBinVolume.addressMode[0] = cudaAddressModeClamp;
	t2dSrcBinVolume.addressMode[1] = cudaAddressModeClamp;
	t2dSrcBinVolume.filterMode =	cudaFilterModePoint;
	t2dSrcBinVolume.normalized =	false;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dSrcBinVolume, 
			cBinVolume_pitched.ptr, 
			cudaCreateChannelDesc<int>(),
			cVolumeExtent_array.width, 
			cVolumeExtent_array.height * cVolumeExtent_array.depth, 
			cBinVolume_pitched.pitch) );

	// bind the histogram as a texture
	t2dActiveVoxelSortedNeighbors.addressMode[0] = cudaAddressModeClamp;
	t2dActiveVoxelSortedNeighbors.addressMode[1] = cudaAddressModeClamp;
	t2dActiveVoxelSortedNeighbors.filterMode =	cudaFilterModePoint;
	t2dActiveVoxelSortedNeighbors.normalized =	false;

	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dActiveVoxelSortedNeighbors, 
			cActiveVoxelNeighbors_pitched.ptr, 
			cudaCreateChannelDesc<unsigned int>(),
			cActiveVoxelNeighbors_pitched.xsize,
			cActiveVoxelNeighbors_pitched.ysize, 
			cActiveVoxelNeighbors_pitched.pitch) );

	CUDPPHandle hScanPlan = 0;

	CUDPPConfiguration	cConfig;
	cConfig.op =		CUDPP_ADD;
	cConfig.datatype =	CUDPP_UINT;
	cConfig.algorithm =	CUDPP_SORT_RADIX;
	cConfig.options =	CUDPP_OPTION_KEYS_ONLY;

	assert( 
		CUDPP_SUCCESS  == cudppPlan(
			&hScanPlan,	
			cConfig, 
			cActiveVoxelNeighbors_pitched.pitch * cActiveVoxelNeighbors_pitched.ysize / sizeof(unsigned int),
			1, 
			0) );

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	for(int			y = 0; y < i3VolumeSize.y; y += v3Grid.y * v3Blk.y)
		for(int		x = 0; x < i3VolumeSize.x; x += v3Grid.x * v3Blk.x)
			for(int	z = 0; z < i3VolumeSize.z; z ++ )
			{
				CLOCK_INIT(_ComputeEntropyVolume_PRINT_LOOP_TIMING, __FUNCTION__ " (main loop): ");

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				int3 i3BlockCorner = make_int3(x, y, z);
				_CollectNeighbors_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					i3KernelSize,
					i3VolumeSize,
					iNrOfBins,
					cActiveVoxelNeighbors_pitched
				);
				CUT_CHECK_ERROR("_CollectNeighbors_kernel() failed");

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				// call cudpp to sort the buffer _CollectNeighborsToBeSorted_kernel
				cudppSort(
					hScanPlan,
					cActiveVoxelNeighbors_pitched.ptr,
					NULL,
					RADIX_SORT_BITS,
					cActiveVoxelNeighbors_pitched.pitch * cActiveVoxelNeighbors_pitched.ysize / sizeof(unsigned int)) ;
				CUT_CHECK_ERROR("cudppSort() failed");
CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// 
				_ComputeEntropyOnSortedNeighbors_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					(unsigned int *)cActiveVoxelNeighbors_pitched.ptr,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_ComputeEntropyOnSortedNeighbors_kernel() failed");
CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
			}


CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	cudppDestroyPlan(hScanPlan);
	FREE_MEMORY(cActiveVoxelNeighbors_pitched.ptr);

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

#endif	// #if COMPUTE_ENTROPY_VOLUME_WITH_SORTING_CUDA

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/12/27 19:03:06  leeten

[12/27/2009]
1. [1ST] First time checkin. Ths file define the host and kernel functions to compute the netropy field by sorting all the neighbors of all threads via CUDPP.


*/
