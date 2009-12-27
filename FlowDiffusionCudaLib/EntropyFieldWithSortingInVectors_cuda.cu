#if COMPUTE_ENTROPY_VOLUME_WITH_SORTING_IN_VECTORS_CUDA

static texture<int4, 2, cudaReadModeElementType> t2dActiveVoxelSortedNeighborsInVectors;

__device__
void
_SwapData(int tid, int ixj, int t, int k, int iNrOfBins, int column, cudaPitchedPtr cActiveVoxelNeighbors_pitched)
{
	int4 i4Tid, i4Ixj;
	i4Tid = tex2D(t2dActiveVoxelSortedNeighborsInVectors, column, tid);
	i4Ixj = tex2D(t2dActiveVoxelSortedNeighborsInVectors, column, ixj);

	int4 i4Max = make_int4(
		max(i4Tid.x, i4Ixj.x),
		max(i4Tid.y, i4Ixj.y),
		max(i4Tid.z, i4Ixj.z),
		max(i4Tid.w, i4Ixj.w)
		);
	int4 i4Min = make_int4(
		min(i4Tid.x, i4Ixj.x),
		min(i4Tid.y, i4Ixj.y),
		min(i4Tid.z, i4Ixj.z),
		min(i4Tid.w, i4Ixj.w)
		);
	if ((t & k) == 0)
    {
		/*
        if (shared[tid] < shared[ixj])
        {
            swap(shared[tid], shared[ixj]);
        }
		*/
		i4Tid = i4Max;
		i4Ixj = i4Min;
    }
    else
    {
		/*
        if (shared[tid] > shared[ixj])
        {
            swap(shared[tid], shared[ixj]);
        }
		*/
		i4Tid = i4Min;
		i4Ixj = i4Max;
    }

	if( column < cActiveVoxelNeighbors_pitched.pitch/sizeof(int4) )
	{
		if(tid < cActiveVoxelNeighbors_pitched.ysize)
			*ADDRESS_2D(
				int4, cActiveVoxelNeighbors_pitched.ptr, 
				sizeof(int4), cActiveVoxelNeighbors_pitched.pitch, 
				column, tid) = i4Tid;
		if(ixj < cActiveVoxelNeighbors_pitched.ysize)
			*ADDRESS_2D(
				int4, cActiveVoxelNeighbors_pitched.ptr, 
				sizeof(int4), cActiveVoxelNeighbors_pitched.pitch, 
				column, ixj) = i4Ixj;
	}
}

__device__ 
void
_BitonicSwap(int j, int k, int tid, int iNrOfBins, int column, cudaPitchedPtr cActiveVoxelNeighbors_pitched)
{
    int ixj = tid ^ j;

    if (ixj > tid)
		_SwapData(tid, ixj, tid, k, iNrOfBins, column, cActiveVoxelNeighbors_pitched);
}

__global__
void
_BitonicMergeInVectors_kernel
(
	int k, 
	int j,
	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched
)
{
	int iThreadId = threadIdx.x + blockIdx.x * blockDim.x;

	// size_t tid = blockIdx.y;
	size_t tid = blockIdx.y;
    size_t ixj = tid ^ j;

	if (ixj <= tid)
    {
		tid = ixj + cActiveVoxelNeighbors_pitched.ysize / 2;
		ixj = tid ^ j;
	}

	_BitonicSwap(j, k, tid, iNrOfBins, iThreadId, cActiveVoxelNeighbors_pitched);
}

__global__ 
void
_ComputeEntropyOnSortedNeighborsInVectors_kernel
(
	int3 i3BlockCorner,
	int iNrOfBins,
	int3 i3Step,
	dim3 v3Grid,
	int3 i3KernelSize,
	unsigned int *puActiveVoxelSortedNeighbors_global,
	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	int iBlockIdX = blockIdx.x;
	int iBlockIdY = blockIdx.y % v3Grid.y;
	int iBlockIdZ = blockIdx.y / v3Grid.y;
	int iThreadX = iBlockIdX * blockDim.x + threadIdx.x;
	int iThreadY = iBlockIdY * blockDim.y + threadIdx.y;
	int iThreadZ = iBlockIdZ * blockDim.z + threadIdx.z;
	int iThreadId = iThreadX + iThreadY * i3Step.x + iThreadZ * i3Step.x * i3Step.y;

 	int iVoxelX = i3BlockCorner.x + iThreadX;
	int iVoxelY = i3BlockCorner.y + iThreadY;
	int iVoxelZ = i3BlockCorner.z + iThreadZ;

	int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);
	float fNrOfNeighbors = float(iNrOfNeighbors);

	float fEntropy = 0.0f;
	unsigned int uPrevBin = 0;
	float fCount = 0.0f; 

	for(int n = 0; n < iNrOfNeighbors; n++)
	{
		unsigned int uBin = tex2D(t2dActiveVoxelSortedNeighbors, iThreadId, n);

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
_CollectNeighborsInVectors_kernel
(
	int3 i3BlockCorner,
	int3 i3Step,
	dim3 v3Grid,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched
)
{
	int iBlockIdX = blockIdx.x;
	int iBlockIdY = blockIdx.y % v3Grid.y;
	int iBlockIdZ = blockIdx.y / v3Grid.y;
	int iThreadX = iBlockIdX * blockDim.x + threadIdx.x;
	int iThreadY = iBlockIdY * blockDim.y + threadIdx.y;
	int iThreadZ = iBlockIdZ * blockDim.z + threadIdx.z;
	int iThreadId = iThreadX + iThreadY * i3Step.x + iThreadZ * i3Step.x * i3Step.y;

 	int iVoxelX = i3BlockCorner.x + iThreadX;
	int iVoxelY = i3BlockCorner.y + iThreadY;
	int iVoxelZ = i3BlockCorner.z + iThreadZ;

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
					int,			cActiveVoxelNeighbors_pitched.ptr, 
					sizeof(int),	cActiveVoxelNeighbors_pitched.pitch, 
					iThreadId, iOffset) = iSrcBin;
			}
}
// ADD-BY-LEETEN 12/18/2009-END

void 
_ComputeEntropyVolumeWithSortingInVectors_cuda
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
	const int iNrOfRowsRoundTo2 = int(pow(2.0, ceil(log2(double(iNrOfRows)))));

	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	int iTotalNrOfXBlocks = int(ceilf(float(i3VolumeSize.x) / float(v3Blk.x)));
	int iTotalNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	int iTotalNrOfZBlocks = int(ceilf(float(i3VolumeSize.z) / float(v3Blk.z)));
	int iNrOfBlocksInMemory = MAX_MEMORY_SIZE / (iNrOfRowsRoundTo2 * sizeof(int) * v3Blk.x * v3Blk.y * v3Blk.z);
	int iNrOfXBlocksInMemory = min(iTotalNrOfXBlocks, iNrOfBlocksInMemory);
	int iNrOfYBlocksInMemory = min(iTotalNrOfYBlocks, max(1, iNrOfBlocksInMemory / iTotalNrOfXBlocks));
	int iNrOfZBlocksInMemory = min(iTotalNrOfZBlocks, max(1, iNrOfBlocksInMemory / (iTotalNrOfXBlocks * iTotalNrOfYBlocks)));

	dim3 v3Grid = dim3(iNrOfXBlocksInMemory, iNrOfYBlocksInMemory, iNrOfZBlocksInMemory);
	fprintf(stderr, "#BLOCKS = %d x %d x %d\n", v3Grid.x, v3Grid.y, v3Grid.z);

	int3 i3Step;
	i3Step.x = iNrOfXBlocksInMemory * v3Blk.x;
	i3Step.y = iNrOfYBlocksInMemory * v3Blk.y;
	i3Step.z = iNrOfZBlocksInMemory * v3Blk.z;

	cudaPitchedPtr cActiveVoxelNeighbors_pitched;
	cActiveVoxelNeighbors_pitched.xsize = v3Grid.x * v3Grid.y * v3Grid.z * v3Blk.x * v3Blk.y* v3Blk.z;
	cActiveVoxelNeighbors_pitched.ysize = iNrOfRowsRoundTo2;
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

	// bind the histogram as another texture where the type of each element is int4
	t2dActiveVoxelSortedNeighborsInVectors.addressMode[0] = cudaAddressModeClamp;
	t2dActiveVoxelSortedNeighborsInVectors.addressMode[1] = cudaAddressModeClamp;
	t2dActiveVoxelSortedNeighborsInVectors.filterMode =	cudaFilterModePoint;
	t2dActiveVoxelSortedNeighborsInVectors.normalized =	false;

	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dActiveVoxelSortedNeighborsInVectors, 
			cActiveVoxelNeighbors_pitched.ptr, 
			cudaCreateChannelDesc<int4>(),
			cActiveVoxelNeighbors_pitched.xsize * sizeof(int) / sizeof(int4),
			cActiveVoxelNeighbors_pitched.ysize, 
			cActiveVoxelNeighbors_pitched.pitch) );
CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	for(int			z = 0; z < i3VolumeSize.z; z += i3Step.z)
		for(int		y = 0; y < i3VolumeSize.y; y += i3Step.y)
			for(int	x = 0; x < i3VolumeSize.x; x += i3Step.x)
			{
				CLOCK_INIT(_ComputeEntropyVolume_PRINT_LOOP_TIMING, __FUNCTION__ " (main loop): ");

				CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				int3 i3BlockCorner = make_int3(x, y, z);
				_CollectNeighborsInVectors_kernel<<<
					dim3(v3Grid.x, v3Grid.y * v3Grid.z), 
					v3Blk, 
					0>>>
				(
					i3BlockCorner,
					i3Step,
					v3Grid,
					i3KernelSize,
					i3VolumeSize,
					iNrOfBins,
					cActiveVoxelNeighbors_pitched
				);
				CUT_CHECK_ERROR("_CollectNeighbors_kernel() failed");

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				// apply bitnoic sort to sort the data in descend order
				for (int k = 2; k <= iNrOfRowsRoundTo2; k *= 2)
					// Bitonic merge:
					for (int j = k / 2; j > 0; j /= 2)
					{
						_BitonicMergeInVectors_kernel<<<
							dim3(
								v3Grid.x * v3Grid.y * v3Grid.z / (sizeof(int4) / sizeof(int)) , 
								// iNrOfRowsRoundTo2),
								iNrOfRowsRoundTo2/2),
							dim3(
								v3Blk.x * v3Blk.y * v3Blk.z), 
							0>>>(
								k, 
								j,
								iNrOfBins,
								cActiveVoxelNeighbors_pitched
								);
						CUT_CHECK_ERROR("BitonicSort failed");
					}

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// 
				_ComputeEntropyOnSortedNeighborsInVectors_kernel<<<
					dim3(v3Grid.x, v3Grid.y * v3Grid.z), 
					v3Blk, 
					0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3Step,
					v3Grid,
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

	FREE_MEMORY(cActiveVoxelNeighbors_pitched.ptr);

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

#endif	// #if COMPUTE_ENTROPY_VOLUME_WITH_SORTING_IN_VECTORS_CUDA

/*

$Log: not supported by cvs2svn $

*/
