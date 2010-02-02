#ifndef __FLOW_FUSION_H__
#define __FLOW_FUSION_H__

// ADD-BY-LEETEN 12/07/2009-BEGIN
#define _USE_MATH_DEFINES	1
#include <math.h>

// ADD-BY-LEETEN 12/07/2009-BEGIN
#include "cudpp/cudpp.h"
// ADD-BY-LEETEN 01/25/2010-BEGIN
#ifdef __DEVICE_EMULATION__
#pragma comment (lib, "cudpp32d_emu.lib")
#else
// ADD-BY-LEETEN 01/25/2010-END
#pragma comment (lib, "cudpp32.lib")
// ADD-BY-LEETEN 01/25/2010-BEGIN
#endif
// ADD-BY-LEETEN 01/25/2010-END
// ADD-BY-LEETEN 12/07/2009-END

#include "cuda_macro.h"

#pragma comment (lib, "cutil32.lib ")      // link with my own library libfps

#pragma comment (lib, "cudart.lib")      // link with my own library libfps

#if	0	// MOD-BY-LEETEN 02/02/2010-FROM:
	#define MALLOC(p, type, size)\
		{	\
			if( NULL == (p) )\
			{\
				(p) = (type*)calloc((size), sizeof(type));\
			}\
		}\

#else	// MOD-BY-LEETEN 02/02/2010-TO:
#define MALLOC(p, type, size)\
	{	\
		if( NULL == (p) )\
		{\
			(p) = (type*)calloc((size), sizeof(type));\
			assert(p);\
		}\
	}\

#endif	// MOD-BY-LEETEN 02/02/2010-END

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

// ADD-BY-LEETEN 01/27/2010-BEGIN
void
_ComputeSrcEntropyVolume
(
	int iNrOfBins,
	int iKernelWidth, int iKernelHeight, int iKernelDepth,
	float *pfEntropyVolume_host = NULL
);
// ADD-BY-LEETEN 01/27/2010-END

// ADD-BY-LEETEN 12/14/2009-BEGIN
void 
_FlowDiffusionSetAngleMap(int *piAngleMap, int iNrOfPhis, int iNrOfThetas);

void 
_GetSrcBinVolume(int *piBinVolume);

void 
_GetDstBinVolume(int *piBinVolume);

void
_ComputeSrcBinVolume
(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	float *pfSrcVolume
);

// ADD-BY-LEETEN 12/14/2009-END

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
Revision 1.5  2010/01/27 22:06:56  leeten

[01/27/2010]
1. [ADD] Add the declaration of the new _ComputeSrcEntropyVolume().

Revision 1.4  2010/01/25 15:21:33  leeten

[01/25/2010]
1. [ADD] If the preprocessor __DEVICE_EMULATION__ is defined, link the cudpp32d_emu.lib.

Revision 1.3  2009/12/17 20:22:12  leeten

[12/17/2009]
1. [ADD] Incldue the header cudpp.h and the lib cudpp32.lib.

Revision 1.2  2009/12/15 20:00:20  leeten

[12/15/2009]
1. [ADD] Declare new functions _FlowDiffusionSetAngleMap(), _GetSrcBinVolume(), _GetDstBinVolume() and _ComputeSrcBinVolume().

Revision 1.1.1.1  2009/12/07 20:04:02  leeten

[12/07/2009]
1. [1ST] First time checkin.

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
