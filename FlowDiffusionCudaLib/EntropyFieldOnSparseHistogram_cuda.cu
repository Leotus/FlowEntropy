#if	COMPUTE_ENTROPY_VOLUME_ON_SPARSE_HISTOGRAM_CUDA

static texture<int, 2, cudaReadModeElementType> t2dActiveVoxelSparseHistorgrams;

__global__ 
static 
void 
_CreateSparseHistogram_kernel
(
	int3 i3BlockCorner,

	int3 i3KernelSize,

	int3 i3VolumeSize,

	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelsHistorgram_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;
//	int iNrOfSparseMatrixEntries = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z  )
	{
		int iSparseMatrixOffset_words = cActiveVoxelsHistorgram_pitched.pitch / sizeof(int);
		int *piBase_global = ADDRESS_2D(
			int,			cActiveVoxelsHistorgram_pitched.ptr, 
			sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
			iActiveVoxelId, 0);
		int iNrOfEntries = 0;

		for(int				iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
			for(int			iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
				for(int		iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++)
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

					int b;
					for(b = 0; b < iNrOfEntries; b++)
					{
						int *piEntry_global = 
							&piBase_global[(1 + 2 * b) * iSparseMatrixOffset_words];

						if( piEntry_global[0] == iSrcBin )
						{
							atomicAdd(
								&piEntry_global[iSparseMatrixOffset_words],
								1);
							break;
						}
					}
					if( b  == iNrOfEntries )
					{
						piBase_global[
							(1 + 2 * b) * iSparseMatrixOffset_words] 
							= iSrcBin;

						piBase_global[
							(1 + 2 * b + 1) * iSparseMatrixOffset_words] 
							= 1;

						iNrOfEntries++;
					}
				}
		piBase_global[0] = iNrOfEntries;
	}
}

__global__ 
void 
_ComputeEntropyOnSparseMatrix_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
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
	int iNrOfEntries = tex2D(t2dActiveVoxelSparseHistorgrams, iActiveVoxelId, 0);
	for(int e = 0; e < iNrOfEntries; e++)
	{
		float fCount = float(tex2D(t2dActiveVoxelSparseHistorgrams, iActiveVoxelId, 1 + 2 * e + 1));
		fEntropy += fCount * log2(fCount);
	}
	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	fEntropy = max(0.0, fEntropy);

	*ADDRESS_2D(
		float, cEntropyVolume_pitched.ptr, 
		sizeof(float), cEntropyVolume_pitched.pitch, 
		iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = fEntropy;
}

//////////////////////////////////////////////////////////////////////////////
void 
_ComputeEntropyVolumeOnSparseHistogram_cuda
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
	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	const unsigned int iMaxMemorySpace = MAX_MEMORY_SIZE;
	int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);
	int iSparseMatrixMemorySize_words = 1 + 2 * iNrOfNeighbors; // 1 as the counter; each entry incldues 2 values
	int iSparseMatrixMemorySize_bytes = iSparseMatrixMemorySize_words * sizeof(int);
	int iMemorySpacePerBlock_bytes = iSparseMatrixMemorySize_bytes * v3Blk.x * v3Blk.y;
	int iMaxNrOfBlocks = int(floorf(float(iMaxMemorySpace) / float(iMemorySpacePerBlock_bytes)));	// each elements take 2 values (entry, pair)
/*
	int iNrOfXBlocks = int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x)));
	int iNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	iMaxNrOfBlocks = min(iMaxNrOfBlocks, iNrOfXBlocks * iNrOfYBlocks);
	iNrOfXBlocks = min(iNrOfXBlocks, iMaxNrOfBlocks);
	iNrOfYBlocks = int(ceilf(float(iMaxNrOfBlocks) / float(iNrOfXBlocks)));
*/
	int iNrOfXBlocks = 
		min(
			iMaxNrOfBlocks, 
			int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x))));
	int iNrOfYBlocks = 
		min(
			max(1, iMaxNrOfBlocks / iNrOfXBlocks), 
			int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y))));
	dim3 v3Grid = dim3(iNrOfXBlocks, iNrOfYBlocks);

	fprintf(stderr, "MEM = %d MB; #BLOCKS = %d x %d\n", iMaxMemorySpace/(1<<20), v3Grid.x, v3Grid.y);

	cudaPitchedPtr cActiveVoxelHistorgrams_pitched;
	cActiveVoxelHistorgrams_pitched.xsize = v3Grid.x * v3Grid.y * v3Blk.x * v3Blk.y;
	cActiveVoxelHistorgrams_pitched.ysize = iSparseMatrixMemorySize_words;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocPitch(
			(void**)&cActiveVoxelHistorgrams_pitched.ptr, 
			&cActiveVoxelHistorgrams_pitched.pitch,
			cActiveVoxelHistorgrams_pitched.xsize * sizeof(int), 
			cActiveVoxelHistorgrams_pitched.ysize)	);

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
	t2dActiveVoxelSparseHistorgrams.addressMode[0] = cudaAddressModeClamp;
	t2dActiveVoxelSparseHistorgrams.addressMode[1] = cudaAddressModeClamp;
	t2dActiveVoxelSparseHistorgrams.filterMode =	cudaFilterModePoint;
	t2dActiveVoxelSparseHistorgrams.normalized =	false;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dActiveVoxelSparseHistorgrams, 
			cActiveVoxelHistorgrams_pitched.ptr, 
			cudaCreateChannelDesc<int>(),
			cActiveVoxelHistorgrams_pitched.xsize, 
			cActiveVoxelHistorgrams_pitched.ysize,
			cActiveVoxelHistorgrams_pitched.pitch) );

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	for(int			y = 0; y < i3VolumeSize.y; y += v3Grid.y * v3Blk.y)
		for(int		x = 0; x < i3VolumeSize.x; x += v3Grid.x * v3Blk.x)
			for(int z = 0; z < i3VolumeSize.z; z++)
			{
				CLOCK_INIT(_ComputeEntropyVolume_PRINT_LOOP_TIMING, __FUNCTION__ " (main loop): ");

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				int3 i3BlockCorner = make_int3(x, y, z);

				// reset the counters
				CUDA_SAFE_CALL_NO_SYNC(
					cudaMemset2D(
						cActiveVoxelHistorgrams_pitched.ptr, 
						cActiveVoxelHistorgrams_pitched.pitch,
						0, 
						cActiveVoxelHistorgrams_pitched.pitch, 
						1)	);

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				_CreateSparseHistogram_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					i3KernelSize,
					i3VolumeSize,
					iNrOfBins,
					cActiveVoxelHistorgrams_pitched
				);
				CUT_CHECK_ERROR("_CreateSparseHistogram_kernel() failed");

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				_ComputeEntropyOnSparseMatrix_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_ComputeEntropyOnSparseMatrix_kernel() failed");

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_PRINT(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
			}

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	FREE_MEMORY(cActiveVoxelHistorgrams_pitched.ptr);

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

#endif	// #if		COMPUTE_ENTROPY_VOLUME_ON_SPARSE_HISTOGRAM_CUDA

/*

$Log: not supported by cvs2svn $

*/
