#if	COMPUTE_ENTROPY_VOLUME_HOST

int iNrOfMargins;
int iNrOfMarginalBins;
static 	TBuffer<int> piMarginalHistogram_host;

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

	piMarginalHistogram_host[iSrcBin / iNrOfMargins] += iHisotgramOp; 
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
	for(int			xi = 0; xi < 2 * i3KernelSize.x + 1; xi++, i3Point.x++)
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
		i3Prev.x -= iDir * (i3KernelSize.x + 1);
		i3Next.x += iDir * i3KernelSize.x;
		break;
	case UPDATE_DIR_Y:	
		i3UDir = make_int3(1, 0, 0);
		i3VDir = make_int3(0, 0, 1);
		iUKernelSize = i3KernelSize.x;
		iVKernelSize = i3KernelSize.z;
		i3Prev.y -= iDir * (i3KernelSize.y + 1);
		i3Next.y += iDir * i3KernelSize.y;
		break;
	case UPDATE_DIR_Z:	
		i3UDir = make_int3(1, 0, 0);
		i3VDir = make_int3(0, 1, 0);
		iUKernelSize = i3KernelSize.x;
		iVKernelSize = i3KernelSize.y;
		i3Prev.z -= iDir * (i3KernelSize.z + 1);
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
	float* pfEntropyVolume_host
)
{
	float fEntropy = 0.0f;

	#if	SCANNING_METHOD == SCANNING_METHOD_SCAN_WHOLE_HISTOGRAM
	for(int b = 0; b < iNrOfBins; b++)
	{
		if( 0 == piHistogram_host[b] )
			continue;

		float fProb = 
			float(piHistogram_host[b]) / 
			float(
				(2 * i3KernelSize.x + 1) * 
				(2 * i3KernelSize.y + 1) * 
				(2 * i3KernelSize.z + 1) );
		fEntropy += fProb * log2f(fProb);
	}
	fEntropy = -fEntropy;

	#endif
	
	#if	SCANNING_METHOD == SCANNING_METHOD_SKIP_WITH_MARGINAL_HISTOGRAM
	for(int mb = 0; mb < iNrOfMarginalBins; mb++)
	{
		if( 0 == piMarginalHistogram_host[mb] )
			continue;

		for(int b = mb * iNrOfMargins, b0 = 0; b < iNrOfBins && b0 < iNrOfMargins; b0++, b++)
		{
			if( 0 == piHistogram_host[b] )
				continue;

			float fCount = float(piHistogram_host[b]);
			fEntropy += fCount * log2f(fCount);
		}
	}
	float fNrOfNeighbors = 	float(
		(2 * i3KernelSize.x + 1) * 
		(2 * i3KernelSize.y + 1) * 
		(2 * i3KernelSize.z + 1) );

	fEntropy = -fEntropy / fNrOfNeighbors + log2(fNrOfNeighbors);
	#endif

	fEntropy = max(fEntropy, 0.0f);

	pfEntropyVolume_host[i3Point.x + i3Point.y * i3VolumeSize.x + i3Point.z * i3VolumeSize.x * i3VolumeSize.y] = fEntropy;
}

void 
_ComputeEntropyVolume_host
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

	int *piHistogram_host;
	piHistogram_host = (int*)calloc(sizeof(piHistogram_host[0]), iNrOfBins);
	assert(piHistogram_host);

	iNrOfMargins = int(sqrtf(float(iNrOfBins)));
	iNrOfMarginalBins = int(ceilf(float(iNrOfBins) / float(iNrOfMargins)));

	piMarginalHistogram_host.alloc(iNrOfMarginalBins);

	int *piBinVolume_host;
	piBinVolume_host = (int*)calloc(sizeof(piBinVolume_host[0]), i3VolumeSize.x * i3VolumeSize.y * i3VolumeSize.z);
	assert(piBinVolume_host);

	_GetSrcBinVolume(piBinVolume_host);

	float* pfEntropyVolume_host;
	pfEntropyVolume_host = (float*)calloc(sizeof(pfEntropyVolume_host[0]), i3VolumeSize.x * i3VolumeSize.y * i3VolumeSize.z);
	assert( pfEntropyVolume_host );

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
					pfEntropyVolume_host
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
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemcpy2D(
			cEntropyVolume_pitched.ptr, 
			cEntropyVolume_pitched.pitch,
			pfEntropyVolume_host,
			i3VolumeSize.x * sizeof(pfEntropyVolume_host[0]),
			i3VolumeSize.x * sizeof(pfEntropyVolume_host[0]),
			i3VolumeSize.y * i3VolumeSize.z,
			cudaMemcpyHostToDevice) );
	free(pfEntropyVolume_host);

	free(piHistogram_host);
	free(piBinVolume_host);
CLOCK_END(_ComputeEntropyVolume_PRINT_TIMING, false);

CLOCK_PRINT(_ComputeEntropyVolume_PRINT_TIMING);
}

#endif	// #if #if	COMPUTE_ENTROPY_VOLUME_HOST

/*

$Log: not supported by cvs2svn $
Revision 1.2  2009/12/31 02:43:30  leeten

[12/30/2009]
1. [DEL] Remvoe the debug statement.

Revision 1.1  2009/12/27 19:05:04  leeten

[12/27/2009]
1. [1ST] First time checkin. Ths file define the functions to compute the entropy field on CPU.


*/
