#ifndef __FLOW_DIFFUSION_GLSL_LIB_H__
#define __FLOW_DIFFUSION_GLSL_LIB_H__

void
_FlowDiffusionGLGLInit
(
	int iWidth,
	int iHeight,
	int iDepth
);

void
_FlowDiffusionGLGL
(
	float fAttenuation,
	int iNrOfIters,
	float pfWeightVolume[],
	float pfXOffsetVolume[],
	float pfYOffsetVolume[],
	float pfZOffsetVolume[],
	float pfSrcVolume[],
	float pfDstVolume[]
);

#endif	__FLOW_DIFFUSION_GLSL_LIB_H__

/*

$Log: not supported by cvs2svn $

*/
