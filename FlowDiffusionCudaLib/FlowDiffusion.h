#ifndef __FLOW_FUSION_H__
#define __FLOW_FUSION_H__

#define _USE_MATH_DEFINES	1
#include <math.h>

#define	DIFFUSION_CONVERGE_THRESHOLD	0.9995


#include "cuda_macro.h"

#include "libclock.h"	
#define CLOCK_INIT	LIBCLOCK_INIT
#define CLOCK_BEGIN	LIBCLOCK_BEGIN
#define CLOCK_END	LIBCLOCK_END
#define CLOCK_PRINT	LIBCLOCK_PRINT

#include "liblog.h"
#ifdef assert
#undef assert
#endif
#define assert(expr)	ASSERT_OR_LOG(expr, "")

#define MALLOC(p, type, size)\
	{	\
		if( NULL == (p) )\
		{\
			(p) = (type*)calloc((size), sizeof(type));\
			assert(p);\
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

extern "C" {

void
_ComputeSrcEntropyVolume
(
	int iNrOfBins,
	int iKernelWidth, int iKernelHeight, int iKernelDepth,
	float *pfEntropyVolume_host = NULL
);

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


void
_FlowDiffusionInit(
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth
);

void
_FlowDiffusion(
	float fAttenuation,
	int iNrOfIterations,
	int iVolumeWidth,
	int iVolumeHeight,
	int iVolumeDepth,
	int iBlockZSize,
	float *pfWeightVolume,
	float *pfXOffsetVolume,
	float *pfYOffsetVolume,
	float *pfZOffsetVolume,
	float *pfSrcVolume,
	float *pfDstVolume,
	int *piFlagVolume = NULL
);

void _FlowDiffusionFree();

} 
// extern "C"

#endif	// ifndef __FLOW_FUSION_H__

