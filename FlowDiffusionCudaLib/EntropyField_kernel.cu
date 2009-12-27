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

// DEL-BY-LEETEN 12/23/2009-BEGIN
/*
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
*/
// DEL-BY-LEETEN 12/23/2009-END


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


/*

$Log: not supported by cvs2svn $
Revision 1.2  2009/12/20 03:21:45  leeten

[12/19/2009]
1. [MOD] Change the 1D texture (t1dActiveVoxelHistorgrams) to a 2D texture (t2dActiveVoxelHistorgrams).
2. [MOD] Change the function name _Vector3DToVolume_kernel to _Vector3DToBinVolume_kernel.
3. [ADD] Define the kernel and device functions to compute the histogram on GPUs.
4. [ADD] Define the kernel and device functions to compute the entropy of each voxel by scanning the sorted bins of its neighboring regions.

Revision 1.1  2009/12/17 17:44:56  leeten

[12/17/2009]
1. [1ST] First time checkin.


*/
