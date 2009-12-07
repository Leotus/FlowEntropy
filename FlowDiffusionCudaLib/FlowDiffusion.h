#ifndef __FLOW_FUSION_H__
#define __FLOW_FUSION_H__

// ADD-BY-LEETEN 12/07/2009-BEGIN
#define _USE_MATH_DEFINES	1
#include <math.h>

#include "cuda_macro.h"

#pragma comment (lib, "cutil32.lib ")      // link with my own library libfps
#pragma comment (lib, "cudart.lib")      // link with my own library libfps

#define MALLOC(p, type, size)\
	{	\
		if( NULL == (p) )\
		{\
			(p) = (type*)calloc((size), sizeof(type));\
		}\
	}\


#define FREE(p)	\
	{	\
		if( NULL != (p) )	\
		{\
			free(p);\
		}\
		p = NULL;\
	}\

// ADD-BY-LEETEN 12/07/2009-END

extern "C" {

void
// MOD-BY-LEETEN 12/07/2009-FROM:
	// _FlowFusionInit(
// TO:
_FlowDiffusionInit(
// MOD-BY-LEETEN 12/07/2009-END
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth
);

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
	int *piFlagVolume = NULL
	// ADD-BY-LEETEN 2009/11/10-END
);

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
	);
#endif	// DEL-BY-LEETEN 12/07/2009-END

// MOD-BY-LEETEN 12/07/2009-FROM:
	// void _FlowFusionFree();
// TO:
void _FlowDiffusionFree();
// MOD-BY-LEETEN 12/07/2009-END

} 
// extern "C"

#endif	// ifndef __FLOW_FUSION_H__

/*

$Log: not supported by cvs2svn $
Revision 1.3  2009/12/05 21:21:50  leeten

[12/05/2009]
1. [ADD] Specify the Xoffset, Yoffset, Zoffset, the weight and the donnotchange flag of each voxel when call the function.

Revision 1.2  2009/11/04 19:13:35  leeten

[2009/11/04]
1. [ADD] Add 3 new parameters as the block size on z direction, weight volume and the offset volume to the function _FlowFusion().
2. [ADD] Add 2 new parameters as the weight volume and the offset volume to the function _FlowFusionCPU().

Revision 1.1.1.1  2009/11/02 15:30:56  leeten

[11/02/2009]
1 [1ST] Firs time checkin.


*/
