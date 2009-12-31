#pragma once

	#include "libdvr2/DvrWin2.h"
	#include "libbuf3d.h"

	#include "Streamline.h"

class CFlowEntropyViewerWin :
	public CDvrWin2
{
	enum {
		RENDER_MODE_ENTROPY_FIELD,
		RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING,
		RENDER_MODE_STREAMLINES_IN_SLABS,
	};
	int iRenderMode;

	// streamline-related variables
	CStreamline cStreamline;

	// entropy-field-related variables
	GLuint t3dEntropyField;
	float fMaxEntropy;
	TBuffer3D<float> pf3DEntropyField;

	// transfer-function-related variables
	GLuint t1dTf;
	float fTfDomainMin;
	float fTfDomainMax;

	// DVR-related variables
	float fThicknessGain;	
	GLuint pidRayIntegral;	// Id of shader programs

	float fOcclusionSaturation;
	GLuint pidImportanceFilling;	// Id of shader programs to do importance culling
	GLuint pidImportanceCulling;	// Id of shader programs to do importance culling
	GLint iMinSlab;
	GLint iMaxSlab;

public:
	// methods to read the input
	void _ReadStreamlines(char *szStreamlineFilename);
	void _SetEntropyField(int iXDim, int iYDim, int iZDim, float *pfEntropyField);

	// methods to control the transfer functions
	void _GetScalarRange(float *pfMin, float *pfMax);
	void _SetTfDomain(float fMin, float fMax);
	void _SetTransferFunc(const void *pTf, GLenum eType, GLenum eFormat, int iNrOfTfEntries);

	// methods to control the rendering
	void _BeginDisplay();
	void _EndDisplay();
	void _RenderSlab(
		int iSlab, int iNrOfSlabs,
		double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
		double dMinX, double dMaxX, 
		double dMinY, double dMaxY, 
		double dMinZ, double dMaxZ);

	// GLUT callbacks
	void _InitFunc();
	void _DisplayFunc();
	void _ReshapeFunc(int w, int h);
	// void _TimerFunc(unsigned short uEvent);
	// void _GluiFunc(unsigned short uEvent);

	CFlowEntropyViewerWin(void);
	virtual ~CFlowEntropyViewerWin(void);
};

/*

$Log: not supported by cvs2svn $

*/
