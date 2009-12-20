#include "FlowDiffusion_cuda.h"

enum {
	UPDATE_DIR_X, 
	UPDATE_DIR_Y, 
	UPDATE_DIR_Z, 
};

enum {
	HISTOGRAM_OP_SUB_SLICE = -1, 
	HISTOGRAM_OP_ADD_SLICE = +1, 
};

						// a texture tha represents the src. bin volume
						// this textrue defines the input for the kernel
static texture<int, 2, cudaReadModeElementType> t2dSrcBinVolume;
						// a texture tha represents the dst. bin volume
static texture<int, 2, cudaReadModeElementType> t2dDstBinVolume;
static texture<int, 1, cudaReadModeElementType> t1dHistogram;

					// a volume of integer, where each element is the joint histogram of both the src. and dst. bin volume
					// here the joint of two bins mean aan integer that represent both numbers
static cudaPitchedPtr cSrcDstBinVolumePtr_global;	

// ADD-BY-LEETEN 12/18/2009-BEGIN
static texture<int, 2, cudaReadModeElementType> t2dActiveVoxelHistorgrams;

// MOD-BY-LEETEN 12/18/2009-FROM
	// static texture<unsigned int, 1, cudaReadModeElementType> t1dActiveVoxelSortedNeighbors;
// TO:
static texture<unsigned int, 2, cudaReadModeElementType> t2dActiveVoxelSortedNeighbors;
// MOD-BY-LEETEN 12/18/2009-END
// ADD-BY-LEETEN 12/18/2009-END


// the kernel to update the histogram 
__device__ 
int IMirrorCoord(int iCoord, int iSize)
{
	int iMirroredCoord = iCoord;
	if( iCoord < 0 )
		iMirroredCoord = -iCoord;
	if( iCoord > iSize - 1 )
		iMirroredCoord = iSize - (iCoord - (iSize - 1));
	return iMirroredCoord;
}

// before calling this kernel, the input volume should be bound to the texture t2dBinVolume

__global__ 
void
_UpdateSliceToHistogram_kernel
(
	int3 i3Center, 
	int	iUpdateDir, 
	int iHisotgramOp, 

	int3 i3UDir, 
	int3 i3VDir,
	int iUKernelSize, 
	int iVKernelSize,

	int3 i3VolumeSize,

	int iNrOfBins,
	int *piHistorgram_global
)
{
 	int iX = blockIdx.x * blockDim.x + threadIdx.x;
 	int iY = blockIdx.y * blockDim.y + threadIdx.y;
	int3 i3Point;
	i3Point.x = i3Center.x + (iX - iUKernelSize) * i3UDir.x + (iY - iVKernelSize) * i3VDir.x;
	i3Point.y = i3Center.y + (iX - iUKernelSize) * i3UDir.y + (iY - iVKernelSize) * i3VDir.y;
	i3Point.z = i3Center.z + (iX - iUKernelSize) * i3UDir.z + (iY - iVKernelSize) * i3VDir.z;

	// read the bin
	int3 i3TexCoord;
	i3TexCoord.x = IMirrorCoord(i3Point.x, i3VolumeSize.x);
	i3TexCoord.y = IMirrorCoord(i3Point.y, i3VolumeSize.y);
	i3TexCoord.z = IMirrorCoord(i3Point.z, i3VolumeSize.z);

	if( iX < 2 * iUKernelSize + 1 && iY < 2 * iVKernelSize + 1)
	{
		int iSrcBin = tex2D(t2dSrcBinVolume, i3TexCoord.x, i3TexCoord.y + i3TexCoord.z * i3VolumeSize.y);

		// update the joint histogram
		atomicAdd(&piHistorgram_global[iSrcBin], iHisotgramOp );
	}
}

// comptue p log p for each bin
// the final entropy will be summed later via CUDPP
__global__ 
void 
_ComputeProbDotLogProb_kernel
(
	int iNrOfElements,

	int iNrOfBins,
	float *pfLogHistogram_global
)
{
	int iBin = blockIdx.x * blockDim.x + threadIdx.x;
	int iCount = tex1D(t1dHistogram, iBin);
	float fProbDotLogProb = 0.0f;
	if( iCount )
	{
		float fCount = float(iCount);
		fProbDotLogProb = log2(fCount) * fCount;
	}
	float fNrOfElements = float(iNrOfElements);
	fProbDotLogProb = log2(fNrOfElements)/float(iNrOfBins) - fProbDotLogProb / fNrOfElements;

	if( iBin < iNrOfBins )
	{
		pfLogHistogram_global[iBin] = fProbDotLogProb;
	}
}

__global__
void 
_JointSrcDst_kernel
(
	int iNrOfSrcBins, 
	int iNrOfDstbins,

	int3 i3VolumeSize,
	cudaPitchedPtr cJointBinVolume_pitched
)
{
	int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iVoxelYZ = blockIdx.y * blockDim.y + threadIdx.y;
	int iVoxelY = (iVoxelYZ % i3VolumeSize.y);
	int iVoxelZ = (iVoxelYZ / i3VolumeSize.y);

	int iSrcBin = tex2D(t2dSrcBinVolume, iVoxelX,	iVoxelY + iVoxelZ * i3VolumeSize.y);
	int iDstBin = tex2D(t2dDstBinVolume, iVoxelX,	iVoxelY + iVoxelZ * i3VolumeSize.y);
	int iJointBin = iSrcBin + iDstBin * iNrOfSrcBins;

	if( iVoxelX < i3VolumeSize.x && iVoxelY < i3VolumeSize.y && iVoxelZ < i3VolumeSize.z )  
		*ADDRESS_2D(
			int, cJointBinVolume_pitched.ptr, 
			sizeof(int), cJointBinVolume_pitched.pitch, 
			iVoxelX, iVoxelY + iVoxelZ * i3VolumeSize.y) = iJointBin;
}

__global__ 
static 
void 
// MOD-BY-LEETEN 12/18/2009-FROM:
	// _Vector3DToVolume_kernel
// TO:
_Vector3DToBinVolume_kernel
// MOD-BY-LEETEN 12/18/2009-END
(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,

	int iNrOfYBlocks,
	int iBlockZSize,

	// texture<float4, 2, cudaReadModeElementType> t2dSrc,
	cudaPitchedPtr cBinVolumePtr_global
)
{
 	int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iVoxelY = (blockIdx.y % iNrOfYBlocks) * blockDim.y + threadIdx.y;
	int iBeginZ = (blockIdx.y / iNrOfYBlocks) * iBlockZSize;
	int iEndZ = min(iBeginZ + iBlockZSize, iVolumeDepth);

	for(int z = iBeginZ; z < iEndZ; z++)
	{
		float4 f4Vector = tex2D(t2dVectorVolume, iVoxelX,		iVoxelY + z				* iVolumeHeight);;
		
		float fLength	= sqrt(f4Vector.x * f4Vector.x + f4Vector.y * f4Vector.y + f4Vector.z * f4Vector.z);
		float fTheta	= 0.0f;
		float fPhi		= 0.0f;
		int iBin = 0;

		if( 0 < fLength )
		{
			f4Vector.x /= fLength;
			f4Vector.y /= fLength;
			f4Vector.z /= fLength;
			fTheta = ( 0.0f == f4Vector.x && 0.0f == f4Vector.y )?0.0f:(float(M_PI)+(atan2(f4Vector.y, f4Vector.x)));
			fTheta /= 2.0f * float(M_PI);

			float fLength2D = sqrt(f4Vector.x * f4Vector.x + f4Vector.y * f4Vector.y);
			fPhi = ((0.0f == fLength2D)&&(0.0f == f4Vector.z))?0.0f:fabs(float(M_PI)/2.0f-(atan2(f4Vector.z, fLength2D)));
			fPhi /= float(M_PI);
			iBin = tex2D(t2dAngleMap, fPhi, fTheta);
		}

		if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
		{
			*ADDRESS_2D(
				int,			cBinVolumePtr_global.ptr, 
				sizeof(int),	cBinVolumePtr_global.pitch, 
				iVoxelX, iVoxelY + z * iVolumeHeight) = iBin;
		}
	}
}

// ADD-BY-LEETEN 12/18/2009-BEGIN
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
	int iNrOfXYNeighbors = (2 * i3KernelSize.x + 1) * (2 * i3KernelSize.y + 1);
	#endif	// MOD-BY-LEETEN 12/19/2009-END

	// MOD-BY-LEETEN 12/19/2009-FROM:
		// if ( 0 == iVoxelY )
	// TO:
	if ( 0 == iVoxelZ )
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
	else
	{
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
}
// ADD-BY-LEETEN 12/18/2009-END


/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/12/17 17:44:56  leeten

[12/17/2009]
1. [1ST] First time checkin.


*/
