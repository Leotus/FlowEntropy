#if	COMPUTE_ENTROPY_VOLUME_SORT_PER_VOXEL_CUDA

__global__ 
void 
_CollectNeighborsHorizontally_kernel
(
	int3 i3BlockCorner,
	int3 i3KernelSize,
	int3 i3VolumeSize,
	int3 i3Steps,
	cudaPitchedPtr cActiveVoxelNeighbors_pitched
)
{
	int iActiveVoxelX = blockIdx.x;
	int iActiveVoxelY = blockIdx.y / i3Steps.y;
	int iActiveVoxelZ = blockIdx.y % i3Steps.y;
	int iActiveVoxelId = blockIdx.x + blockIdx.y * gridDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
	int iNrOfNeighbors = (i3KernelSize.x * 2 + 1) * (i3KernelSize.y * 2 + 1) * (i3KernelSize.z * 2 + 1);

	for(int n = threadIdx.x; n < iNrOfNeighbors; n += blockDim.x)
	{
		int iXOffset	= n % (i3KernelSize.x * 2 + 1);
		int iXYOffset	= n / (i3KernelSize.x * 2 + 1); 
		int iYOffset	= iXYOffset % (i3KernelSize.y * 2 + 1);
		int iZOffset	= iXYOffset / (i3KernelSize.y * 2 + 1);
		iXOffset -= i3KernelSize.x;
		iYOffset -= i3KernelSize.y;
		iZOffset -= i3KernelSize.z;

		int iX = iVoxelX + iXOffset;
		int iY = iVoxelY + iYOffset;
		int iZ = iVoxelZ + iZOffset;

		int3 i3TexCoord;
		i3TexCoord.x = IMirrorCoord(iX, i3VolumeSize.x);
		i3TexCoord.y = IMirrorCoord(iY, i3VolumeSize.y);
		i3TexCoord.z = IMirrorCoord(iZ, i3VolumeSize.z);

		int iSrcBin = tex2D(t2dSrcBinVolume, i3TexCoord.x, i3TexCoord.y + i3TexCoord.z * i3VolumeSize.y);

			*ADDRESS_2D(
				unsigned int,			cActiveVoxelNeighbors_pitched.ptr, 
				sizeof(unsigned int),	cActiveVoxelNeighbors_pitched.pitch, 
				n, 
				iActiveVoxelId) 
					= unsigned int(iSrcBin);
		}
}

void 
_ComputeEntropyVolumePerVoxel_cuda
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
	const int iNrOfNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1) * (2 * i3KernelSize.z + 1);

	int3 i3Step;

	int iRequiredMemoryInBytesPerVoxel = iNrOfNeighbors * sizeof(int);
	int iMaxVoxelsInMemory = MAX_MEMORY_SIZE / iRequiredMemoryInBytesPerVoxel;
	i3Step.x = min(i3VolumeSize.x, iMaxVoxelsInMemory);
	i3Step.y = min(i3VolumeSize.y, max(iMaxVoxelsInMemory / i3Step.x, 1));
	i3Step.z = min(i3VolumeSize.z, max(iMaxVoxelsInMemory / (i3Step.x * i3Step.y), 1));

	dim3 v3Blk = dim3(MAX_THREADS, 1);
	dim3 v3Grid = dim3(i3Step.x, i3Step.y * i3Step.z);

	fprintf(stderr, "#BLOCKS = %d x %d\n", v3Grid.x, v3Grid.y);

	cudaPitchedPtr cActiveVoxelNeighbors_pitched;
	cActiveVoxelNeighbors_pitched.xsize = iNrOfNeighbors;
	cActiveVoxelNeighbors_pitched.ysize = i3Step.x * i3Step.y * i3Step.z;
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
			cActiveVoxelNeighbors_pitched.xsize * cActiveVoxelNeighbors_pitched.ysize,
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
				_CollectNeighborsHorizontally_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					i3KernelSize,
					i3VolumeSize,
					i3Step,
					cActiveVoxelNeighbors_pitched
				);
				CUT_CHECK_ERROR("_CollectNeighborsHorizontally_kernel() failed");

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

/*
				for(int n = 0; n < v3Grid.x * v3Grid.y / 4096; n++)
					cudppSort(
						hScanPlan,
						(unsigned int*)cActiveVoxelNeighbors_pitched.ptr + 4096 * cActiveVoxelNeighbors_pitched.xsize * n,
						NULL,
						12,	// RADIX_SORT_BITS,
						4096 * cActiveVoxelNeighbors_pitched.xsize) ;
*/
					cudppSort(
						hScanPlan,
						cActiveVoxelNeighbors_pitched.ptr,
						NULL,
						18,	// RADIX_SORT_BITS,
						cActiveVoxelNeighbors_pitched.xsize * cActiveVoxelNeighbors_pitched.ysize) ;
				CUT_CHECK_ERROR("cudppSort() failed");
CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

#if	0
CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
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
#endif

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
			}


CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	cudppDestroyPlan(hScanPlan);
	FREE_MEMORY(cActiveVoxelNeighbors_pitched.ptr);

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

#endif	// #if	COMPUTE_ENTROPY_VOLUME_SORT_PER_VOXEL_CUDA


/*

$Log: not supported by cvs2svn $

*/
