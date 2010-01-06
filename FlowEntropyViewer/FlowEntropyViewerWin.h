#pragma once

	#include "libdvr2/DvrWin2.h"
	#include "libbuf3d.h"

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	#include "libclip/ClipPlanes.h"
	#include "libclip/ClipVolume.h"
	// ADD-BY-LEETEN 01/05/2010-END

	#include "Streamline.h"

class CFlowEntropyViewerWin :
	// MOD-BY-LEETEN 01/05/2010-FROM:
		// public CDvrWin2
	// TO:
	public CDvrWin2, public CClipVolume
	// MOD-BY-LEETEN 01/05/2010-END
{
	enum {
		RENDER_MODE_ENTROPY_FIELD,
		RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING,
		RENDER_MODE_STREAMLINES_IN_SLABS,
	};
	int iRenderMode;

	// ADD-BY-LEETEN 01/01/2010-BEGIN

	#if	0	// MOD-BY-LEETEN 01/03/2010-FROM:
			enum {
				SHADING_NO_LIGHTING,
				SHADING_LIGHTING,
				SHADING_HALO,
			};
			int iShading;
	#else	// MOD-BY-LEETEN 01/03/2010-TO:
	int ibIsLightingEnabled;
	int ibIsColorMono;
	int ibIsHaloEnabled;
	#endif	// MOD-BY-LEETEN 01/03/2010-END
	// ADD-BY-LEETEN 01/01/2010-END

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	float fClippingThreshold;

	struct CClippingPlaneColor {
		int ibMonoColor;
		float4 v4Color;
	};
	CClippingPlaneColor cClippingPlaneFrontColor, cClippingPlaneBackColor;
	// ADD-BY-LEETEN 01/05/2010-END

	// ADD-BY-LEETEN 01/03/2010-BEGIN
	int iMaxDistanceToNeighbors_screen;	
	// ADD-BY-LEETEN 01/03/2010-END
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
	// MOD-BY-LEETEN 01/01/2010-FROM:
		// GLint iMaxSlab;
	// TO:
	GLint iNrOfSlabsToRender;
	// MOD-BY-LEETEN 01/01/2010-END

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	// lighting
	struct CMaterial {
		float fAmbient;
		float fDiffuse;
		float fSpecular;
		float fShininess;
	
		CMaterial()
		{
			fAmbient =		0.2f;
			fDiffuse =		0.4f;
			fSpecular =		0.4f;
			fShininess =	4.0f;
		}

		void _AddGlui(GLUI* pcGlui)
		{
			GLUI_Panel	*pcPanel_Material = pcGlui->add_panel("Material");
				GLUI_Spinner *pcSpinner_Ambient = pcGlui->add_spinner_to_panel(pcPanel_Material, "Ambinet", GLUI_SPINNER_FLOAT, &fAmbient);	
				pcSpinner_Ambient->set_float_limits(0.0f, 1.0f);
				GLUI_Spinner *pcSpinner_Diffuse = pcGlui->add_spinner_to_panel(pcPanel_Material, "Diffuse", GLUI_SPINNER_FLOAT, &fDiffuse);	
				pcSpinner_Diffuse->set_float_limits(0.0f, 1.0f);
				GLUI_Spinner *pcSpinner_Specular = pcGlui->add_spinner_to_panel(pcPanel_Material, "Specular", GLUI_SPINNER_FLOAT, &fSpecular);	
				pcSpinner_Specular->set_float_limits(0.0f, 1.0f);
				GLUI_Spinner *pcSpinner_Shininess = pcGlui->add_spinner_to_panel(pcPanel_Material, "Shininess", GLUI_SPINNER_FLOAT, &fShininess);	
				pcSpinner_Shininess->set_float_limits(0.0f, 128.0f);
		}
	} cMaterial;
	// ADD-BY-LEETEN 01/01/2010-END

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
Revision 1.4  2010/01/04 18:25:56  leeten

[01/04/2010]
1. [ADD] Add new member variables: ibIsLightingEnabled, ibIsColorMono, ibIsHaloEnabled and iMaxDistanceToNeighbors_screen.

Revision 1.3  2010/01/01 18:23:12  leeten

[01/01/2010]
1. [ADD] Declare new constants SHADING_NO_LIGHTING, SHADING_LIGHTING, SHADING_HALO and a variable iShading to indicate the shading algorithm.
2. [MOD] Change the variable iMaxSlab to INrOFSlabsToRender.
3. [ADD] Add a new data structure CMaterial to define the material for lighting,

Revision 1.2  2009/12/31 01:59:54  leeten

[12/30/2009]
1. [ADD] Add the log section.


*/
