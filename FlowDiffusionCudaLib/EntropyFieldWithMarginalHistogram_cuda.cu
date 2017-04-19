#if	COMPUTE_ENTROPY_VOLUME_WITH_MARGINAL_HISTOGRAM	

__global__ 
static 
void 
_UpdateHistogramWithMarginalHistogram_kernel
(
	int3 i3BlockCorner,

	int3 i3KernelSize,

	int3 i3VolumeSize,

	int iNrOfBins,
	int iNrOfMarginalBins,

	cudaPitchedPtr cActiveVoxelsHistorgram_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z  )

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
					int iX, iY, iZ;
					iX = iVoxelX + iXOffset;
					iY = iVoxelY + iYOffset;
					iZ = iVoxelZ + iZOffset;

					int3 i3TexCoord;
					i3TexCoord.x = IMirrorCoord(iX, i3VolumeSize.x);
					i3TexCoord.y = IMirrorCoord(iY, i3VolumeSize.y);
					i3TexCoord.z = IMirrorCoord(iZ, i3VolumeSize.z);

					int iSrcBin = tex2D(t2dSrcBinVolume, i3TexCoord.x, i3TexCoord.y + i3TexCoord.z * i3VolumeSize.y);
					int iMarginalBin = iSrcBin / iNrOfMarginalBins;

										// update the joint histogram
					atomicAdd(
						ADDRESS_2D(
							int,			cActiveVoxelsHistorgram_pitched.ptr, 
							sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
							iActiveVoxelId, iSrcBin + iNrOfMarginalBins),
						iZDir);

					atomicAdd(
						ADDRESS_2D(
							int,			cActiveVoxelsHistorgram_pitched.ptr, 
							sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
							iActiveVoxelId, iMarginalBin),
						iZDir);
				}
		}
}

__global__
void
_CreateHistogramWithMarginalHistogram_kernel
(
	int3 i3BlockCorner,

	int3 i3KernelSize,

	int3 i3VolumeSize,

	int iNrOfBins,
	int iNrOfMarginalBins,
	cudaPitchedPtr cActiveVoxelsHistorgram_pitched
)
{
	int iActiveVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z  )
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
					int iMarginalBin = iSrcBin / iNrOfMarginalBins;

										// update the joint histogram
					atomicAdd(
						ADDRESS_2D(
							int,			cActiveVoxelsHistorgram_pitched.ptr, 
							sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
							iActiveVoxelId, iSrcBin + iNrOfMarginalBins),
						+1.0);

					atomicAdd(
						ADDRESS_2D(
							int,			cActiveVoxelsHistorgram_pitched.ptr, 
							sizeof(int),	cActiveVoxelsHistorgram_pitched.pitch, 
							iActiveVoxelId, iMarginalBin),
						+1.0);
				}
}

__global__ 
void 
_ComputeEntropyWithMarginalHistogram_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int iNrOfMarginalBins,
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

	int iNrOfMargins = iNrOfBins / iNrOfMarginalBins;
	float fEntropy = 0.0f;
	for(int b0 = 0; b0 < iNrOfMarginalBins; b0++)
	{
		int iCount = tex2D(t2dActiveVoxelHistorgrams, iActiveVoxelId, b0);
		if( 0 == iCount )
			continue;

		for(int		b1 = 0; b1 < iNrOfMargins;		b1++)
		{
			int iCount = tex2D(t2dActiveVoxelHistorgrams, iActiveVoxelId, iNrOfMarginalBins + b0 * iNrOfMargins + b1);
			if( iCount )
			{
				float fCount = float(iCount);
				fEntropy += fCount * log2(fCount);
			}
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

void 
_ComputeEntropyVolumeWithMarginalHistogram_cuda
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
	int iNrOfMarginalBins = int(sqrt(double(iNrOfBins)));

	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	const unsigned int iMaxMemorySpace = MAX_MEMORY_SIZE;
	int iMaxNrOfBlocks = int(floorf(float(iMaxMemorySpace) / float(sizeof(int) * (iNrOfMarginalBins + iNrOfBins) * v3Blk.x * v3Blk.y)));
	int iNrOfXBlocks = int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x)));
	int iNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	iMaxNrOfBlocks = min(iMaxNrOfBlocks, iNrOfXBlocks * iNrOfYBlocks);
	iNrOfXBlocks = min(iNrOfXBlocks, iMaxNrOfBlocks);
	iNrOfYBlocks = int(ceilf(float(iMaxNrOfBlocks) / float(iNrOfXBlocks)));
	dim3 v3Grid = dim3(iNrOfXBlocks, iNrOfYBlocks);
	fprintf(stderr, "MEM = %d MB; #BLOCKS = %d x %d\n", iMaxMemorySpace/(1<<20), v3Grid.x, v3Grid.y);

	cudaPitchedPtr cActiveVoxelHistorgrams_pitched;
	cActiveVoxelHistorgrams_pitched.xsize = v3Grid.x * v3Grid.y * v3Blk.x * v3Blk.y;
	cActiveVoxelHistorgrams_pitched.ysize = iNrOfMarginalBins + iNrOfBins;
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

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	for(int			y = 0; y < i3VolumeSize.y; y += v3Grid.y * v3Blk.y)
		for(int		x = 0; x < i3VolumeSize.x; x += v3Grid.x * v3Blk.x)
			for(int z = 0; z < i3VolumeSize.z; z++)
			{
				CLOCK_INIT(_ComputeEntropyVolume_PRINT_LOOP_TIMING, __FUNCTION__ " (main loop): ");

				int3 i3BlockCorner = make_int3(x, y, z);

				/*
				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				CUDA_SAFE_CALL_NO_SYNC(
					cudaMemset2D(
						cActiveVoxelHistorgrams_pitched.ptr, 
						cActiveVoxelHistorgrams_pitched.pitch,
						0, 
						cActiveVoxelHistorgrams_pitched.xsize * sizeof(int), 
						cActiveVoxelHistorgrams_pitched.ysize)	);

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);
				*/

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				if( 0 == z )
				{
					_CreateHistogramWithMarginalHistogram_kernel<<<v3Grid, v3Blk, 0>>>
					(
						i3BlockCorner,
						i3KernelSize,
						i3VolumeSize,
						iNrOfBins,
						iNrOfMarginalBins,
						cActiveVoxelHistorgrams_pitched
					);
					CUT_CHECK_ERROR("_CreateHistogramWithMarginalHistogram_kernel() failed");
				}
				else
				{
					_UpdateHistogramWithMarginalHistogram_kernel<<<v3Grid, v3Blk, 0>>>
					(
						i3BlockCorner,
						i3KernelSize,
						i3VolumeSize,
						iNrOfBins,
						iNrOfMarginalBins,
						cActiveVoxelHistorgrams_pitched
					);
					CUT_CHECK_ERROR("_UpdateHistogramWithMarginalHistogram_kernel() failed");
				}

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				_ComputeEntropyWithMarginalHistogram_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					iNrOfMarginalBins,
					i3KernelSize,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				CUT_CHECK_ERROR("_ComputeEntropy_kernel() failed");

				CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

				CLOCK_PRINT(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

			}

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	FREE_MEMORY(cActiveVoxelHistorgrams_pitched.ptr);

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

#endif	// COMPUTE_ENTROPY_VOLUME_WITH_MARGINAL_HISTOGRAM

/*

$Log: not supported by cvs2svn $

*/
