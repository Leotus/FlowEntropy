#ifndef __SORT__SLAB__H__
#define __SORT__SLAB__H__

extern "C" {
	void _ComputeDepthInit_cuda
	(
		int iNrOfLines,
		float pfLineCentroids[]
	);

	void _ComputeDepth_cuda
	(
		int iNrOfSlabs,

		double dMinZ,
		double dMaxZ,
		double pdModelViewMatrix[],
		double pdProjectionMatrix[],

		int iNrOfLines,
		int2 pi2Slabs[]
	);

	void 
	_ComputeDeptpFree_cuda
	(
	);
}

#endif	// __SORT__SLAB__H__