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
			iVoxelX, iVoxelY) = fDiff;
		// ADD-BY-LEETEN 12/16/2009-BEGIN
		#endif	// #if	CHECK_ERROR_CONVERGENCE_BY_CUDPP
		// ADD-BY-LEETEN 12/16/2009-END
	}
}

/*

$Log: not supported by cvs2svn $

*/
