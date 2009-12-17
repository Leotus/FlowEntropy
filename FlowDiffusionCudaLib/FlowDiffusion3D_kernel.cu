#include "FlowDiffusion_cuda.h"

__global__ 
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

	#if	DIFFUSION_BY_FOR_LOOP_ON_HOST

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

			// ADD-BY-LEETEN 12/16/2009-BEGIN
			#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
			// ADD-BY-LEETEN 12/16/2009-END
			float4 f4Diff;
			f4Diff.x = f4Value.x - f4Result.x;
			f4Diff.y = f4Value.y - f4Result.y;
			f4Diff.z = f4Value.z - f4Result.z;
			float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
			*ADDRESS_2D(
				float, cErrorPitchedPtr.ptr, 
				sizeof(float), cErrorPitchedPtr.pitch, 
				iVoxelX, iVoxelY + iZ * iVolumeHeight) = fDiff;
			// ADD-BY-LEETEN 12/16/2009-BEGIN
			#endif	// CHECK_ERROR_CONVERGENCE_BY_CUDPP	
			// ADD-BY-LEETEN 12/16/2009-END
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
	// ADD-BY-LEETEN 12/16/2009-BEGIN
	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
	// ADD-BY-LEETEN 12/16/2009-END
	float *pfErrorPtr = ADDRESS_2D(
							float, cErrorPitchedPtr.ptr, 
							sizeof(float), cErrorPitchedPtr.pitch, 
							iVoxelX, iVoxelY + iBeginZ * iVolumeHeight);
	// ADD-BY-LEETEN 12/16/2009-BEGIN
	#endif	// CHECK_ERROR_CONVERGENCE_BY_CUDPP	
	// ADD-BY-LEETEN 12/16/2009-END

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

				// ADD-BY-LEETEN 12/16/2009-BEGIN
				#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
				// ADD-BY-LEETEN 12/16/2009-END

				// ADD-BY-LEETEN 2009/11/25-BEGIN
				float4 f4Diff;
				f4Diff.x = f4Value.x - f4Result.x;
				f4Diff.y = f4Value.y - f4Result.y;
				f4Diff.z = f4Value.z - f4Result.z;
				float fDiff;
				// float fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
				// MOD-BY-LEETEN 2009/12/17-FROM:
					// fDiff = max(abs(f4Diff.x), max(abs(f4Diff.y), abs(f4Diff.z)));
				// TO:
				fDiff = f4Diff.x * f4Diff.x + f4Diff.y * f4Diff.y + f4Diff.z * f4Diff.z;
				// MOD-BY-LEETEN 2009/12/17-END
				pfErrorPtr[0] = fDiff;
				// ADD-BY-LEETEN 2009/11/25-END
				// ADD-BY-LEETEN 12/16/2009-BEGIN
				#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
				// ADD-BY-LEETEN 12/16/2009-END
			}

			pf4DstPtr += iVolumeHeight * cDstPitchedPtr.pitch / sizeof(float4);

			// ADD-BY-LEETEN 12/16/2009-BEGIN
			#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
			// ADD-BY-LEETEN 12/16/2009-END
			// ADD-BY-LEETEN 2009/11/25-BEGIN
			pfErrorPtr += iVolumeHeight * cErrorPitchedPtr.pitch / sizeof(float);
			// ADD-BY-LEETEN 2009/11/25-END
			// ADD-BY-LEETEN 12/16/2009-BEGIN
			#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
			// ADD-BY-LEETEN 12/16/2009-END
		}

	#endif	// MOD-BY-LEETEN 10/02/2009-END
}

/*

$Log: not supported by cvs2svn $

*/

