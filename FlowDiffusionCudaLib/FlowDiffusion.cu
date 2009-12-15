////////////////////////////////////////////
	#include <stdio.h>
	#include <texture_fetch_functions.h>
	// ADD-BY-LEETEN 2009/11/25-BEGIN
	#define USE_CUDPP	0

	#if	USE_CUDPP	
		#include "cudpp/cudpp.h"
		#pragma comment (lib, "cudpp32.lib")
	#endif
	// ADD-BY-LEETEN 2009/11/25-END

	#include "cuda_macro.h"

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	#include "liblog.h"
	using namespace std;

	#define	M_PI	3.1415926535897932384626433832795f
	// ADD-BY-LEETEN 12/14/2009-END

	#include "libbuf.h"

////////////////////////////////////////////
	#define PRINT_FLOW_FUSION_TIMING	1
	#define USE_SHARED_MEMORY			0

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	#define SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING	1	
	// ADD-BY-LEETEN 12/14/2009-END

	// ADD-BY-LEETEN 11/04/2009-BEGIN
						// if this preprocessor is non zero, the volume is scanned via a for loop on the host
	#define DIFFUSION_BY_FOR_LOOP_ON_HOST		0
	// ADD-BY-LEETEN 11/04/2009-END

	// ADD-BY-LEETEN 10/02/2009-BEGIN
	#define BLOCK_DIM_X	16
	#define BLOCK_DIM_Y	12
	// ADD-BY-LEETEN 10/02/2009-END

////////////////////////////////////////////
	static cudaExtent cVolumeExtent;
	static float4 *pf4Volume_host;
	static cudaPitchedPtr pcVolumePtrs_global[2];
	static texture<float4, 2, cudaReadModeElementType> t2dSrc;
	// ADD-BY-LEETEN 10/02/2009-BEGIN
						// array and texture reference for the weight and offset volume
	static cudaExtent cVolumeExtent_array;
	static float4 *pf4WeightOffsetVolume_host;
	static cudaArray *cWeightOffsetVolume_array = 0;
	static texture<float4, 3, cudaReadModeElementType> t3dWeightOffset;
    static cudaChannelFormatDesc cWeightOffsetChannelDesc = cudaCreateChannelDesc<float4>();
	// ADD-BY-LEETEN 10/02/2009-END

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	#if	USE_CUDPP
    static CUDPPConfiguration cConfig;
    static CUDPPHandle cScanplan = 0;
	#endif	// #if	USE_CUDPP

	static cudaExtent cErrorVolumeExtent;
	static cudaPitchedPtr cErrorVolume_device;
	static cudaPitchedPtr cErrorSum_device;
	// ADD-BY-LEETEN 2009/11/25-END

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	int iNrOfVoxels;

	static texture<int, 2, cudaReadModeElementType> t2dAngleMap;
	static cudaArray *pcAngleMap_array;	// cuda array to hold the Gaussian kernels

	static cudaExtent cBinVolumeExtent;
	static cudaPitchedPtr cSrcBinVolumePtr_global;
	static cudaPitchedPtr cDstBinVolumePtr_global;
	static int *piSrcAngleBinVolume_host;
	static int *piDstAngleBinVolume_host;

	static texture<float4, 2, cudaReadModeElementType> t2dBinVolumeSrc;
	// ADD-BY-LEETEN 12/14/2009-END

// ADD-BY-LEETEN 12/07/2009-BEGIN
__global__ 
static 
void 
_FlowDiffusion2D_kernel
(
	// INPUT
	float fAttenuation,

	int iVolumeWidth,
	int iVolumeHeight,

	cudaPitchedPtr cDstPitchedPtr,

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	cudaPitchedPtr cErrorPitchedPtr
	// ADD-BY-LEETEN 2009/11/25-END
)
{
 	int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	int iVoxelY = blockIdx.y * blockDim.y + threadIdx.y;

	// compute the central differnece
	float4 f4Value	= tex2D(t2dSrc, iVoxelX,		iVoxelY);
	float4 f4PX		= tex2D(t2dSrc, min(iVoxelX + 1, iVolumeWidth - 1),	iVoxelY);
	float4 f4NX		= tex2D(t2dSrc, max(iVoxelX - 1, 0),				iVoxelY);
	float4 f4PY		= tex2D(t2dSrc, iVoxelX,		min(iVoxelY + 1, iVolumeHeight - 1));
	float4 f4NY		= tex2D(t2dSrc, iVoxelX,		max(iVoxelY - 1, 0)				);

	float4 f4WeightOffset = tex3D(t3dWeightOffset, iVoxelX,	iVoxelY, 0);

	float4 f4Result;

	f4Result = make_float4(
		f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x - 4.0f * f4Value.x) * fAttenuation + f4WeightOffset.x,
		f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y - 4.0f * f4Value.y) * fAttenuation + f4WeightOffset.y,
		f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z - 4.0f * f4Value.z) * fAttenuation + f4WeightOffset.z,
		0);

	if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
	{
		*ADDRESS_2D(
			float4, cDstPitchedPtr.ptr, 
			sizeof(float4), cDstPitchedPtr.pitch, 
			iVoxelX, iVoxelY) = f4Result;

		float4 f4Diff;
		f4Diff.x = f4Value.x - f4Result.x;
		f4Diff.y = f4Value.y - f4Result.y;
		f4Diff.z = f4Value.z - f4Result.z;
		float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
		*ADDRESS_2D(
			float, cErrorPitchedPtr.ptr, 
			sizeof(float), cErrorPitchedPtr.pitch, 
			iVoxelX, iVoxelY) = fDiff;
	}
}

// ADD-BY-LEETEN 12/07/2009-END

// ADD-BY-LEETEN 12/14/2009-BEGIN
__global__ 
static 
void 
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusion_kernel
// TO:
_Vector3DToVolume_kernel
// MOD-BY-LEETEN 12/07/2009-END
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
		float4 f4Vector = tex2D(t2dBinVolumeSrc, iVoxelX,		iVoxelY + z				* iVolumeHeight);;
		
		float fLength	= sqrt(f4Vector.x * f4Vector.x + f4Vector.y * f4Vector.y + f4Vector.z * f4Vector.z);
		float fTheta	= 0.0f;
		float fPhi		= 0.0f;
		int iBin = 0;

		if( 0 < fLength )
		{
			f4Vector.x /= fLength;
			f4Vector.y /= fLength;
			f4Vector.z /= fLength;
			fTheta = ( 0.0f == f4Vector.x && 0.0f == f4Vector.y )?0.0f:(M_PI+(atan2(f4Vector.y, f4Vector.x)));
			fTheta /= 2.0f * M_PI;

			float fLength2D = sqrt(f4Vector.x * f4Vector.x + f4Vector.y * f4Vector.y);
			fPhi = ((0.0f == fLength2D)&&(0.0f == f4Vector.z))?0.0f:fabs(M_PI/2.0f-(atan2(f4Vector.z, fLength2D)));
			fPhi /= M_PI;
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
// ADD-BY-LEETEN 12/14/2009-END

////////////////////////////////////////////
__global__ 
static 
void 
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusion_kernel
// TO:
_FlowDiffusion3D_kernel
// MOD-BY-LEETEN 12/07/2009-END
(
	// INPUT
	float fAttenuation,

	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iZ,

	// ADD-BY-LEETEN 11/04/2009-BEGIN
	#if				!DIFFUSION_BY_FOR_LOOP_ON_HOST
	// ADD-BY-LEETEN 11/04/2009-END

	int iNrOfYBlocks,
	int iBlockZSize,

	// ADD-BY-LEETEN 11/04/2009-BEGIN
	#endif // #if	!DIFFUSION_BY_FOR_LOOP_ON_HOST
	// ADD-BY-LEETEN 11/04/2009-END

	// output
	cudaPitchedPtr cDstPitchedPtr,

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	cudaPitchedPtr cErrorPitchedPtr
	// ADD-BY-LEETEN 2009/11/25-END
)
{
 	int iVoxelX = blockIdx.x * blockDim.x + threadIdx.x;
	// ADD-BY-LEETEN 11/04/2009-BEGIN
	#if				DIFFUSION_BY_FOR_LOOP_ON_HOST
	// ADD-BY-LEETEN 11/04/2009-BEGIN

	int iVoxelY = blockIdx.y * blockDim.y + threadIdx.y;

	// ADD-BY-LEETEN 11/04/2009-BEGIN
	#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
	int iVoxelY = (blockIdx.y % iNrOfYBlocks) * blockDim.y + threadIdx.y;
	int iBeginZ = (blockIdx.y / iNrOfYBlocks) * iBlockZSize;
	int iEndZ = min(iBeginZ + iBlockZSize, iVolumeDepth);
	#endif	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
	// ADD-BY-LEETEN 11/04/2009-END

	#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
		// ADD-BY-LEETEN 11/04/2009-BEGIN
		float fAttenuationDividedBy6 = fAttenuation / 6.0f;
		// ADD-BY-LEETEN 11/04/2009-END
	#endif	// DEL-BY-LEETEN 12/07/2009-END

	// MOD-BY-LEETEN 11/04/2009-FROM:
		// #if	0	// MOD-BY-LEETEN 10/02/2009-FROM:
	// TO:
	#if	DIFFUSION_BY_FOR_LOOP_ON_HOST
	// MOD-BY-LEETEN 11/04/2009-END

		// compute the central differnece
		float4 f4Value	= tex2D(t2dSrc, iVoxelX,		iVoxelY + iZ * iVolumeHeight);
		float4 f4PX		= tex2D(t2dSrc, iVoxelX + 1,	iVoxelY + iZ * iVolumeHeight);
		float4 f4NX		= tex2D(t2dSrc, iVoxelX - 1,	iVoxelY + iZ * iVolumeHeight);
		float4 f4PY		= tex2D(t2dSrc, iVoxelX,		min(iVoxelY + 1, iVolumeHeight - 1) + iZ * iVolumeHeight);
		float4 f4NY		= tex2D(t2dSrc, iVoxelX,		max(iVoxelY - 1, 0)					+ iZ * iVolumeHeight);
		float4 f4PZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + min(iZ + 1, iVolumeDepth - 1) * iVolumeHeight);
		float4 f4NZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + max(iZ - 1, 0)				* iVolumeHeight);

	#if	0	// MOD-BY-LEETEN 11/04/2009-FROM:
	#else	// MOD-BY-LEETEN 11/04/2009-TO:
		float4 f4WeightOffset = tex3D(t3dWeightOffset, iVoxelX,	iVoxelY, iZ);

		#if	0	// MOD-BY-LEETEN 2009/11/25-FROM:
		#else	// MOD-BY-LEETEN 2009/11/25-TO:
		float4 f4Result;

		#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
			if( 0.0f != f4Value.w || 
				iZ == 0		 || iZ == iVolumeDepth - 1 || 
				iVoxelX == 0 || iVoxelX == iVolumeWidth - 1 ||
				iVoxelY == 0 || iVoxelY == iVolumeHeight - 1 )
				f4Result = f4Value;
			else
		#endif	// DEL-BY-LEETEN 12/07/2009-END

		#if	0	// MOD-BY-LEETEN 12/07/2009-FROM:
			f4Result = make_float4(
				f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuationDividedBy6 + f4WeightOffset.x,
				f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuationDividedBy6 + f4WeightOffset.y,
				f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuationDividedBy6 + f4WeightOffset.z,
				0);
		#else	// MOD-BY-LEETEN 12/07/2009-TO:
		f4Result = make_float4(
			f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuation + f4WeightOffset.x,
			f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuation + f4WeightOffset.y,
			f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuation + f4WeightOffset.z,
			0);
		#endif	// MOD-BY-LEETEN 12/07/2009-END

		if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
		{
			*ADDRESS_2D(
				float4, cDstPitchedPtr.ptr, 
				sizeof(float4), cDstPitchedPtr.pitch, 
				iVoxelX, iVoxelY + iZ * iVolumeHeight) = f4Result;

			float4 f4Diff;
			f4Diff.x = f4Value.x - f4Result.x;
			f4Diff.y = f4Value.y - f4Result.y;
			f4Diff.z = f4Value.z - f4Result.z;
			float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
			*ADDRESS_2D(
				float, cErrorPitchedPtr.ptr, 
				sizeof(float), cErrorPitchedPtr.pitch, 
				iVoxelX, iVoxelY + iZ * iVolumeHeight) = fDiff;
		}
	#endif	// MOD-BY-LEETEN 2009/11/25-END

	#endif	// MOD-BY-LEETEN 11/04/2009-END

	// MOD-BY-LEETEN 11/04/2009-FROM:
		// #else	// MOD-BY-LEETEN 10/02/2009-TO:
	// TO:
	#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
	// MOD-BY-LEETEN 11/04/2009-END

	float4 f4Value;
	float4 f4PX;
	float4 f4NX;
	float4 f4PY;
	float4 f4NY;
	float4 f4PZ;
	float4 f4NZ;

	#if	0	// MOD-BY-LEETEN 11/04/2009-FROM:
	#else	// MOD-BY-LEETEN 11/04/2009-TO:

	float4 *pf4DstPtr = ADDRESS_2D(
							float4, cDstPitchedPtr.ptr, 
							sizeof(float4), cDstPitchedPtr.pitch, 
							iVoxelX, iVoxelY + iBeginZ * iVolumeHeight);

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	float *pfErrorPtr = ADDRESS_2D(
							float, cErrorPitchedPtr.ptr, 
							sizeof(float), cErrorPitchedPtr.pitch, 
							iVoxelX, iVoxelY + iBeginZ * iVolumeHeight);
	// ADD-BY-LEETEN 2009/11/25-END

	#endif	// MOD-BY-LEETEN 11/04/2009-END

		#if	0	// MOD-BY-LEETEN 11/04/2009-FROM:
		#else	// MOD-BY-LEETEN 11/04/2009-TO:
		f4Value		= tex2D(t2dSrc, iVoxelX,		iVoxelY + iBeginZ				* iVolumeHeight);
		f4NZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + max(iBeginZ - 1, 0)	* iVolumeHeight);
		#endif	// MOD-BY-LEETEN 11/04/2009-END

		for(int z = iBeginZ; z < iEndZ; z++, f4NZ = f4Value, f4Value = f4PZ)
		{
			// ADD-BY-LEETEN 10/02/2009-BEGIN
			float4 f4WeightOffset = tex3D(t3dWeightOffset, iVoxelX,	iVoxelY, z);
			// ADD-BY-LEETEN 10/02/2009-END
			// f4Value		= tex2D(t2dSrc, iVoxelX,		iVoxelY + z * iVolumeHeight);
			#if	!USE_SHARED_MEMORY
			f4PX		= tex2D(t2dSrc, iVoxelX + 1,	iVoxelY + z * iVolumeHeight);
			f4NX		= tex2D(t2dSrc, iVoxelX - 1,	iVoxelY + z * iVolumeHeight);
			f4PY		= tex2D(t2dSrc, iVoxelX,		min(iVoxelY + 1, iVolumeHeight - 1) + z * iVolumeHeight);
			f4NY		= tex2D(t2dSrc, iVoxelX,		max(iVoxelY - 1, 0)					+ z * iVolumeHeight);

			#else	// #if	!USE_SHARED_MEMORY
			#endif	// #if	!USE_SHARED_MEMORY
			f4PZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + min(z + 1, iVolumeDepth - 1) * iVolumeHeight);
			// f4NZ		= tex2D(t2dSrc, iVoxelX,		iVoxelY + max(z - 1, 0)				* iVolumeHeight);

			// store the result back to the dst.
			#if	0	// MOD-BY-LEETEN 2009/11/10-FROM:
			#else	// MOD-BY-LEETEN 2009/11/10-TO:
			float4 f4Result;

			#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
				if( 0.0f != f4Value.w || 
					z == 0 || z == iVolumeDepth - 1 || 
					iVoxelX == 0 || iVoxelX == iVolumeWidth - 1 ||
					iVoxelY == 0 || iVoxelY == iVolumeHeight - 1 )
					f4Result = f4Value;
				else
			#endif	// DEL-BY-LEETEN 12/07/2009-END	

				#if	0	// MOD-BY-LEETEN 12/07/2009-FROM:
					f4Result = make_float4(
						f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuationDividedBy6 + f4WeightOffset.x,
						f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuationDividedBy6 + f4WeightOffset.y,
						f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuationDividedBy6 + f4WeightOffset.z,
						0);
				#else	// MOD-BY-LEETEN 12/07/2009-TO:
				f4Result = make_float4(	
					f4WeightOffset.w * f4Value.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0f * f4Value.x) * fAttenuation + f4WeightOffset.x,
					f4WeightOffset.w * f4Value.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0f * f4Value.y) * fAttenuation + f4WeightOffset.y,
					f4WeightOffset.w * f4Value.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0f * f4Value.z) * fAttenuation + f4WeightOffset.z,
					0.0);
				#endif	// MOD-BY-LEETEN 12/07/2009-END
			#endif	// MOD-BY-LEETEN 2009/11/10-TO:

			// ADD-BY-LEETEN 11/04/2009-BEGIN
			if( iVoxelX < iVolumeWidth && iVoxelY < iVolumeHeight )
			// ADD-BY-LEETEN 11/04/2009-END
			{
				pf4DstPtr[0] = f4Result;

				// ADD-BY-LEETEN 2009/11/25-BEGIN
				float4 f4Diff;
				f4Diff.x = f4Value.x - f4Result.x;
				f4Diff.y = f4Value.y - f4Result.y;
				f4Diff.z = f4Value.z - f4Result.z;
				float fDiff;
				// float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
				fDiff = max(abs(f4Diff.x), max(abs(f4Diff.y), abs(f4Diff.z)));
				pfErrorPtr[0] = fDiff;
				// ADD-BY-LEETEN 2009/11/25-END
			}

			pf4DstPtr += iVolumeHeight * cDstPitchedPtr.pitch / sizeof(float4);

			// ADD-BY-LEETEN 2009/11/25-BEGIN
			pfErrorPtr += iVolumeHeight * cErrorPitchedPtr.pitch / sizeof(float);
			// ADD-BY-LEETEN 2009/11/25-END
		}
	// DEL-BY-LEETEN 11/04/2009-BEGIN
		// }
	// DEL-BY-LEETEN 11/04/2009-END

	#endif	// MOD-BY-LEETEN 10/02/2009-END
}


extern "C" {

// MOD-BY-LEETEN 12/07/2009-FROM:
	// void _FlowFusionFree()
// TO:
void 
_FlowDiffusionFree()
// MOD-BY-LEETEN 12/07/2009-END
{
	FREE_MEMORY_ON_HOST(pf4Volume_host);
	for(int i = 0; i < 2; i++)
		FREE_MEMORY(pcVolumePtrs_global[i].ptr);
	FREE_ARRAY(cWeightOffsetVolume_array);
	FREE_MEMORY_ON_HOST(pf4WeightOffsetVolume_host);
	// ADD-BY-LEETEN 2009/11/25-BEGIN
	FREE_MEMORY(cErrorVolume_device.ptr);
	FREE_MEMORY(cErrorSum_device.ptr);

	#if	USE_CUDPP
	cudppDestroyPlan(cScanplan);
	#endif	// #if	USE_CUDPP
	// ADD-BY-LEETEN 2009/11/25-END

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	FREE_ARRAY(pcAngleMap_array);	

	FREE_MEMORY(cSrcBinVolumePtr_global.ptr);
	FREE_MEMORY(cDstBinVolumePtr_global.ptr);
	FREE_MEMORY_ON_HOST(piSrcAngleBinVolume_host);
	FREE_MEMORY_ON_HOST(piDstAngleBinVolume_host);
	// ADD-BY-LEETEN 12/14/2009-END
}

void
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusionInit(
// TO:
_FlowDiffusionInit(
// MOD-BY-LEETEN 12/07/2009-END
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth
)
{
	// MOD-BY-LEETEN 12/07/2009-FROM:
		// CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, "_FlowFusionInit(): ");
	// TO:
	CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, __FUNCTION__ ": ");
	// MOD-BY-LEETEN 12/07/2009-END

	// MOD-BY-LEETEN 12/14/2009-FROM:
		// int iNrOfVoxels = iVolumeWidth * iVolumeHeight * iVolumeDepth;
	// TO:
	iNrOfVoxels = iVolumeWidth * iVolumeHeight * iVolumeDepth;
	// MOD-BY-LEETEN 12/14/2009-END

	cVolumeExtent	= make_cudaExtent(
		iVolumeWidth * sizeof(float4),
		iVolumeHeight,
		iVolumeDepth);
	// allocate a local copy in the CPU side
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&pf4Volume_host,
			sizeof(pf4Volume_host[0]) * iNrOfVoxels) );
	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// Allocate 2 volumes on the  GPU's global memory.
	// in each iteration, one of them will be used as the source
	// 3D texture, and the other will be served as the dst.
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

	for(int i = 0; i < 2; i++)
	{
		CUDA_SAFE_CALL( 
			cudaMalloc3D(&pcVolumePtrs_global[i], cVolumeExtent) );
	}

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	cErrorVolumeExtent	= make_cudaExtent(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth);
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cErrorVolume_device,	cErrorVolumeExtent) );
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cErrorSum_device,		cErrorVolumeExtent) );

	#if	USE_CUDPP
	// cConfig.op = CUDPP_ADD;
	cConfig.op = CUDPP_MAX;
	cConfig.datatype = CUDPP_FLOAT;
	cConfig.algorithm = CUDPP_SCAN;
	cConfig.options = CUDPP_OPTION_FORWARD | CUDPP_OPTION_INCLUSIVE;

	cudppPlan(&cScanplan, cConfig, iVolumeWidth * iVolumeHeight * iVolumeDepth, iVolumeHeight * iVolumeDepth, cErrorVolume_device.pitch);  
	#endif	// #if	USE_CUDPP
	// ADD-BY-LEETEN 2009/11/25-END

	// ADD-BY-LEETEN 10/02/2009-BEGIN
	cVolumeExtent_array = make_cudaExtent(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth);

	CUDA_SAFE_CALL( 
		cudaMalloc3DArray(&cWeightOffsetVolume_array, &cWeightOffsetChannelDesc, cVolumeExtent_array) );

	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&pf4WeightOffsetVolume_host,
			sizeof(pf4WeightOffsetVolume_host[0]) * iNrOfVoxels) );
	// ADD-BY-LEETEN 10/02/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	CLOCK_PRINT(PRINT_FLOW_FUSION_TIMING);
}

// ADD-BY-LEETEN 12/14/2009-BEGIN
void 
_FlowDiffusionSetAngleMap(int *piAngleMap, int iNrOfPhis, int iNrOfThetas)
{
	// allocate an array on the GPU side

	// upload the angle map to the araray

	// setup the texture for the angle map
	/*
    t2dAngleMap.addressMode[0] =	cudaAddressModeClamp;
    t2dAngleMap.addressMode[1] =	cudaAddressModeClamp;
    t2dAngleMap.filterMode =		cudaFilterModePoint;
    t2dAngleMap.normalized =		true;
	*/
	SETUP_ARRAY(pcAngleMap_array, iNrOfPhis, iNrOfThetas, 32, 0, 0, 0, cudaChannelFormatKindSigned);	
	BIND_ARRAY_AS_TEXTURE(
		t2dAngleMap, pcAngleMap_array, 
		cudaAddressModeClamp, cudaAddressModeClamp, cudaFilterModePoint, true);	

    CUDA_SAFE_CALL( 
		cudaMemcpy2DToArray(
			pcAngleMap_array, 
			0, 
			0, 
			(void*)piAngleMap, 
			iNrOfPhis * sizeof(piAngleMap), 
			iNrOfPhis * sizeof(piAngleMap), 
			iNrOfThetas, 
			cudaMemcpyHostToDevice) );

	// check whether the volume extent has been setup
	assert( NULL != pcVolumePtrs_global[0].ptr );

	// allocate the volume of bins in the global memorty on GPUs
	cBinVolumeExtent	= make_cudaExtent(
		cVolumeExtent_array.width * sizeof(int),
		cVolumeExtent_array.height,
		cVolumeExtent_array.depth);
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cSrcBinVolumePtr_global, cBinVolumeExtent) );
	CUDA_SAFE_CALL( 
		cudaMalloc3D(&cDstBinVolumePtr_global, cBinVolumeExtent) );

	// allocate the volume of bins in the host side
	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&piSrcAngleBinVolume_host,
			sizeof(piSrcAngleBinVolume_host[0]) * iNrOfVoxels) );

	CUDA_SAFE_CALL(
		cudaMallocHost(
			(void**)&piDstAngleBinVolume_host,
			sizeof(piDstAngleBinVolume_host[0]) * iNrOfVoxels) );

	// setup the src. texture for bin lookup
	t2dBinVolumeSrc.addressMode[0] = cudaAddressModeClamp;
	t2dBinVolumeSrc.addressMode[1] = cudaAddressModeClamp;
	t2dBinVolumeSrc.filterMode =	cudaFilterModePoint;
	t2dBinVolumeSrc.normalized =	false;
}

void
_Vector3DToVolume(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	cudaPitchedPtr cVolumePtr_global,	// input
	cudaPitchedPtr cBinVolumePtr_global	// output
)
{
	CUDA_SAFE_CALL_NO_SYNC(
		cudaBindTexture2D(
			0, 
			t2dBinVolumeSrc, 
			cVolumePtr_global.ptr, 
			cudaCreateChannelDesc<float4>(),
			iVolumeWidth, 
			iVolumeHeight * iVolumeDepth, 
			cVolumePtr_global.pitch) );

	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	dim3 v3Grid = dim3(
		(unsigned int)ceilf((float)iVolumeWidth	 / (float)v3Blk.x),
		unsigned int(ceilf( float(iVolumeHeight) / float(v3Blk.y) ) ) *
		unsigned int(ceilf( float(iVolumeDepth) /  float(iBlockZSize) ) ) );

	_Vector3DToVolume_kernel<<<v3Grid, v3Blk, 0>>>
	(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth,
		int(ceilf(float(iVolumeHeight) / float(BLOCK_DIM_Y))),
		iBlockZSize,
		cBinVolumePtr_global
	);
	CUT_CHECK_ERROR("_Vector3DToVolume_kernel() failed");
}

void
_ComputeSrcBinVolume
(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	float *pfSrcVolume
)
{
	CLOCK_INIT(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, __FUNCTION__ ": ");

	CLOCK_BEGIN(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pf4Volume_host[v].x = pfSrcVolume[v * 3 + 0];
				pf4Volume_host[v].y = pfSrcVolume[v * 3 + 1];
				pf4Volume_host[v].z = pfSrcVolume[v * 3 + 2];
				pf4Volume_host[v].w = 0.0f;
			}
	CLOCK_END(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, false);

	CLOCK_BEGIN(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
    cudaMemcpy3DParms cCopyParamsHostToDevice = {0};

    cCopyParamsHostToDevice.srcPtr   = make_cudaPitchedPtr(
		(void*)pf4Volume_host, 
		cVolumeExtent.width, 
		iVolumeWidth * sizeof(float4), 
		iVolumeHeight);
    cCopyParamsHostToDevice.dstPtr		= pcVolumePtrs_global[0];
    cCopyParamsHostToDevice.extent		= cVolumeExtent;
    cCopyParamsHostToDevice.kind		= cudaMemcpyHostToDevice;
    CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsHostToDevice) );  

	CLOCK_END(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, false);

	CLOCK_BEGIN(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
	_Vector3DToVolume(
		iVolumeWidth,
		iVolumeHeight,
		iVolumeDepth,
		iBlockZSize,
		pcVolumePtrs_global[0],
		cSrcBinVolumePtr_global);
	CLOCK_END(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING, false);

	CLOCK_PRINT(SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING);
}

void 
_GetSrcBinVolume(int *piBinVolume)
{
	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr		= cSrcBinVolumePtr_global;
	cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		(void*)piSrcAngleBinVolume_host, 
		cVolumeExtent_array.width * sizeof(int),	// cSrcBinVolumePtr_global.width, 
		cVolumeExtent_array.width * sizeof(int), 
		cVolumeExtent_array.height);
	cCopyParamsDeviceToHost.extent		= cBinVolumeExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	memcpy(piBinVolume, piSrcAngleBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);
}

void 
_GetDstBinVolume(int *piBinVolume)
{
	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr		= cDstBinVolumePtr_global;
	cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		(void*)piDstAngleBinVolume_host, 
		cVolumeExtent_array.width * sizeof(int),	// cDstBinVolumePtr_global.width, 
		cVolumeExtent_array.width * sizeof(int), 
		cVolumeExtent_array.height);
	cCopyParamsDeviceToHost.extent		= cBinVolumeExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	memcpy(piBinVolume, piDstAngleBinVolume_host, sizeof(piBinVolume) * iNrOfVoxels);
}

// ADD-BY-LEETEN 12/14/2009-END

void
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusion(
// TO:
_FlowDiffusion(
// MOD-BY-LEETEN 12/07/2009-END
	float fAttenuation,
	int iNrOfIterations,
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	// ADD-BY-LEETEN 10/02/2009-BEGIN
	int iBlockZSize,
	float *pfWeightVolume,
	// MOD-BY-LEETEN 2009/11/10-FROM:
		// float *pfOffsetVolume,
	// TO:
	float *pfXOffsetVolume,
	float *pfYOffsetVolume,
	float *pfZOffsetVolume,
	// MOD-BY-LEETEN 2009/11/10-END
	// ADD-BY-LEETEN 10/02/2009-END
	float *pfSrcVolume,
	float *pfDstVolume,
	// ADD-BY-LEETEN 2009/11/10-BEGIN
	int *piFlagVolume
	// ADD-BY-LEETEN 2009/11/10-END
)
{
	// MOD-BY-LEETEN 12/07/2009-FROM:
		// CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, "_FlowFusion(): ");
	// TO:
	// MOD-BY-LEETEN 12/14/2009-FROM:
		// CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, __FUNCTION__);
	// TO:
	CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, __FUNCTION__ ": ");
	// MOD-BY-LEETEN 12/07/2009-END

	#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
		// ADD-BY-LEETEN 2009/11/10-BEGIN
		fAttenuation *= 6.0f;
		// ADD-BY-LEETEN 2009/11/10-END
	#endif	// DEL-BY-LEETEN 12/07/2009-END

	// MOD-BY-LEETEN 10/02/2009-FROM:
		// dim3 v3Blk = dim3(16, 8);
	// TO:
	dim3 v3Blk = dim3(BLOCK_DIM_X, BLOCK_DIM_Y);
	// MOD-BY-LEETEN 10/02/2009-END
	dim3 v3Grid = dim3(
		(unsigned int)ceilf((float)iVolumeWidth	 / (float)v3Blk.x),
		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#if					DIFFUSION_BY_FOR_LOOP_ON_HOST	
		// ADD-BY-LEETEN 11/04/2009-END

		(unsigned int)ceilf((float)iVolumeHeight / (float)v3Blk.y));

		// ADD-BY-LEETEN 11/04/2009-BEGIN
		#else	// #if		DIFFUSION_BY_FOR_LOOP_ON_HOST
		unsigned int(ceilf( float(iVolumeHeight) / float(v3Blk.y) ) ) *
		unsigned int(ceilf( float(iVolumeDepth) /  float(iBlockZSize) ) ) );
		#endif	// #if		DIFFUSION_BY_FOR_LOOP_ON_HOST
		// ADD-BY-LEETEN 11/04/2009-END

	// convert the #channels in the src volume from 3 to 4
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pf4Volume_host[v].x = pfSrcVolume[v * 3 + 0];
				pf4Volume_host[v].y = pfSrcVolume[v * 3 + 1];
				pf4Volume_host[v].z = pfSrcVolume[v * 3 + 2];

				// ADD-BY-LEETEN 2009/11/10-BEGIN
				if( piFlagVolume )
					pf4Volume_host[v].w = float(piFlagVolume[v]);
				else
				// ADD-BY-LEETEN 2009/11/10-END
				pf4Volume_host[v].w = 0.0f;
			}

	// ADD-BY-LEETEN 10/02/2009-BEGIN
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				#if	0	// MOD-BY-LEETEN 2009/11/10-FROM:
				#else	// MOD-BY-LEETEN 2009/11/10-TO:
				// ADD-BY-LEETEN 12/07/2009-BEGIN
				if( pfXOffsetVolume )
				// ADD-BY-LEETEN 12/07/2009-END
				pf4WeightOffsetVolume_host[v].x = pfXOffsetVolume[v];
				// ADD-BY-LEETEN 12/07/2009-BEGIN
				if( pfYOffsetVolume )
				// ADD-BY-LEETEN 12/07/2009-END
				pf4WeightOffsetVolume_host[v].y = pfYOffsetVolume[v];
				// ADD-BY-LEETEN 12/07/2009-BEGIN
				if( pfZOffsetVolume )
				// ADD-BY-LEETEN 12/07/2009-END
				pf4WeightOffsetVolume_host[v].z = pfZOffsetVolume[v];
				pf4WeightOffsetVolume_host[v].w = 1.0f - pfWeightVolume[v];
				#endif	// MOD-BY-LEETEN 2009/11/10-END
			}
	// ADD-BY-LEETEN 10/02/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// Upload the input volume to the first volume on the GPU
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

    cudaMemcpy3DParms cCopyParamsHostToDevice = {0};

    cCopyParamsHostToDevice.srcPtr   = make_cudaPitchedPtr(
		(void*)pf4Volume_host, 
		cVolumeExtent.width, 
		iVolumeWidth * sizeof(float4), 
		iVolumeHeight);
    cCopyParamsHostToDevice.dstPtr		= pcVolumePtrs_global[0];
    cCopyParamsHostToDevice.extent		= cVolumeExtent;
    cCopyParamsHostToDevice.kind		= cudaMemcpyHostToDevice;
    CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsHostToDevice) );  

	// ADD-BY-LEETEN 10/02/2009-BEGIN
    cudaMemcpy3DParms cCopyParamsHostToArray = {0};
    cCopyParamsHostToArray.srcPtr   = make_cudaPitchedPtr(
		(void*)pf4WeightOffsetVolume_host, 
		iVolumeWidth * sizeof(float4), 
		iVolumeWidth, 
		iVolumeHeight);
    cCopyParamsHostToArray.dstArray = cWeightOffsetVolume_array;
    cCopyParamsHostToArray.extent   = cVolumeExtent_array;
    cCopyParamsHostToArray.kind     = cudaMemcpyHostToDevice;
    CUDA_SAFE_CALL( 
		cudaMemcpy3D(&cCopyParamsHostToArray) );  

    t3dWeightOffset.addressMode[0] = cudaAddressModeClamp;
    t3dWeightOffset.addressMode[1] = cudaAddressModeClamp;
    t3dWeightOffset.addressMode[2] = cudaAddressModeClamp;
    t3dWeightOffset.filterMode		= cudaFilterModePoint;
    t3dWeightOffset.normalized		= false; 

    // bind array to 3D texture
    CUDA_SAFE_CALL(
		cudaBindTextureToArray(t3dWeightOffset, cWeightOffsetVolume_array, cWeightOffsetChannelDesc));
	// ADD-BY-LEETEN 10/02/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// calcuate the fusion operator on GPUs
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

    t2dSrc.addressMode[0] = cudaAddressModeClamp;
    t2dSrc.addressMode[1] = cudaAddressModeClamp;
    t2dSrc.filterMode =		cudaFilterModePoint;
    t2dSrc.normalized =		false;

	int iSrc = 0;

	for(int i = 0; 
			i < iNrOfIterations; 
			i++,				iSrc = 1 - iSrc)
	{
		// bind the src. volume as the 3D texture

		cudaChannelFormatDesc cChannelDesc =
			cudaCreateChannelDesc<float4>();

		CUDA_SAFE_CALL_NO_SYNC(
			cudaBindTexture2D(
				0, 
				t2dSrc, 
				pcVolumePtrs_global[iSrc].ptr, 
				cChannelDesc,
				iVolumeWidth, 
				iVolumeHeight * iVolumeDepth, 
				pcVolumePtrs_global[iSrc].pitch) );


		// ADD-BY-LEETEN 12/07/2009-BEGIN
		if( iVolumeDepth <= 2 )
			_FlowDiffusion2D_kernel<<<v3Grid, v3Blk, 0>>>
			(
				fAttenuation,
				iVolumeWidth,
				iVolumeHeight,
				pcVolumePtrs_global[1 - iSrc],
				cErrorVolume_device
			);	
		else
		{
		// ADD-BY-LEETEN 12/07/2009-END

		// launch the kernel to compute the diffusion operator for 1 iteration
		// MOD-BY-LEETEN 11/04/2009-FROM:
			// #if	0	// MOD-BY-LEETEN 10/02/2009-FROM:
		// TO:
		#if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		// MOD-BY-LEETEN 11/04/2009-END

			for(int z = 0; z < iVolumeDepth; z++)
				// MOD-BY-LEETEN 12/07/2009-FROM:
					// _FlowFusion_kernel<<<v3Grid, v3Blk, 0>>>
				// TO:
				_FlowDiffusion3D_kernel<<<v3Grid, v3Blk, 0>>>
				// MOD-BY-LEETEN 12/07/2009-END
				(
					fAttenuation,
					iVolumeWidth,
					iVolumeHeight,
					iVolumeDepth,
					z,

					// output
					pcVolumePtrs_global[1 - iSrc],
					// ADD-BY-LEETEN 2009/11/25-BEGIN
					cErrorVolume_device
					// ADD-BY-LEETEN 2009/11/25-END
				);	
		// MOD-BY-LEETEN 11/04/2009-FROM:
			// #else	// MOD-BY-LEETEN 10/02/2009-TO:
		// TO:
		#else	// #if	DIFFUSION_BY_FOR_LOOP_ON_HOST
		// MOD-BY-LEETEN 11/04/2009-END

		// _FlowFusion_kernel<<<v3Grid, v3Blk, v3Blk.x * v3Blk.y * sizeof(float4)>>>
		// MOD-BY-LEETEN 12/07/2009-FROM:
			// _FlowFusion_kernel<<<v3Grid, v3Blk, 0>>>
		// TO:
		_FlowDiffusion3D_kernel<<<v3Grid, v3Blk, 0>>>
		// MOD-BY-LEETEN 12/07/2009-END
		(
			fAttenuation,
			iVolumeWidth,
			iVolumeHeight,
			iVolumeDepth,
			0,
			// ADD-BY-LEETEN 11/04/2009-BEGIN
			int(ceilf(float(iVolumeHeight) / float(BLOCK_DIM_Y))),
			iBlockZSize,
			// ADD-BY-LEETEN 11/04/2009-END
			// output
			pcVolumePtrs_global[1 - iSrc],
			// ADD-BY-LEETEN 2009/11/25-BEGIN
			cErrorVolume_device
			// ADD-BY-LEETEN 2009/11/25-END
		);
		#endif	// MOD-BY-LEETEN 10/02/2009-END
		// ADD-BY-LEETEN 12/07/2009-BEGIN
		}
		// ADD-BY-LEETEN 12/07/2009-END
		CUT_CHECK_ERROR("_FlowFusion_kernel() failed");

		// ADD-BY-LEETEN 2009/11/25-BEGIN
		#if	USE_CUDPP
		cudppScan(cScanplan, cErrorSum_device.ptr, cErrorVolume_device.ptr, iVolumeWidth * iVolumeHeight * iVolumeDepth);

		float fError;
	    CUDA_SAFE_CALL( 
			cudaMemcpy(
				&fError, 
				ADDRESS_2D(
					float, cErrorSum_device.ptr, 
					sizeof(float), cErrorSum_device.pitch, 
					iVolumeWidth - 1, iVolumeHeight - 1 + (iVolumeDepth - 1 ) * iVolumeHeight),
				sizeof(float), 
				cudaMemcpyDeviceToHost));

		/*
		fError /= float(iVolumeWidth * iVolumeHeight * iVolumeDepth * 3);
		fError = sqrtf(fError);
		*/
		fprintf(stderr, "Error: %f\n", fError);

		if( fError < 1e-10 )
			break;
		#endif	// #if	USE_CUDPP
		// ADD-BY-LEETEN 2009/11/25-END

	}

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	if( iVolumeDepth > 2 )
	{
		_Vector3DToVolume(
			iVolumeWidth,
			iVolumeHeight,
			iVolumeDepth,
			iBlockZSize,
			pcVolumePtrs_global[iSrc],
			cDstBinVolumePtr_global);
			// piDstAngleBinVolume_host);
	}
	// ADD-BY-LEETEN 12/14/2009-END

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	int iDst = iSrc;

	// read the result from latest iteration back to the CPU side
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);

	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
    cCopyParamsDeviceToHost.srcPtr		= pcVolumePtrs_global[iDst];
    cCopyParamsDeviceToHost.dstPtr		= make_cudaPitchedPtr(
		(void*)pf4Volume_host, 
		cVolumeExtent.width, 
		iVolumeWidth * sizeof(float4), 
		iVolumeHeight);
    cCopyParamsDeviceToHost.extent		= cVolumeExtent;
    cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;

	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  

	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	// convert the #channels in the dst volume from 4 to 3
	CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
	for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
		for(int			h = 0; h < iVolumeHeight;	h++)
			for(int		w = 0; w < iVolumeWidth;	w++, v++)
			{
				pfDstVolume[v * 3 + 0] = pf4Volume_host[v].x;
				pfDstVolume[v * 3 + 1] = pf4Volume_host[v].y;
				pfDstVolume[v * 3 + 2] = pf4Volume_host[v].z;
			}
	CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

	CLOCK_PRINT(PRINT_FLOW_FUSION_TIMING);
}

#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
	void
	_FlowFusionCPU(
		float fAttenuation,
		int iNrOfIterations,
		int iVolumeWidth,
		int iVolumeHeight,
		int iVolumeDepth,
		// ADD-BY-LEETEN 10/02/2009-BEGIN
		float *pfWeightVolume,
		float *pfOffsetVolume,
		// ADD-BY-LEETEN 10/02/2009-END
		float *pfSrcVolume,
		float *pfDstVolume
	)
	{
		CLOCK_INIT(PRINT_FLOW_FUSION_TIMING, "_FlowFusionCPU(): ");

		CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
		int iNrOfVoxels = iVolumeWidth * iVolumeHeight * iVolumeDepth;

		TBuffer<float> ppfTempVolume[2];
		for(int i = 0; i < 2; i++)
			ppfTempVolume[i].alloc(3 * iNrOfVoxels);
		memcpy(
			&ppfTempVolume[0][0], 
			&pfSrcVolume[0], 
			sizeof(ppfTempVolume[0][0]) * ppfTempVolume[0].USize());
		CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

		// convert the #channels in the src volume from 3 to 4
		CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
		int iSrc = 0;
		for(int i = 0; 
				i < iNrOfIterations; 
				i++,				iSrc = 1 - iSrc)
		{
			int iDst = 1 - iSrc;
			for(int v = 0,		d = 0; d < iVolumeDepth;	d++)
				for(int			h = 0; h < iVolumeHeight;	h++)
					for(int		w = 0; w < iVolumeWidth;	w++, v++)
					{
						for(int c = 0; c < 3; c++)
						{
							float fV =	ppfTempVolume[iSrc][c + 3 * v];
							#if	0	// MOD-BY-LEETEN 10/02/2009-FROM:
								float fPX = ppfTempVolume[iSrc][c + 3 * (min(w+1, iVolumeWidth-1) + h * iVolumeWidth + d * iVolumeWidth * iVolumeHeight)];
								float fNX = ppfTempVolume[iSrc][c + 3 * (max(w-1, 0) + h * iVolumeWidth + d * iVolumeWidth * iVolumeHeight)];
								float fPY = ppfTempVolume[iSrc][c + 3 * (w + min(h+1, iVolumeHeight-1) * iVolumeWidth + d * iVolumeWidth * iVolumeHeight)];
								float fNY = ppfTempVolume[iSrc][c + 3 * (w + max(h-1, 0) * iVolumeWidth + d * iVolumeWidth * iVolumeHeight)];
								float fPZ = ppfTempVolume[iSrc][c + 3 * (w + h * iVolumeWidth + min(d+1, iVolumeDepth-1) * iVolumeWidth * iVolumeHeight)];
								float fNZ = ppfTempVolume[iSrc][c + 3 * (w + h * iVolumeWidth + max(d-1, 0) * iVolumeWidth * iVolumeHeight)];
							#else	// MOD-BY-LEETEN 10/02/2009-TO:
							float fPX = ppfTempVolume[iSrc][c + 3 * (v + ((w+1<iVolumeWidth)?1:0))];
							float fNX = ppfTempVolume[iSrc][c + 3 * (v - ((w-1>=0)?1:0))];
							float fPY = ppfTempVolume[iSrc][c + 3 * (v + ((h+1<iVolumeHeight)?iVolumeWidth:0))];
							float fNY = ppfTempVolume[iSrc][c + 3 * (v - ((h-1>=0)?iVolumeWidth:0))];
							float fPZ = ppfTempVolume[iSrc][c + 3 * (v + ((d+1<iVolumeDepth) ? (iVolumeWidth * iVolumeHeight) : 0))];
							float fNZ = ppfTempVolume[iSrc][c + 3 * (v - ((d-1>=0) ? (iVolumeWidth * iVolumeHeight) : 0))];
							#endif	// MOD-BY-LEETEN 10/02/2009-END
							ppfTempVolume[iDst][c + 3 * v] = 
								// MOD-BY-LEETEN 10/02/2009-FROM:
									// fV + (fPX + fNX + fPY + fNY + fPZ + fNZ - 6.0f * fV) * fAttenuation / 6.0f;
								// TO:
								pfWeightVolume[v] * fV + (fPX + fNX + fPY + fNY + fPZ + fNZ - 6.0f * fV) * fAttenuation / 6.0f + pfOffsetVolume[c + 3 * v];
								// MOD-BY-LEETEN 10/02/2009-END
						}
					}
		}
		CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

		CLOCK_BEGIN(PRINT_FLOW_FUSION_TIMING);
		int iDst = iSrc;
		memcpy(
			&pfDstVolume[0], 
			&ppfTempVolume[iDst][0], 
			sizeof(ppfTempVolume[iDst][0]) * ppfTempVolume[iDst].USize());
		CLOCK_END(PRINT_FLOW_FUSION_TIMING, true);

		CLOCK_PRINT(PRINT_FLOW_FUSION_TIMING);
	}
#endif	// DEL-BY-LEETEN 12/07/2009-END

} // extern "C"

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2009/12/07 20:04:02  leeten

[12/07/2009]
1. [1ST] First time checkin.

Revision 1.3  2009/12/05 21:19:51  leeten

[12/05/2009]
1. [ADD] Add a 3D texture glWeightOffset to store the weights and offsets.
2. [ADD] Specify the flag donochange to the w channel in the 3D textures.
3. [ADD] Output the error to a 3D volume.

Revision 1.2  2009/11/04 19:11:13  leeten

[2009/11/04]
1. [ADD] Add a new preprocessor USE_SHARED_MEMORY to decide whether the shared memory is utilized.
2. [ADD] Add a new preprocessor DIFFUSION_BY_FOR_LOOP_ON_HOSTto decide whether the diffusion is executed via a for loop on the host side to scan throught all XY layers.
If it is not zero, the volume will be divided along the Z direction as well, and each block will be assigned as a single block on CUDA.
3. [ADD] Add 2 new preprocessors BLOCK_DIM_X and BLOCK_DIM_Y to control the block dimension.
4. [ADD] Add 1 array and 1 3D textrue for the weight/offset volume.

Revision 1.1.1.1  2009/11/02 15:30:56  leeten

[11/02/2009]
1 [1ST] Firs time checkin.


*/
