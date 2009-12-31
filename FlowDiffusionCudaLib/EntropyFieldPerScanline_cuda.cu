#if	COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA

// ADD-BY-LEETEN 12/27/2009-BEGIN
__global__
void
_ComputeEntropyAlongScanlineInParallel_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	int iRoundKernelSizeXToHalfWarp,
	int3 i3Step,
	dim3 v3Grid,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched,
	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	__shared__ int	piBin_shared[MAX_THREADS];
	__shared__ int	piCounter_shared[MAX_THREADS];
	__shared__ int iMinBin_shared;
	__shared__ int iMaxBin_shared;
	__shared__ int iNextBin_shared;
	__shared__ int iMinInd_shared;

	// initialize the tables
	piBin_shared[threadIdx.x] = -1;
	piCounter_shared[threadIdx.x] = 0;
	__syncthreads();

	int iBlockIdxX = blockIdx.x;
	int iBlockIdxY = blockIdx.y % v3Grid.y;
	int iBlockIdxZ = blockIdx.y / v3Grid.y;
	int iActiveVoxelX =	threadIdx.x + iBlockIdxX * (blockDim.x - 2 * iRoundKernelSizeXToHalfWarp);
	int iActiveVoxelY = threadIdx.y + iBlockIdxY * blockDim.y;
	int iActiveVoxelZ = threadIdx.z + iBlockIdxZ * blockDim.z;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX - iRoundKernelSizeXToHalfWarp;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	int iActiveVoxelID = iBlockIdxX + iActiveVoxelY * v3Grid.x + iActiveVoxelZ * v3Grid.x * v3Grid.y;

	int iNrOfYZNeighbors = (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	float fEntropy = 0.0f;
	for(int r = 0; r < iNrOfYZNeighbors; r++)
	{
		unsigned int uBin = 
			tex2D(t2dActiveVoxelSortedNeighbors, 
				threadIdx.x, 
				iNrOfYZNeighbors * iActiveVoxelID + r);
		#if	0	// MOD-BY-LEETEN 12/28/2009-FROM:
			unsigned int uX = uBin % (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp);
			int iX = int(uX) - iRoundKernelSizeXToHalfWarp;
			int iBin = uBin / (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp);
		#else	// MOD-BY-LEETEN 12/28/2009-TO:
		unsigned int uX = uBin % blockDim.x;
		int iX = int(uX);
		int iBin = uBin / blockDim.x;
		#endif	// MOD-BY-LEETEN 12/28/2009-END

		if( 0 == threadIdx.x )
			iMinBin_shared = iBin;

		if( blockDim.x - 1 == threadIdx.x )
			iMaxBin_shared = iBin;

		__syncthreads();

		// iNextBin_shared = iMinBin_shared;

		for(int b = iMinBin_shared; b <= iMaxBin_shared; b = iNextBin_shared)
		{
			// pass-1: each thread updates the corresponding entropy
			int iPrevBin = piBin_shared[threadIdx.x];
			int iCounter = piCounter_shared[threadIdx.x];
			if( iCounter > 0 && b != iPrevBin  )
			{
				fEntropy += float(iCounter) * log2f(float(iCounter));
				piCounter_shared[threadIdx.x] = 0; // reset the counter
			}
			__syncthreads();

			// pass-2: each thread updates its neighbors
			if( b == iBin )
			{
				#if	0	// MOD-BY-LEETEN 12/28/2009-FROM:
					int iLeftT	= int(threadIdx.x) + ((iX - i3KernelSize.x) - iVoxelX);
					iLeftT = max(iRoundKernelSizeXToHalfWarp, iLeftT);
					int iRightT = int(threadIdx.x) + ((iX + i3KernelSize.x) - iVoxelX);
					iRightT = min(blockDim.x - iRoundKernelSizeXToHalfWarp, iRightT);
				#else	// MOD-BY-LEETEN 12/28/2009-TO:
				int iLeftT	= iX - i3KernelSize.x;
				iLeftT = max(iRoundKernelSizeXToHalfWarp, iLeftT);
				int iRightT = iX + i3KernelSize.x + 1;
				iRightT = min(blockDim.x - iRoundKernelSizeXToHalfWarp, iRightT);
				#endif	// MOD-BY-LEETEN 12/28/2009-END

				int iNrOfThreadsToBeUpdated = iRightT - iLeftT;
				for(int ti = 0; ti < iNrOfThreadsToBeUpdated; ti++)
				{
					// shuffer the index to reduce band conflict
					int t = iLeftT + (threadIdx.x + ti) % iNrOfThreadsToBeUpdated;

					atomicAdd(&piCounter_shared[t], 1);
					piBin_shared[t] = b;
				}
			}
			__syncthreads();

			// pass-3: update the next bin to search
			if( 0 == threadIdx.x )
				iMinInd_shared = blockDim.x;
			__syncthreads();

			if( iBin > b )
				atomicMin(&iMinInd_shared, threadIdx.x);
			__syncthreads();

			// if no new bin is found, exit the loop
			if( iMinInd_shared == blockDim.x )
				break;

			if( threadIdx.x == iMinInd_shared ) 
				iNextBin_shared = iBin;
			__syncthreads();
		}
	}

	int iCounter = piCounter_shared[threadIdx.x];
	if( iCounter > 0 )
		fEntropy += float(iCounter) * log2f(float(iCounter));

	int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);
	float fNrOfNeighbors = float(iNrOfNeighbors);

	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	fEntropy = max(fEntropy, 0.0f);

	if( iRoundKernelSizeXToHalfWarp <=	threadIdx.x && 
										threadIdx.x < blockDim.x - iRoundKernelSizeXToHalfWarp &&
		0 <= iVoxelX && iVoxelX < i3VolumeSize.x && 
		0 <= iVoxelY && iVoxelY < i3VolumeSize.y && 
		0 <= iVoxelZ && iVoxelZ < i3VolumeSize.z )
		*ADDRESS_2D(
			float, cEntropyVolume_pitched.ptr, 
			sizeof(float), cEntropyVolume_pitched.pitch, 
			iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = fEntropy;
}
// ADD-BY-LEETEN 12/27/2009-END

// ADD-BY-LEETEN 12/20/2009-BEGIN
__global__
void
_ComputeEntropyAlongScanline_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	int iRoundKernelSizeXToHalfWarp,
	int3 i3Step,
	// ADD-BY-LEETEN 12/20/2009-BEGIN
	dim3 v3Grid,
	// ADD-BY-LEETEN 12/20/2009-END
	cudaPitchedPtr cActiveVoxelNeighbors_pitched,
	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
		int iActiveVoxelX = threadIdx.x + blockIdx.x * blockDim.x;
		int iActiveVoxelID = blockIdx.x + blockIdx.y * gridDim.x;
		int iActiveVoxelYZ = blockIdx.y;
		int iActiveVoxelY = iActiveVoxelYZ % i3Step.y;
		int iActiveVoxelZ = iActiveVoxelYZ / i3Step.y;
 		int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
		int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
		int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	#else	// MOD-BY-LEETEN 12/20/2009-TO:
	int iBlockIdxX = blockIdx.x;
	int iBlockIdxY = blockIdx.y % v3Grid.y;
	int iBlockIdxZ = blockIdx.y / v3Grid.y;
	int iActiveVoxelX =	threadIdx.x + iBlockIdxX * blockDim.x;
	int iActiveVoxelY = threadIdx.y + iBlockIdxY * blockDim.y;
	int iActiveVoxelZ = threadIdx.z + iBlockIdxZ * blockDim.z;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	int iActiveVoxelID = iBlockIdxX + iActiveVoxelY * v3Grid.x + iActiveVoxelZ * v3Grid.x * v3Grid.y;
	#endif	// MOD-BY-LEETEN 12/20/2009-END

	int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);
	float fNrOfNeighbors = float(iNrOfNeighbors);

	int iNrOfYZNeighbors = (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	float fEntropy = 0.0f;
	int iPrevBin = -1;
	float fCount = 0.0f; 

	for(int		n = 0,	r = 0; r < iNrOfYZNeighbors;					r++)
	{
		for(int			x = 0; x < cActiveVoxelNeighbors_pitched.xsize; x++, n++)
		{

			unsigned int uBin = 
				tex2D(t2dActiveVoxelSortedNeighbors, 
					x, 
					iNrOfYZNeighbors * iActiveVoxelID + r);
			/*
			unsigned int uX = uBin / unsigned int (iNrOfBins);
			uBin = uBin % unsigned int (iNrOfBins);
			*/
			#if	0	// MOD-BY-LEETEN 12/28/2009-FROM:
				unsigned int uX = uBin % (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp);
				uBin = uBin / (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp);
				int iX = int(uX) - iRoundKernelSizeXToHalfWarp;
				if( iX > iVoxelX + i3KernelSize.x || iX < iVoxelX - i3KernelSize.x )
					continue;
			#else	// MOD-BY-LEETEN 12/28/2009-TO:
			unsigned int uX = uBin % (blockDim.x + 2 * iRoundKernelSizeXToHalfWarp);
			uBin = uBin / (blockDim.x + 2 * iRoundKernelSizeXToHalfWarp);
			int iX = int(uX) - iRoundKernelSizeXToHalfWarp;
			if( iX > int(threadIdx.x) + i3KernelSize.x || iX < int(threadIdx.x) - i3KernelSize.x )
				continue;
			#endif	// MOD-BY-LEETEN 12/28/2009-END


			if( iPrevBin < 0 || int(uBin) == iPrevBin  )
				fCount += 1.0f;
			else
			{
				if( fCount > 0.0f )
					fEntropy += fCount * log2(fCount);
				
				fCount = 1.0f;
			}
			iPrevBin = int(uBin);
		}
	}

	if( fCount > 0.0f )
		fEntropy += fCount * log2(fCount);

	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	fEntropy = max(fEntropy, 0.0f);

	// MOD-BY-LEETEN 12/20/2009-FROM:
		// if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z )
	// TO:
	if( 0 <= iVoxelX && iVoxelX < i3VolumeSize.x && 
		0 <= iVoxelY && iVoxelY < i3VolumeSize.y && 
		0 <= iVoxelZ && iVoxelZ < i3VolumeSize.z )
	// MOD-BY-LEETEN 12/20/2009-END
		*ADDRESS_2D(
			float, cEntropyVolume_pitched.ptr, 
			sizeof(float), cEntropyVolume_pitched.pitch, 
			iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = fEntropy;
}

__global__ 
void 
_CollectNeighborsAlongScanline_kernel
(
	int3 i3BlockCorner,
	int3 i3KernelSize,
	int iRoundKernelSizeXToHalfWarp,
	int3 i3VolumeSize,
	int3 i3Step,
	// ADD-BY-LEETEN 12/20/2009-BEGIN
	dim3 v3Grid,
	// ADD-BY-LEETEN 12/20/2009-END
	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched
)
{
	// NOTE!! here blockDim.x is the actuall #threads; 
	// but in order to get the idea #threads per block, 
	// the tails in the 2-sided should be substratected
	#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
		int iActiveVoxelX = threadIdx.x + blockIdx.x * (blockDim.x - 2 * iRoundKernelSizeXToHalfWarp);	
		int iActiveVoxelID = blockIdx.x + blockIdx.y * gridDim.x;
		int iActiveVoxelYZ = blockIdx.y;
		int iActiveVoxelY = iActiveVoxelYZ % i3Step.y;
		int iActiveVoxelZ = iActiveVoxelYZ / i3Step.y;
 		int iVoxelX = i3BlockCorner.x + iActiveVoxelX - iRoundKernelSizeXToHalfWarp;
		int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
		int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	#else	// MOD-BY-LEETEN 12/20/2009-TO:
	int iBlockIdxX = blockIdx.x;
	int iBlockIdxY = blockIdx.y % v3Grid.y;
	int iBlockIdxZ = blockIdx.y / v3Grid.y;
	int iActiveVoxelX =	threadIdx.x + iBlockIdxX * (blockDim.x - 2 * iRoundKernelSizeXToHalfWarp);
	int iActiveVoxelY = threadIdx.y + iBlockIdxY * blockDim.y;
	int iActiveVoxelZ = threadIdx.z + iBlockIdxZ * blockDim.z;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX - iRoundKernelSizeXToHalfWarp;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	int iActiveVoxelID = iBlockIdxX + iActiveVoxelY * v3Grid.x + iActiveVoxelZ * v3Grid.x * v3Grid.y;
	#endif	// MOD-BY-LEETEN 12/20/2009-END

	int iNrOfRows = (i3KernelSize.y * 2 + 1) * (i3KernelSize.z * 2 + 1);

	int iXOffset = 0;
	for(int	iOffset = 0,iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
		for(int			iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++, iOffset++)
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

			#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
				*ADDRESS_2D(
					unsigned int,			cActiveVoxelNeighbors_pitched.ptr, 
					sizeof(unsigned int),	cActiveVoxelNeighbors_pitched.pitch, 
					threadIdx.x, 
					iActiveVoxelID * iNrOfRows + iOffset) 
			#else	// MOD-BY-LEETEN 12/20/2009-TO:
			*ADDRESS_2D(
				unsigned int,			cActiveVoxelNeighbors_pitched.ptr, 
				sizeof(unsigned int),	cActiveVoxelNeighbors_pitched.pitch, 
				threadIdx.x, 
				iActiveVoxelID * iNrOfRows + iOffset) 
			#endif	// MOD-BY-LEETEN 12/20/2009-END
					// MOD-BY-LEETEN 12/28/2009-FROM:
						// = unsigned int(iSrcBin * (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp) + (iVoxelX + iRoundKernelSizeXToHalfWarp));
					// TO:
					= unsigned int(iSrcBin * blockDim.x  + threadIdx.x);
					// MOD-BY-LEETEN 12/28/2009-END
		}
}
// ADD-BY-LEETEN 12/20/2009-END

void 
_ComputeEntropyVolumePerScanline_cuda
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
	dim3 v3Blk, v3Grid;
	int3 i3Step;
	// DEL-BY-LEETEN 12/28/2009-BEGIN
		// int3 i3NrOfSteps;
	// DEL-BY-LEETEN 12/28/2009-END

	const int iNrOfRows = (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	int iRoundKernelSizeXToHalfWarp = NR_OF_THREADS_PER_HALF_WARP * int(ceilf( float(i3KernelSize.x) / float(NR_OF_THREADS_PER_HALF_WARP) ));

	#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
		int iNrOfNeededThreadsPerBlock = min(MAX_THREADS - 2 * iRoundKernelSizeXToHalfWarp, i3VolumeSize.x);
		int iNrOfActualThreadsPerBlock = iNrOfNeededThreadsPerBlock + 2 * iRoundKernelSizeXToHalfWarp;		// #threads (including the real ones and the tailing ones)
		v3Blk = dim3(iNrOfActualThreadsPerBlock, 1, 1);
		v3Grid = dim3(1, i3VolumeSize.y, 1);
		i3Step.x = iNrOfNeededThreadsPerBlock;
		i3Step.y = i3VolumeSize.y;
		i3Step.z = 1;

		fprintf(stderr, "MEM = %d MB; #BLOCKS = %d x %d\n", MAX_MEMORY_SIZE / (1<<20), v3Grid.x, v3Grid.y);
	#else	// MOD-BY-LEETEN 12/20/2009-TO:
	int iNrOfNeededThreadsPerBlock = min(MAX_THREADS - 2 * iRoundKernelSizeXToHalfWarp, i3VolumeSize.x);
	int iNrOfActualThreadsPerBlock = iNrOfNeededThreadsPerBlock + 2 * iRoundKernelSizeXToHalfWarp;		// #threads (including the real ones and the tailing ones)

	v3Blk = dim3(iNrOfActualThreadsPerBlock, 1, 1);

	int iTotalNrOfXBlocks = int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x)));
	int iTotalNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	int iTotalNrOfZBlocks = int(ceilf(float(i3VolumeSize.z) / float(v3Blk.z)));
	int iNrOfBlocksInMemory = MAX_MEMORY_SIZE / (sizeof(int) * iNrOfRows * iNrOfActualThreadsPerBlock);
	// ADD-BY-LEETEN 12/20/2009-BEGIN
	int iNrOfAllowableBlocks = int(pow(2.0, double(RADIX_SORT_BITS)) / (iNrOfBins * (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp)));
	iNrOfBlocksInMemory = min(iNrOfBlocksInMemory, iNrOfAllowableBlocks);
	// ADD-BY-LEETEN 12/20/2009-END
	int iNrOfXBlocksInMemory = min(iTotalNrOfXBlocks, iNrOfBlocksInMemory);
	int iNrOfYBlocksInMemory = min(iTotalNrOfYBlocks, max(1, iNrOfBlocksInMemory / iTotalNrOfXBlocks));
	#if	0	// MOD-BY-LEETEN 12/28/2009-FROM:
		int iNrOfZBlocksInMemory = min(
			MAX_Z_BLOCKS_IN_MEMORY, min(
				iTotalNrOfZBlocks, max(
					1, iNrOfBlocksInMemory / (iTotalNrOfXBlocks * iTotalNrOfYBlocks))));
	#else	// MOD-BY-LEETEN 12/28/2009-TO:
	int iMaxNrOfAllowableZBlocks = max(1, (1<<16) / (iNrOfRows * iNrOfXBlocksInMemory * iNrOfYBlocksInMemory));
	int iNrOfZBlocksInMemory = min(
		iMaxNrOfAllowableZBlocks, min(
			iTotalNrOfZBlocks, max(
				1, iNrOfBlocksInMemory / (iTotalNrOfXBlocks * iTotalNrOfYBlocks))));
	#endif	// MOD-BY-LEETEN 12/28/2009-END

	v3Grid = dim3(iNrOfXBlocksInMemory, iNrOfYBlocksInMemory, iNrOfZBlocksInMemory);
	fprintf(stderr, "#BLOCKS = %d x %d x %d\n", v3Grid.x, v3Grid.y, v3Grid.z);

	i3Step.x = iNrOfXBlocksInMemory * v3Blk.x;
	i3Step.y = iNrOfYBlocksInMemory * v3Blk.y;
	i3Step.z = iNrOfZBlocksInMemory * v3Blk.z;

	fprintf(stderr, "MEM = %d MB; #BLOCKS = %d x %d x %d\n", MAX_MEMORY_SIZE / (1<<20), v3Grid.x, v3Grid.y, v3Grid.z);
	#endif	// MOD-BY-LEETEN 12/20/2009-END
	cudaPitchedPtr cActiveVoxelNeighbors_pitched;
	cActiveVoxelNeighbors_pitched.xsize = v3Blk.x;
	// MOD-BY-LEETEN 12/20/2009-FROM:
		// cActiveVoxelNeighbors_pitched.ysize = iNrOfRows * v3Grid.x * v3Grid.y;
	// TO:
	cActiveVoxelNeighbors_pitched.ysize = iNrOfRows * v3Grid.x * v3Grid.y * v3Grid.z;
	// MOD-BY-LEETEN 12/20/2009-END
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocPitch(
			(void**)&cActiveVoxelNeighbors_pitched.ptr, 
			&cActiveVoxelNeighbors_pitched.pitch,
			cActiveVoxelNeighbors_pitched.xsize * sizeof(int), 
			cActiveVoxelNeighbors_pitched.ysize)	);

	// TEST-DEBUG
	fprintf(stderr, "%d = 2 ^ %f", cActiveVoxelNeighbors_pitched.ysize, log2f(float(cActiveVoxelNeighbors_pitched.ysize)));

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
			iNrOfRows * cActiveVoxelNeighbors_pitched.xsize,
			1, 
			0) );

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	for(int			z = 0; z < i3VolumeSize.z; z += i3Step.z)
		for(int		y = 0; y < i3VolumeSize.y; y += i3Step.y)
			for(int	x = 0; x < i3VolumeSize.x; x += i3Step.x)
			{
				CLOCK_INIT(_ComputeEntropyVolume_PRINT_LOOP_TIMING, __FUNCTION__ " (main loop): ");

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				int3 i3BlockCorner = make_int3(x, y, z);
				_CollectNeighborsAlongScanline_kernel<<<
					// MOD-BY-LEETEN 12/20/2009-FROM:
						// v3Grid, 
					// TO:
					dim3(v3Grid.x, v3Grid.y * v3Grid.z),
					// MOD-BY-LEETEN 12/20/2009-END
					v3Blk, 
					0>>>
				(
					i3BlockCorner,
					i3KernelSize,
					iRoundKernelSizeXToHalfWarp,
					i3VolumeSize,
					// ADD-BY-LEETEN 12/20/2009-BEGIN
					i3Step,
					v3Grid,
					// ADD-BY-LEETEN 12/20/2009-END
					iNrOfBins,
					cActiveVoxelNeighbors_pitched
				);
				CUT_CHECK_ERROR("_CollectNeighborsAlongScanline_kernel() failed");

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				// call cudpp to sort the buffer _CollectNeighborsToBeSorted_kernel
				// MOD-BY-LEETEN 12/20/2009-FROM:
					// for(int y2 = 0; y2 < int(v3Grid.y); y2++)
				// TO:
				for(int y2 = 0; y2 < int(v3Grid.x * v3Grid.y * v3Grid.z); y2++)
				// MOD-BY-LEETEN 12/20/2009-END
				{
					cudppSort(
						hScanPlan,
						((unsigned int*)cActiveVoxelNeighbors_pitched.ptr) + y2 * iNrOfRows * v3Blk.x,
						NULL,
						// MOD-BY-LEETEN 12/27/2009-FROM:
							// RADIX_SORT_BITS,
						// TO:
						// MOD-BY-LEETEN 12/28/2009-FROM:
							// int(ceilf(log2f(float(iNrOfBins * (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp))))),
						// TO:
						int(ceilf(log2f(float(iNrOfBins * v3Blk.x)))),
						// MOD-BY-LEETEN 12/28/2009-END
						// MOD-BY-LEETEN 12/27/2009-END
						iNrOfRows * v3Blk.x) ;
					CUT_CHECK_ERROR("cudppSort() failed");
				}

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// 
				// ADD-BY-LEETEN 12/27/2009-BEGIN
				#if	SCANNING_METHOD == SCANNING_METHOD_SCAN_WHOLE_HISTOGRAM
				// ADD-BY-LEETEN 12/27/2009-END

				_ComputeEntropyAlongScanline_kernel<<<
					// MOD-BY-LEETEN 12/20/2009-FROM:
						// v3Grid, 
					// TO:
					dim3(v3Grid.x, v3Grid.y * v3Grid.z),
					// MOD-BY-LEETEN 12/20/2009-END
					dim3(iNrOfNeededThreadsPerBlock, 1, 1),
					0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					iRoundKernelSizeXToHalfWarp,
					i3Step,
					// ADD-BY-LEETEN 12/20/2009-BEGIN
					v3Grid,
					// ADD-BY-LEETEN 12/20/2009-END
					cActiveVoxelNeighbors_pitched,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_ComputeEntropyOnSortedNeighbors_kernel() failed");

				// ADD-BY-LEETEN 12/27/2009-BEGIN
				#endif	// #if	SCANNING_METHOD == SCANNING_METHOD_SCAN_WHOLE_HISTOGRAM

				#if	SCANNING_METHOD == SCANNING_METHOD_SCAN_ROWS_IN_PARALLEL

				_ComputeEntropyAlongScanlineInParallel_kernel<<<
					dim3(v3Grid.x, v3Grid.y * v3Grid.z),
					dim3(iNrOfActualThreadsPerBlock, 1, 1)
					>>>
 				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					iRoundKernelSizeXToHalfWarp,
					i3Step,
					v3Grid,
					cActiveVoxelNeighbors_pitched,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_ComputeEntropyAlongScanlineInParallel_kernel() failed");
				#endif	// #if	SCANNING_METHOD == SCANNING_METHOD_SCAN_ROWS_IN_PARALLEL

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
	
#endif	// #if	COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/12/27 19:35:53  leeten

[12/27/2009]
1. [1ST] First time checkin. Ths file define the host and kernel functions to compute the netropy field by sorting all the YZ neighbors of all threads along the X scanline via CUDPP.


*/
