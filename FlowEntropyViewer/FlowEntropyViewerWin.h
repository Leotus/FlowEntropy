#pragma once

	#include "libdvr2/DvrWin2.h"
	#include "libbuf3d.h"

	#include "libclip/ClipPlanes.h"
	#include "libclip/ClipVolume.h"

	#include "Streamline.h"

struct CFlowEntropyViewerWin :
	public CDvrWin2, public CClipVolume
{
	enum {
		RENDER_MODE_NONE,	
		RENDER_MODE_ENTROPY_FIELD,
		RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING,
		RENDER_MODE_STREAMLINES_IN_SLABS,
	};
	int iRenderMode;

	enum {
		DATA_NAME_NONE,
		DATA_NAME_ISABEL,
		DATA_NAME_EARTHQUAKE,
	};
	int iDataName;

	union CData 
	{
		struct CFusion{
			GLuint t2dMap;
		} cFusion;

		struct CEarthquake{
			GLuint lid;
		} cEarthquake;

		CData()
		{
			cFusion.t2dMap = 0;
			cEarthquake.lid = 0;
		}
	} cData;

	int ibIsLightingEnabled;
	int ibIsColorMono;
	int ibIsHaloEnabled;

	struct CClippingPlaneProp {
		float fThreshold;
		int ibMonoColor;
		float4 v4Color;

		CClippingPlaneProp()
		{
			fThreshold = 0.0f;
			ibMonoColor = 0.0f;
			v4Color.x = 0.0;
			v4Color.y = 0.0;
			v4Color.z = 0.0;
			v4Color.w = 0.0;
		}
	};
	CClippingPlaneProp cClippingPlaneOutsideProp, cClippingPlaneInsideProp;

	int iMaxDistanceToNeighbors_screen;	
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
	GLint iNrOfSlabsToRender;

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

		void _AddGlui(GLUI* pcGlui, GLUI_Panel	*pcPanel_Parent)
		{
			GLUI_Panel	*pcPanel_Material = pcGlui->add_panel_to_panel(pcPanel_Parent, "Material");
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

public:

	void _LoadData(int iDataName);

	// methods to read the input
	void _ReadStreamlines(char *szStreamlineFilename, int iMaxNrOfStreamlines);
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

