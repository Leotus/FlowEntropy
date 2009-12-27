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
	unsigned int uPrevBin = 0;
	float fCount = 0.0f; 

	// ADD-BY-LEETEN 12/18/2009-BEGIN
	unsigned int uNrOfActiveVoxels = gridDim.x * blockDim.x * gridDim.y * blockDim.y;
	unsigned int uTexIndex = iNrOfNeighbors * iActiveVoxelId;
	// ADD-BY-LEETEN 12/18/2009-END

	for(int n = 0; n < iNrOfNeighbors; n++)
	{
		#if	0	// MOD-BY-LEETEN 12/18/2009-FROM:
			unsigned int uBin = tex1Dfetch(t1dActiveVoxelSortedNeighbors, iNrOfNeighbors * iActiveVoxelId + n);
			// unsigned int uBin = puActiveVoxelSortedNeighbors_global[iNrOfNeighbors * iActiveVoxelId + n];
			// uBin = uBin % unsigned int (iNrOfBins);
		#else	// MOD-BY-LEETEN 12/18/2009-TO:
		float fTexCoordX = float(uTexIndex % uNrOfActiveVoxels);
		float fTexCoordY = float(uTexIndex / uNrOfActiveVoxels);
		unsigned int uBin = tex2D(t2dActiveVoxelSortedNeighbors, fTexCoordX, fTexCoordY);
		uTexIndex++;
		#endif	// MOD-BY-LEETEN 12/18/2009-END

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

	// ADD-BY-LEETEN 12/20/2009-BEGIN
	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z )
	// ADD-BY-LEETEN 12/20/2009-END
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
	#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
		int iActiveVoxelZ = blockIdx.y * blockDim.y + threadIdx.y;
		int iActiveVoxelId = iActiveVoxelX + iActiveVoxelZ * gridDim.x * blockDim.x;
 		int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
		int iVoxelY = i3BlockCorner.y;
		int iVoxelZ = i3BlockCorner.z + iActiveVoxelZ;
		int iNrOfXZNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.z + 1);
	#else	// MOD-BY-LEETEN 12/19/2009-TO:
	int iActiveVoxelY = blockIdx.y * blockDim.y + threadIdx.y;
	int iActiveVoxelId = iActiveVoxelX + iActiveVoxelY * gridDim.x * blockDim.x;
 	int iVoxelX = i3BlockCorner.x + iActiveVoxelX;
	int iVoxelY = i3BlockCorner.y + iActiveVoxelY;
	int iVoxelZ = i3BlockCorner.z;
	// DEL-BY-LEETEN 12/20/2009-BEGIN
		// int iNrOfXYNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1);
	// DEL-BY-LEETEN 12/20/2009-END
	#endif	// MOD-BY-LEETEN 12/19/2009-END

	// MOD-BY-LEETEN 12/19/2009-FROM:
		// if ( 0 == iVoxelY )
	// TO:
	// DEL-BY-LEETEN 12/20/2009-BEGIN
		// if ( 0 == iVoxelZ )
	// DEL-BY-LEETEN 12/20/2009-END
	// MOD-BY-LEETEN 12/19/2009-END
	{
		#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
			for(int			iOffset = 0,	iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
				for(int						iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
					for(int					iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++, iOffset++)
		#else	// MOD-BY-LEETEN 12/19/2009-TO:
		for(int	iOffset = 0,iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
			for(int			iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
				for(int		iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++, iOffset++)
		#endif	// MOD-BY-LEETEN 12/19/2009-END
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
	#if	0	// DEL-BY-LEETEN 12/20/2009-BEGIN
	else
	{
		// ADD-BY-LEETEN 12/20/2009-BEGIN
		int iNrOfXYNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1);
		// ADD-BY-LEETEN 12/20/2009-END
		#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
			int iYOffset = i3KernelSize.y;
			for(int		iXZOffset = 0,	iZOffset = -i3KernelSize.z; iZOffset <= i3KernelSize.z; iZOffset++)
				for(int					iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++, iXZOffset++)
		#else	// MOD-BY-LEETEN 12/19/2009-TO:
		int iZOffset = i3KernelSize.z;
		for(int		iXYOffset = 0,	iYOffset = -i3KernelSize.y; iYOffset <= i3KernelSize.y; iYOffset++)
			for(int					iXOffset = -i3KernelSize.x; iXOffset <= i3KernelSize.x; iXOffset++, iXYOffset++)
		#endif	// MOD-BY-LEETEN 12/19/2009-END
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

				#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
					int iModY = iVoxelY % (2 * i3KernelSize.y + 1);
					int iOffset = (0 == iModY)?(2 * i3KernelSize.y):(iModY - 1);
					iOffset = iOffset * iNrOfXZNeighbors + iXZOffset;
				#else	// MOD-BY-LEETEN 12/19/2009-TO:
				int iModZ = iVoxelZ % (2 * i3KernelSize.z + 1);
				int iOffset = (0 == iModZ)?(2 * i3KernelSize.z):(iModZ - 1);
				iOffset = iOffset * iNrOfXYNeighbors + iXYOffset;
				#endif	// MOD-BY-LEETEN 12/19/2009-END
				*ADDRESS_2D(
					unsigned int,			cActiveVoxelNeighbors_pitched.ptr, 
					sizeof(unsigned int),	cActiveVoxelNeighbors_pitched.pitch, 
					iActiveVoxelId, iOffset) = unsigned (iActiveVoxelId * iNrOfBins + iSrcBin);
			}
	}
	#endif	// DEL-BY-LEETEN 12/20/2009-END
}
// ADD-BY-LEETEN 12/18/2009-END

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
	#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
	#else	// MOD-BY-LEETEN 12/19/2009-TO:
	int iNrOfYBlocks = int(ceilf(float(i3VolumeSize.y) / float(v3Blk.y)));
	int iMaxNrOfThreads = int(floor(double(1<<(RADIX_SORT_BITS-1)) / double(iNrOfBins)));
	int iMaxNrOfBlocks	= int(floorf(float(iMaxNrOfThreads) / float(v3Blk.x * v3Blk.y)));
	iMaxNrOfBlocks = min(iMaxNrOfBlocks, iNrOfXBlocks * iNrOfYBlocks);
	iNrOfXBlocks = min(iNrOfXBlocks, iMaxNrOfBlocks);
	iNrOfYBlocks = int(ceilf(float(iMaxNrOfBlocks) / float(iNrOfXBlocks)));
	dim3 v3Grid = dim3(iNrOfXBlocks, iNrOfYBlocks);
	#endif	// MOD-BY-LEETEN 12/19/2009-END
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

	#if	0	// DEL-BY-LEETEN 12/20/2009-BEGIN
		cudaPitchedPtr cActiveVoxelSortedNeighbors_pitched = cActiveVoxelNeighbors_pitched;
		CUDA_SAFE_CALL_NO_SYNC(
			cudaMallocPitch(
				(void**)&cActiveVoxelSortedNeighbors_pitched.ptr, 
				&cActiveVoxelSortedNeighbors_pitched.pitch,
				cActiveVoxelSortedNeighbors_pitched.xsize * sizeof(int), 
				cActiveVoxelSortedNeighbors_pitched.ysize)	);
	#endif	// DEL-BY-LEETEN 12/20/2009-END

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
	#if	0	// MOD-BY-LEETEN 12/18/2009-FROM:
	#else	// MOD-BY-LEETEN 12/18/2009-TO:
	t2dActiveVoxelSortedNeighbors.addressMode[0] = cudaAddressModeClamp;
	t2dActiveVoxelSortedNeighbors.addressMode[1] = cudaAddressModeClamp;
	t2dActiveVoxelSortedNeighbors.filterMode =	cudaFilterModePoint;
	t2dActiveVoxelSortedNeighbors.normalized =	false;

	#if	0	// DEL-BY-LEETEN 12/20/2009-BEGIN
		CUDA_SAFE_CALL_NO_SYNC(
			cudaBindTexture2D(
				0, 
				t2dActiveVoxelSortedNeighbors, 
				cActiveVoxelSortedNeighbors_pitched.ptr, 
				cudaCreateChannelDesc<unsigned int>(),
				cActiveVoxelSortedNeighbors_pitched.xsize,
				cActiveVoxelSortedNeighbors_pitched.ysize, 
				cActiveVoxelSortedNeighbors_pitched.pitch) );
	#else	// DEL-BY-LEETEN 12/20/2009-END
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dActiveVoxelSortedNeighbors, 
			cActiveVoxelNeighbors_pitched.ptr, 
			cudaCreateChannelDesc<unsigned int>(),
			cActiveVoxelNeighbors_pitched.xsize,
			cActiveVoxelNeighbors_pitched.ysize, 
			cActiveVoxelNeighbors_pitched.pitch) );
#endif
	#endif	// MOD-BY-LEETEN 12/18/2009-END

	CUDPPHandle hScanPlan = 0;

	CUDPPConfiguration	cConfig;
	cConfig.op =		CUDPP_ADD;
	cConfig.datatype =	CUDPP_UINT;
	cConfig.algorithm =	CUDPP_SORT_RADIX;
	cConfig.options =	CUDPP_OPTION_KEYS_ONLY;

	#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
		assert( 
			CUDPP_SUCCESS  == cudppPlan(
				&hScanPlan,	
				cConfig, 
				cActiveVoxelSortedNeighbors_pitched.pitch * cActiveVoxelSortedNeighbors_pitched.ysize / sizeof(unsigned int),
				1, 
				0) );
	#else	// MOD-BY-LEETEN 12/20/2009-TO:
	assert( 
		CUDPP_SUCCESS  == cudppPlan(
			&hScanPlan,	
			cConfig, 
			cActiveVoxelNeighbors_pitched.pitch * cActiveVoxelNeighbors_pitched.ysize / sizeof(unsigned int),
			1, 
			0) );
	#endif	// MOD-BY-LEETEN 12/20/2009-END

CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_TIMING);

	#if	0	// MOD-BY-LEETEN 12/19/2009-FROM:
	#else	// MOD-BY-LEETEN 12/19/2009-TO:
	for(int			y = 0; y < i3VolumeSize.y; y += v3Grid.y * v3Blk.y)
		for(int		x = 0; x < i3VolumeSize.x; x += v3Grid.x * v3Blk.x)
			for(int	z = 0; z < i3VolumeSize.z; z ++ )
	#endif	// MOD-BY-LEETEN 12/19/2009-END
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

				#if	0	// TEST-DEBUG
											unsigned int *puActiveVoxelNeighbors_host;
											CUDA_SAFE_CALL_NO_SYNC(
												cudaMallocHost(
													(void**)&puActiveVoxelNeighbors_host,
													cActiveVoxelNeighbors_pitched.pitch * cActiveVoxelSortedNeighbors_pitched.ysize) );

											CUDA_SAFE_CALL_NO_SYNC(
												cudaMemcpy(
													puActiveVoxelNeighbors_host, 
													cActiveVoxelNeighbors_pitched.ptr,
													cActiveVoxelSortedNeighbors_pitched.pitch * cActiveVoxelSortedNeighbors_pitched.ysize,
													cudaMemcpyDeviceToHost) );
											for(unsigned int p = 0,	n = 0; n < cActiveVoxelSortedNeighbors_pitched.ysize; n++)
												for(unsigned int	i = 0; i < cActiveVoxelNeighbors_pitched.pitch / sizeof(unsigned int); i++, p++)
												{
													unsigned int uTemp = puActiveVoxelNeighbors_host[p];
													unsigned int uId = uTemp / unsigned int(iNrOfBins);
													unsigned int uBin = uTemp % unsigned int(iNrOfBins);

													fprintf(stderr, "%d, %d, %d\n", n, uId, uBin);
												}
											FREE_MEMORY_ON_HOST(puActiveVoxelNeighbors_host);
				#endif

CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

#if	0	// DEL-BY-LEETEN 12/20/2009-BEGIN
	CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

					// copy the memory from the buffer _CollectNeighbors_kernel to another buffer _CollectNeighborsToBeSorted_kernel
					#if	0	// MOD-BY-LEETEN 12/18/2009-FROM:
						CUDA_SAFE_CALL_NO_SYNC(
							cudaMemcpy(
								cActiveVoxelSortedNeighbors_pitched.ptr, 
								cActiveVoxelNeighbors_pitched.ptr, 
								cActiveVoxelSortedNeighbors_pitched.pitch * cActiveVoxelSortedNeighbors_pitched.ysize,
								cudaMemcpyDeviceToDevice));
					#else	// MOD-BY-LEETEN 12/18/2009-TO:
					CUDA_SAFE_CALL_NO_SYNC(
						cudaMemcpy2D(
							cActiveVoxelSortedNeighbors_pitched.ptr, 
							cActiveVoxelSortedNeighbors_pitched.pitch,
							cActiveVoxelNeighbors_pitched.ptr, 
							cActiveVoxelNeighbors_pitched.pitch,
							cActiveVoxelSortedNeighbors_pitched.pitch,
							cActiveVoxelSortedNeighbors_pitched.ysize,
							cudaMemcpyDeviceToDevice) );
					#endif	// MOD-BY-LEETEN 12/18/2009-END

	CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);
#endif	// DEL-BY-LEETEN 12/20/2009-END

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);

				// call cudpp to sort the buffer _CollectNeighborsToBeSorted_kernel
				#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
					cudppSort(
						hScanPlan,
						cActiveVoxelSortedNeighbors_pitched.ptr,
						NULL,
						RADIX_SORT_BITS,
						cActiveVoxelSortedNeighbors_pitched.pitch * cActiveVoxelSortedNeighbors_pitched.ysize / sizeof(unsigned int)) ;
				#else	// MOD-BY-LEETEN 12/20/2009-TO:
				cudppSort(
					hScanPlan,
					cActiveVoxelNeighbors_pitched.ptr,
					NULL,
					RADIX_SORT_BITS,
					cActiveVoxelNeighbors_pitched.pitch * cActiveVoxelNeighbors_pitched.ysize / sizeof(unsigned int)) ;
				#endif	// MOD-BY-LEETEN 12/20/2009-END
				CUT_CHECK_ERROR("cudppSort() failed");
CLOCK_END(_ComputeEntropyVolume_PRINT_LOOP_TIMING, false);

CLOCK_BEGIN(_ComputeEntropyVolume_PRINT_LOOP_TIMING);
				// 
				#if	0	// MOD-BY-LEETEN 12/20/2009-FROM:
					_ComputeEntropyOnSortedNeighbors_kernel<<<v3Grid, v3Blk, 0>>>
					(
						i3BlockCorner,
						iNrOfBins,
						i3KernelSize,
						(unsigned int *)cActiveVoxelSortedNeighbors_pitched.ptr,
						i3VolumeSize,
						cEntropyVolume_pitched
					);
				#else	// MOD-BY-LEETEN 12/20/2009-TO:
				_ComputeEntropyOnSortedNeighbors_kernel<<<v3Grid, v3Blk, 0>>>
				(
					i3BlockCorner,
					iNrOfBins,
					i3KernelSize,
					(unsigned int *)cActiveVoxelNeighbors_pitched.ptr,
					i3VolumeSize,
					cEntropyVolume_pitched
				);
				#endif	// MOD-BY-LEETEN 12/20/2009-END
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

*/
