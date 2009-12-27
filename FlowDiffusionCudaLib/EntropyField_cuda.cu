#if	COMPUTE_ENTROPY_VOLUME_CUDA

// ADD-BY-LEETEN 12/23/2009-BEGIN
#define NR_OF_THREADS_PER_MARGIN	1

static texture<float, 2, cudaReadModeElementType> t2dActiveVoxelMarginalEntropy;
// ADD-BY-LEETEN 12/23/2009-END

__device__
void
_UpdateHistogramEntry_device
(
	int iActiveVoxelId,

	int iVoxelX, 
	int iVoxelY, 
	int iVoxelZ,
	int iXOffset,
	int iYOffset,
	int iZOffset,

	int iValue,

	int iNrOfBins,

	int3 i3VolumeSize,
	cudaPitchedPtr cActiveVoxelsHistorgram_pitched
)
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

						// update the joint histogram
	if( iSrcBin  < iNrOfBins )
		atomicAdd(
			ADDRESS_2D(
				int,			cActiveVoxelsHistorgram_pitched.ptr, 
				sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
				iActiveVoxelId, iSrcBin),
			iValue );
}


__global__ 
static 
void 
_UpdateHistogram_kernel
(
	int3 i3BlockCorner,

	int3 i3KernelSize,

	int3 i3VolumeSize,

	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelsHistorgram_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	#if	0		// MOD-BY-LEETEN 12/19/2009-FROM:
		int iActiveVoxelZ = blockIdx.y * blockDim.y + threadIdx.y;
		int iActiveVoxelId = iActiveVoxelX + iActiveVoxelZ * gridDim.x * blockDim.x;
 		int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
		int iVoxelY = i3BlockCorner.y;
		int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	#else		// MOD-BY-LEETEN 12/19/2009-TO:

	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	#endif		// MOD-BY-LEETEN 12/19/2009-END


	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z  )

		#if	0		// MOD-BY-LEETEN 12/19/2009-FROM:

			for(int iYDir = -1; iYDir <= +1; iYDir+=2)
			{
				int iYOffset = i3KernelSize.y * iYDir;
				switch (iYDir)
				{
				case -1:	iYOffset--;		break;
				}
				for(int		iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
					for(int iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++)
					{
						_UpdateHistogramEntry_device
						(
							iActiveVoxelId,	
							iVoxelX, iVoxelY, iVoxelZ,
							iXOffset,iYOffset,iZOffset,
							iYDir,
							iNrOfBins,
							i3VolumeSize, 
							cActiveVoxelsHistorgram_pitched
						);
					}
			}

		#else

		for(int iZDir = -1; iZDir <= +1; iZDir+=2)
		{
			int iZOffset = i3KernelSize.z * iZDir;
			switch (iZDir)
			{
			case -1:	iZOffset--;		break;
			}
			for(int		iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
				for(int iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++)
				{
					_UpdateHistogramEntry_device
					(
						iActiveVoxelId,	
						iVoxelX, iVoxelY, iVoxelZ,
						iXOffset,iYOffset,iZOffset,
						iZDir,
						iNrOfBins,
						i3VolumeSize, 
						cActiveVoxelsHistorgram_pitched
					);
				}
		}
		#endif
}

__global__ 
static 
void 
_CreateHistogram_kernel
(
	int3 i3BlockCorner,

	int3 i3KernelSize,

	int3 i3VolumeSize,

	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelsHistorgram_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	#if	0		// MOD-BY-LEETEN 12/19/2009-FROM:
		int iActiveVoxelZ = blockIdx.y * blockDim.y + threadIdx.y;
		int iActiveVoxelId = iActiveVoxelX + iActiveVoxelZ * gridDim.x * blockDim.x;
 		int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
		int iVoxelY = i3BlockCorner.y;
		int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	#else		// MOD-BY-LEETEN 12/19/2009-TO:

	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	#endif		// MOD-BY-LEETEN 12/19/2009-END

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z  )
		for(int				iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
			for(int			iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
				for(int		iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++)
				{
					_UpdateHistogramEntry_device
					(
						iActiveVoxelId,	
						iVoxelX, iVoxelY, iVoxelZ,
						iXOffset,iYOffset,iZOffset,
						+1,
						iNrOfBins,
						i3VolumeSize, 
						cActiveVoxelsHistorgram_pitched
					);
				}
}

__global__ 
void 
_ComputeEntropy_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
		int iActiveVoxelZ = blockIdx.y * blockDim.y + threadIdx.y;
		int iActiveVoxelId = iActiveVoxelX + iActiveVoxelZ * gridDim.x * blockDim.x;
 		int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
		int iVoxelY = i3BlockCorner.y;
		int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	#else	// MOD-BY-LEETEN 12/19/2009-TO:

	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	#endif	// MOD-BY-LEETEN 12/19/2009-END

	int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);
	float fNrOfNeighbors = float(iNrOfNeighbors);

	float fEntropy = 0.0f;
	for(int b = 0; b < iNrOfBins; b++)
	{
		int iCount = tex2D(t2dActiveVoxelHistorgrams, iActiveVoxelId, b);
		if( iCount )
		{
			/*
			float fProb = float(iCount) / fNrOfNeighbors;
			fEntropy += fProb * log2(fProb);
			*/
			float fCount = float(iCount);
			fEntropy += fCount * log2(fCount);
		}
	}
	// fEntropy *= -1.0;
	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	fEntropy = max(0.0, fEntropy);

	*ADDRESS_2D(
		float, cEntropyVolume_pitched.ptr, 
		sizeof(float), cEntropyVolume_pitched.pitch, 
		iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = fEntropy;
}

// ADD-BY-LEETEN 12/23/2009-BEGIN
__global__ 
void 
_ComputeEntropyHierarchically_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	int iGridDimX, 
	int iGridDimY, 
	int iLevel,
	cudaPitchedPtr cActiveVoxelsHistorgram_pitched,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	int iBlockIdxX = blockIdx.x % iGridDimX;
	int iBlockIdxY = blockIdx.x / iGridDimX;
	int iActiveVoxelX = iBlockIdxX * blockDim.x + threadIdx.x;
	int iActiveVoxelY = iBlockIdxY * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * iGridDimX * blockDim.x;
	int iToBeUpdatedBin = blockIdx.y * iLevel;

	int iCount =		( iToBeUpdatedBin < iNrOfBins )?tex2D(t2dActiveVoxelHistorgrams, iActiveVoxelId, iToBeUpdatedBin):0;
	int iNextCount =	( iToBeUpdatedBin + iLevel < iNrOfBins )?tex2D(t2dActiveVoxelHistorgrams, iActiveVoxelId, iToBeUpdatedBin + iLevel):0;
	int iSum = iCount + iNextCount;

	*ADDRESS_2D(
		int,			cActiveVoxelsHistorgram_pitched.ptr, 
		sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
		iActiveVoxelId, iToBeUpdatedBin) = iSum;
}

__global__ 
void 
_ComputeMarginalEntropy_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	int iGridDimX, 
	int iGridDimY, 
	int iNrOfMarginalBins,
	int iNrOfThreadsPerMagin,
	cudaPitchedPtr cActiveVoxelMarginalEntropy_pitched
)
{
	int iBlockIdxX = blockIdx.x % iGridDimX;
	int iBlockIdxY = blockIdx.x / iGridDimX;
	int iActiveVoxelX = iBlockIdxX * blockDim.x + threadIdx.x;
	int iActiveVoxelY = iBlockIdxY * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * iGridDimX * blockDim.x;
	int iMargin =			blockIdx.y / iNrOfThreadsPerMagin;
	int iThreadIdInMargin = blockIdx.y % iNrOfThreadsPerMagin;

	int iBase = iMargin * iNrOfMarginalBins + iThreadIdInMargin * iNrOfMarginalBins / iNrOfThreadsPerMagin;
	float fEntropy = 0.0f;
	for(int b = 0; b < iNrOfMarginalBins / iNrOfThreadsPerMagin; b++)
	{
		int iCount = tex2D(t2dActiveVoxelHistorgrams, iActiveVoxelId, iBase + b);
		if( iCount )
		{
			float fCount = float(iCount);
			fEntropy += fCount * log2(fCount);
		}
	}
	*ADDRESS_2D(
		float,			cActiveVoxelMarginalEntropy_pitched.ptr, 
		sizeof(float),	cActiveVoxelMarginalEntropy_pitched.pitch, 
		iActiveVoxelId, blockIdx.y) = fEntropy;
}

__global__ 
void 
_SumMarginalEntropy_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	int iNrOfMargins,
	int iNrOfThreadsPerMagin,
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
	for(int b = 0; b < iNrOfMargins * iNrOfThreadsPerMagin; b++)
	{
		float fMarginalEntropy = tex2D(t2dActiveVoxelMarginalEntropy, iActiveVoxelId, b);
		fEntropy += fMarginalEntropy;
	}

	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	fEntropy = max(0.0, fEntropy);

	*ADDRESS_2D(
		float, cEntropyVolume_pitched.ptr, 
		sizeof(float), cEntropyVolume_pitched.pitch, 
		iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = fEntropy;
}
// ADD-BY-LEETEN 12/23/2009-END

//////////////////////////////////////////////////////////////////////////////
void 
_ComputeEntropyVolume_cuda
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
	int iMaxNrOfBlocks = int(floorf(float(iMaxMemorySpace) / float(sizeof(int) * iNrOfBins * v3Blk.x * v3Blk.y)));
	int iNrOfXBlocks = int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x)));
	#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
		int iNrOfZBlocks = int(ceilf(float(i3VolumeSize.z) / float(v3Blk.y)));
		iMaxNrOfBlocks = min(iMaxNrOfBlocks, iNrOfXBlocks * iNrOfZBlocks);
		iNrOfXBlocks = min(iNrOfXBlocks, iMaxNrOfBlocks);
		iNrOfZBlocks = int(ceilf(float(iMaxNrOfBlocks) / float(iNrOfXBlocks)));
		dim3 v3Grid = dim3(iNrOfXBlocks, iNrOfZBlocks);
	#else	// MOD-BY-LEETEN 12/19/2009-TO:
	int iNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	iMaxNrOfBlocks = min(iMaxNrOfBlocks, iNrOfXBlocks * iNrOfYBlocks);
	iNrOfXBlocks = min(iNrOfXBlocks, iMaxNrOfBlocks);
	iNrOfYBlocks = int(ceilf(float(iMaxNrOfBlocks) / float(iNrOfXBlocks)));
	dim3 v3Grid = dim3(iNrOfXBlocks, iNrOfYBlocks);
	#endif	// MOD-BY-LEETEN 12/19/2009-END
	fprintf(stderr, "MEM = %d MB; #BLOCKS = %d x %d\n", iMaxMemorySpace/(1<<20), v3Grid.x, v3Grid.y);

	cudaPitchedPtr cActiveVoxelHistorgrams_pitched;
	cActiveVoxelHistorgrams_pitched.xsize = v3Grid.x * v3Grid.y * v3Blk.x * v3Blk.y;
	cActiveVoxelHistorgrams_pitched.ysize = iNrOfBins;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocPitch(
			(void**)&cActiveVoxelHistorgrams_pitched.ptr, 
			&cActiveVoxelHistorgrams_pitched.pitch,
			cActiveVoxelHistorgrams_pitched.xsize * sizeof(int), 
			cActiveVoxelHistorgrams_pitched.ysize)	);

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset2D(
			cActiveVoxelHistorgrams_pitched.ptr, 
			cActiveVoxelHistorgrams_pitched.pitch,
			0, 
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
	t2dActiveVoxelHistorgrams.addressMode[0] = cudaAddressModeClamp;
	t2dActiveVoxelHistorgrams.addressMode[1] = cudaAddressModeClamp;
	t2dActiveVoxelHistorgrams.filterMode =	cudaFilterModePoint;
	t2dActiveVoxelHistorgrams.normalized =	false;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dActiveVoxelHistorgrams, 
			cActiveVoxelHistorgrams_pitched.ptr, 
			cudaCreateChannelDesc<int>(),
			cActiveVoxelHistorgrams_pitched.xsize, 
			cActiveVoxelHistorgrams_pitched.ysize,
			cActiveVoxelHistorgrams_pitched.pitch) );

	// ADD-BY-LEETEN 12/23/2009-BEGIN
	int iNrOfMarginalBins = int(sqrtf(float(iNrOfBins)));
	int iNrOfMargins = int(ceilf(float(iNrOfBins)/float(iNrOfMarginalBins)));

	cudaPitchedPtr cActiveVoxelMarginalEntropy_pitched;
	cActiveVoxelMarginalEntropy_pitched.xsize = v3Grid.x * v3Grid.y * v3Blk.x * v3Blk.y;
	cActiveVoxelMarginalEntropy_pitched.ysize = iNrOfMargins * NR_OF_THREADS_PER_MARGIN;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocPitch(
			(void**)&cActiveVoxelMarginalEntropy_pitched.ptr, 
			&cActiveVoxelMarginalEntropy_pitched.pitch,
			cActiveVoxelMarginalEntropy_pitched.xsize * sizeof(float), 
			cActiveVoxelMarginalEntropy_pitched.ysize)	);

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemset2D(
			cActiveVoxelMarginalEntropy_pitched.ptr, 
			cActiveVoxelMarginalEntropy_pitched.pitch,
			0, 
			cActiveVoxelMarginalEntropy_pitched.pitch, 
			cActiveVoxelMarginalEntropy_pitched.ysize)	);

	t2dActiveVoxelMarginalEntropy.addressMode[0] = cudaAddressModeClamp;
	t2dActiveVoxelMarginalEntropy.addressMode[1] = cudaAddressModeClamp;
	t2dActiveVoxelMarginalEntropy.filterMode =	cudaFilterModePoint;
	t2dActiveVoxelMarginalEntropy.normalized =	false;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dActiveVoxelMarginalEntropy, 
			cActiveVoxelMarginalEntropy_pitched.ptr, 
			cudaCreateChannelDesc<float>(),
			cActiveVoxelMarginalEntropy_pitched.xsize, 
			cActiveVoxelMarginalEntropy_pitched.ysize,
			cActiveVoxelMarginalEntropy_pitched.pitch) );
	// ADD-BY-LEETEN 12/23/2009-END

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
		for(int			z = 0; z < i3VolumeSize.z; z += v3Grid.y * v3Blk.y)
			for(int		x = 0; x < i3VolumeSize.x; x += v3Grid.x * v3Blk.x)
				for(int y = 0; y < i3VolumeSize.y; y++)
	#else	// MOD-BY-LEETEN 12/19/2009-TO:
	for(int			y = 0; y < i3VolumeSize.y; y += v3Grid.y * v3Blk.y)
		for(int		x = 0; x < i3VolumeSize.x; x += v3Grid.x * v3Blk.x)
			for(int z = 0; z < i3VolumeSize.z; z++)
	#endif	// MOD-BY-LEETEN 12/19/2009-END
			{
				int3 i3BlockCorner = make_int3(x, y, z);

				// ADD-BY-LEETEN 12/23/2009-BEGIN
				CLOCK_INIT(_ComputeEntropyVolume_PRINT_LOOP_TIMING, __FUNCTION__ " (main loop): ");

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// ADD-BY-LEETEN 12/23/2009-END

				// MOD-BY-LEETEN 12/19/2009-FROM:
					// if( 0 == y )
				// TO:
				if( 0 == z )
				// MOD-BY-LEETEN 12/19/2009-END
				{
					_CreateHistogram_kernel<<<v3Grid, v3Blk, 0>>>
					(
						i3BlockCorner,
						i3KernelSize,
						i3VolumeSize,
						iNrOfBins,
						cActiveVoxelHistorgrams_pitched
					);
					CUT_CHECK_ERROR("_CreateHistogram_kernel() failed");
				}
				else
				{
					_UpdateHistogram_kernel<<<v3Grid, v3Blk, 0>>>
					(
						i3BlockCorner,
						i3KernelSize,
						i3VolumeSize,
						iNrOfBins,
						cActiveVoxelHistorgrams_pitched
					);
					CUT_CHECK_ERROR("_UpdateHistogram_kernel() failed");
				}

				// ADD-BY-LEETEN 12/23/2009-BEGIN
				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// ADD-BY-LEETEN 12/23/2009-END

				#if		SCANNING_METHOD	== SCANNING_METHOD_SCAN_WHOLE_HISTOGRAM					// ADD-BY-LEETEN 12/23/2009
				_ComputeEntropy_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_ComputeEntropy_kernel() failed");
				#endif	// #if		SCANNING_METHOD	== SCANNING_METHOD_SCAN_WHOLE_HISTOGRAM		// ADD-BY-LEETEN 12/23/2009

				// ADD-BY-LEETEN 12/23/2009-BEGIN
				#if		SCANNING_METHOD	== SCANNING_METHOD_HIERARCHICAL_SCAN
				int iNrOfLevels = int(floorf(log2f(float(iNrOfBins))));
				int iNrOfThreadsPerVoxel = iNrOfBins/2;
				for(int l = 1; l < iNrOfLevels; l++)
				{
					_ComputeEntropyHierarchically_kernel<<<
						dim3(v3Grid.x * v3Grid.y, iNrOfThreadsPerVoxel),
						v3Blk, 0
						>>>
					(
						i3BlockCorner,
						iNrOfBins,
						i3KernelSize,
						i3VolumeSize,
						v3Grid.x,
						v3Grid.y,
						l,
						cActiveVoxelHistorgrams_pitched,
						cEntropyVolume_pitched
					);

					iNrOfThreadsPerVoxel /= 2;
				}
				CUT_CHECK_ERROR("_ComputeEntropyHierarchically_kernel() failed");
				#endif	// #if	SCANNING_METHOD	== SCANNING_METHOD_HIERARCHICAL_SCAN

				#if	SCANNING_METHOD	== SCANNING_METHOD_SCAN_ROWS_IN_PARALLEL
				_ComputeMarginalEntropy_kernel<<<
					dim3(v3Grid.x * v3Grid.y, NR_OF_THREADS_PER_MARGIN * iNrOfMargins), v3Blk, 0
					>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					i3VolumeSize,
					v3Grid.x,
					v3Grid.y,
					iNrOfMarginalBins,
					NR_OF_THREADS_PER_MARGIN,
					cActiveVoxelMarginalEntropy_pitched
				);
				CUT_CHECK_ERROR("_ComputeMarginalEntropy_kernel() failed");

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				_SumMarginalEntropy_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					i3VolumeSize,
					iNrOfMargins,
					NR_OF_THREADS_PER_MARGIN,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_SumMarginalEntropy_kernel() failed");
				#endif	// #if	SCANNING_METHOD	== SCANNING_METHOD_SCAN_ROWS_IN_PARALLEL
				// ADD-BY-LEETEN 12/23/2009-END

				// ADD-BY-LEETEN 12/23/2009-BEGIN
				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_PRINT(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// ADD-BY-LEETEN 12/23/2009-END
			}

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	FREE_MEMORY(cActiveVoxelHistorgrams_pitched.ptr);

	// ADD-BY-LEETEN 12/23/2009-BEGIN
	FREE_MEMORY(cActiveVoxelMarginalEntropy_pitched.ptr);
	// ADD-BY-LEETEN 12/23/2009-END

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}
// ADD-BY-LEETEN 12/18/2009-END

#endif	// #if	COMPUTE_ENTROPY_VOLUME_CUDA

/*

$Log: not supported by cvs2svn $

*/
