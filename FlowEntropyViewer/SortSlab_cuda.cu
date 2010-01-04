#include <stdio.h>

#include <assert.h>

#include "cudpp/cudpp.h"

#if	 __DEVICE_EMULATION__ 
#pragma comment (lib, "cudpp32d_emu.lib")
#else
#pragma comment (lib, "cudpp32.lib")
#endif

#include "cuda_macro.h"
#pragma comment (lib, "cutil32.lib ")      // link with my own library libfps

#pragma comment (lib, "cudart.lib")      // link with my own library libfps


static float pfModelViewMatrix_host[16];
static float pfProjectionMatrix_host[16];
static __constant__ float pfModelViewMatrix_constant[16];
static __constant__ float pfProjectionMatrix_constant[16];
static float4 *pf4LineCentroids_global;
static int *piSlabs_global;
static int *piLines_global;
static int *piSlabs_host;
static int *piLines_host;
static CUDPPHandle hScanPlan = 0;

__device__
float4 
F4MatrixDotVector
(
	float pfMatrix[],
	float4 f4Vector
)
{
	return make_float4(
		pfMatrix[0] * f4Vector.x + pfMatrix[4] * f4Vector.y + pfMatrix[8] * f4Vector.z + pfMatrix[12] * f4Vector.w,
		pfMatrix[1] * f4Vector.x + pfMatrix[5] * f4Vector.y + pfMatrix[9] * f4Vector.z + pfMatrix[13] * f4Vector.w,
		pfMatrix[2] * f4Vector.x + pfMatrix[6] * f4Vector.y + pfMatrix[10] * f4Vector.z + pfMatrix[14] * f4Vector.w,
		pfMatrix[3] * f4Vector.x + pfMatrix[7] * f4Vector.y + pfMatrix[11] * f4Vector.z + pfMatrix[15] * f4Vector.w
		);
}

__global__
void
_ComputeDepth_kernel
(
	float	fMinZ,
	float	fMaxZ,
	int		iNrOfSlabs,
	int		iNrOfLines,
	float4	pf4LineCentroids_global[],
	int		piSlabs_global[],
	int		piLines_global[]
)
{
	int iL = threadIdx.x + blockDim.x * blockIdx.x;	// index to the assigned line segment

	if( iL < iNrOfLines )
	{
		float4 f4Centroid_obj = pf4LineCentroids_global[iL];
		float4 f4Centroid_eye = F4MatrixDotVector(pfModelViewMatrix_constant,	f4Centroid_obj);
		float fDepth = f4Centroid_eye.z / f4Centroid_eye.w;

		int iSlab = int(float(iNrOfSlabs) *  (fDepth - fMinZ) / (fMaxZ - fMinZ));
		iSlab = min(max(iSlab, 0), iNrOfSlabs - 1);

		piSlabs_global[iL] = iSlab; 
		piLines_global[iL] = iL; 
	}
}

extern "C" {
void 
_ComputeDepthInit_cuda
(
	int iNrOfLines,
	float pfLineCentroids[]
)
{
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&piSlabs_global, sizeof(piSlabs_global[0]) * iNrOfLines) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&piLines_global, sizeof(piLines_global[0]) * iNrOfLines) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMalloc(	(void**)&pf4LineCentroids_global, sizeof(pf4LineCentroids_global[0]) * iNrOfLines) );

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocHost(	(void**)&piSlabs_host, sizeof(piSlabs_host[0]) * iNrOfLines) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocHost(	(void**)&piLines_host, sizeof(piLines_host[0]) * iNrOfLines) );

	float4 *pf4LineCentroids_host;
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMallocHost(	(void**)&pf4LineCentroids_host, sizeof(pf4LineCentroids_host[0]) * iNrOfLines) );
	for(int l = 0; l < iNrOfLines; l++)
	{
		pf4LineCentroids_host[l] = 
			make_float4(
				pfLineCentroids[3 * l + 0],
				pfLineCentroids[3 * l + 1],
				pfLineCentroids[3 * l + 2],
				1.0);
	}

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemcpy(	pf4LineCentroids_global, pf4LineCentroids_host, sizeof(pf4LineCentroids_global[0]) * iNrOfLines, cudaMemcpyHostToDevice) );

	FREE_MEMORY_ON_HOST(pf4LineCentroids_host);

	CUDPPConfiguration	cConfig;
	cConfig.op =		CUDPP_ADD;
	cConfig.datatype =	CUDPP_UINT;
	cConfig.algorithm =	CUDPP_SORT_RADIX;
	cConfig.options =	CUDPP_OPTION_KEY_VALUE_PAIRS;

	assert( 
		CUDPP_SUCCESS  == cudppPlan(
			&hScanPlan,	
			cConfig, 
			iNrOfLines,
			1, 
			0) );

}

void
_ComputeDepth_cuda
(
	int iNrOfSlabs,

	double dMinZ,
	double dMaxZ,
	double pdModelViewMatrix[],
	double pdProjectionMatrix[],

	int iNrOfLines,
	int2 pi2Slabs[]
)
{
	for(int i = 0; i < 16; i++)
	{
		pfModelViewMatrix_host[i] =		float(pdModelViewMatrix[i]);
		pfProjectionMatrix_host[i] =	float(pdProjectionMatrix[i]);
	}
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemcpyToSymbol(pfModelViewMatrix_constant,	pfModelViewMatrix_host,	sizeof(pfModelViewMatrix_constant)) );
	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemcpyToSymbol(pfProjectionMatrix_constant, pfProjectionMatrix_host, sizeof(pfProjectionMatrix_constant)) );

	int iNrOfThreads = 128;
	int iNrOfBlocks = int(ceilf(float(iNrOfLines)/float(iNrOfThreads)));

	_ComputeDepth_kernel<<<
		dim3(iNrOfBlocks, 1, 1),
		dim3(iNrOfThreads, 1, 1),
		0
	>>>
	(
		float(dMinZ),
		float(dMaxZ),
		iNrOfSlabs,
		iNrOfLines,
		pf4LineCentroids_global,
		piSlabs_global,
		piLines_global
	);
	CUT_CHECK_ERROR("_ComputeDepth_kernel() failed");

	cudppSort(
		hScanPlan,
		piSlabs_global,
		piLines_global,
		int(ceilf(log2f(float(iNrOfSlabs)))),
		iNrOfLines) ;
	CUT_CHECK_ERROR("cudppSort() failed");

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemcpy(	piSlabs_host, piSlabs_global, sizeof(piSlabs_host[0]) * iNrOfLines, cudaMemcpyDeviceToHost) );

	CUDA_SAFE_CALL_NO_SYNC(
		cudaMemcpy(	piLines_host, piLines_global, sizeof(piLines_host[0]) * iNrOfLines, cudaMemcpyDeviceToHost) );

	for(int l = 0; l < iNrOfLines; l++)
	{
		pi2Slabs[l].x = piSlabs_host[l];
		pi2Slabs[l].y = piLines_host[l];
	}
}

void 
_ComputeDeptpFree_cuda
(
)
{
	FREE_MEMORY( piSlabs_global );
	FREE_MEMORY( piLines_global );
	FREE_MEMORY_ON_HOST( piSlabs_host );
	FREE_MEMORY_ON_HOST( piLines_host );
	FREE_MEMORY( pf4LineCentroids_global );
	cudppDestroyPlan(hScanPlan);
}

} // extern "C"

/*

$Log: not supported by cvs2svn $
Revision 1.2  2009/12/31 02:00:30  leeten

[12/30/2009]
1. [ADD] Add the log section.


*/
