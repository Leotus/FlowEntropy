#if	COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA

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
	cudaPitchedPtr cActiveVoxelNeighbors_pitched,
	int3 i3VolumeSize,
	cudaPitchedPtr cEntropyVolume_pitched
)
{
	int iActiveVoxelX = threadIdx.x + blockIdx.x * blockDim.x;
	int iActiveVoxelID = blockIdx.x + blockIdx.y * gridDim.x;
	int iActiveVoxelYZ = blockIdx.y;
	int iActiveVoxelY = iActiveVoxelYZ % i3Step.y;
	int iActiveVoxelZ = iActiveVoxelYZ / i3Step.y;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;

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
			unsigned int uX = uBin % (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp);
			uBin = uBin / (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp);

			int iX = int(uX) - iRoundKernelSizeXToHalfWarp;
			if( iX > iVoxelX + i3KernelSize.x || iX < iVoxelX - i3KernelSize.x )
				continue;

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

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z )
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
	int iNrOfBins,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched
)
{
	// NOTE!! here blockDim.x is the actuall #threads; 
	// but in order to get the idea #threads per block, 
	// the tails in the 2-sided should be substratected
	int iActiveVoxelX = threadIdx.x + blockIdx.x * (blockDim.x - 2 * iRoundKernelSizeXToHalfWarp);	
	int iActiveVoxelID = blockIdx.x + blockIdx.y * gridDim.x;
	int iActiveVoxelYZ = blockIdx.y;
	int iActiveVoxelY = iActiveVoxelYZ % i3Step.y;
	int iActiveVoxelZ = iActiveVoxelYZ / i3Step.y;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX - iRoundKernelSizeXToHalfWarp;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;

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

			*ADDRESS_2D(
				unsigned int,			cActiveVoxelNeighbors_pitched.ptr, 
				sizeof(unsigned int),	cActiveVoxelNeighbors_pitched.pitch, 
				threadIdx.x, 
				iActiveVoxelID * iNrOfRows + iOffset) 
					= unsigned int(iSrcBin * (i3VolumeSize.x + 2 * iRoundKernelSizeXToHalfWarp) + (iVoxelX + iRoundKernelSizeXToHalfWarp));
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
	int3 i3NrOfSteps;

	const int iNrOfRows = (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	int iRoundKernelSizeXToHalfWarp = NR_OF_THREADS_PER_HALF_WARP * int(ceilf( float(i3KernelSize.x) / float(NR_OF_THREADS_PER_HALF_WARP) ));

	int iNrOfNeededThreadsPerBlock = min(MAX_THREADS - 2 * iRoundKernelSizeXToHalfWarp, i3VolumeSize.x);
	int iNrOfActualThreadsPerBlock = iNrOfNeededThreadsPerBlock + 2 * iRoundKernelSizeXToHalfWarp;		// #threads (including the real ones and the tailing ones)
	v3Blk = dim3(iNrOfActualThreadsPerBlock, 1, 1);
	v3Grid = dim3(1, i3VolumeSize.y, 1);
	i3Step.x = iNrOfNeededThreadsPerBlock;
	i3Step.y = i3VolumeSize.y;
	i3Step.z = 1;

	fprintf(stderr, "MEM = %d MB; #BLOCKS = %d x %d\n", MAX_MEMORY_SIZE / (1<<20), v3Grid.x, v3Grid.y);

	cudaPitchedPtr cActiveVoxelNeighbors_pitched;
	cActiveVoxelNeighbors_pitched.xsize = v3Blk.x;
	cActiveVoxelNeighbors_pitched.ysize = iNrOfRows * v3Grid.x * v3Grid.y;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocPitch(
			(void**)&cActiveVoxelNeighbors_pitched.ptr, 
			&cActiveVoxelNeighbors_pitched.pitch,
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
					v3Grid, 
					v3Blk, 
					0>>>
				(
					i3BlockCorner,
					i3KernelSize,
					iRoundKernelSizeXToHalfWarp,
					i3VolumeSize,
					#if	1	// TEST-ADD
					i3Step,
					#endif
					iNrOfBins,
					cActiveVoxelNeighbors_pitched
				);
				CUT_CHECK_ERROR("_CollectNeighborsAlongScanline_kernel() failed");

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				// call cudpp to sort the buffer _CollectNeighborsToBeSorted_kernel
				for(int y2 = 0; y2 < int(v3Grid.y); y2++)
				{
					cudppSort(
						hScanPlan,
						((unsigned int*)cActiveVoxelNeighbors_pitched.ptr) + y2 * iNrOfRows * v3Blk.x,
						NULL,
						32,	// RADIX_SORT_BITS,
						iNrOfRows * v3Blk.x) ;
					CUT_CHECK_ERROR("cudppSort() failed");
				}

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// 
				_ComputeEntropyAlongScanline_kernel<<<
					v3Grid, 
					dim3(iNrOfNeededThreadsPerBlock, 1, 1),
					0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					iRoundKernelSizeXToHalfWarp,
					i3Step,
					cActiveVoxelNeighbors_pitched,
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
	
#endif	// #if	COMPUTE_ENTROPY_VOLUME_PER_SCANLINE_CUDA

/*

$Log: not supported by cvs2svn $

*/
