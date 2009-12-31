	#include <GL/glew.h>

	#include <stdio.h>
	#include <stdlib.h>

	#include "shader.h"

	#include "FlowEntropyViewerWin.h"

void 
CFlowEntropyViewerWin::_GetScalarRange(float *pfMin, float *pfMax)
{
	*pfMin = 0.0f;
	*pfMax = fMaxEntropy;
}

void 
CFlowEntropyViewerWin::_SetTfDomain(float fMin, float fMax)
{
	fTfDomainMin = fMin;
	fTfDomainMax = fMax;
}

void 
CFlowEntropyViewerWin::_SetTransferFunc(const void *pTf, GLenum eType, GLenum eFormat, int iNrOfTfEntries)
{
_Begin();

	// upload the transfer func. as a 1D texture
	glActiveTexture(GL_TEXTURE0 + 1);
	if( !t1dTf )
		glGenTextures(1, &t1dTf);	
	glBindTexture(GL_TEXTURE_1D, t1dTf);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB,	
		iNrOfTfEntries, 0, eType, eFormat, pTf);
_End();
}


void 
CFlowEntropyViewerWin::_SetEntropyField(int iXDim, int iYDim, int iZDim, float *pfEntropyField)
{
	fMaxEntropy = -(float)HUGE_VAL;
	for(int v = 0,	z = 0; z < iZDim; z++)
		for(int		y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, v++)
			{
				fMaxEntropy = max(fMaxEntropy, pfEntropyField[v]);
			}

	this->pf3DEntropyField.alloc(iXDim, iYDim, iZDim);
	assert( this->pf3DEntropyField.BIsAllocated() );

	memcpy( &this->pf3DEntropyField[0], &pfEntropyField[0], sizeof(this->pf3DEntropyField[0]) * iXDim * iYDim * iZDim);

	for(int v = 0,	z = 0; z < iZDim; z++)
		for(int		y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, v++)
			{
				this->pf3DEntropyField[v] /= fMaxEntropy;
			}

	CREATE_3D_TEXTURE(
		GL_TEXTURE_3D, t3dEntropyField, 
		GL_LINEAR, GL_LUMINANCE32F_ARB, 
		pf3DEntropyField.iWidth, pf3DEntropyField.iHeight, pf3DEntropyField.iDepth, 
		GL_LUMINANCE, GL_FLOAT, 
		&pf3DEntropyField[0]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void
CFlowEntropyViewerWin::_ReadStreamlines(char *szStreamlineFilename)
{
	cStreamline._Read(float(pf3DEntropyField.iWidth), float(pf3DEntropyField.iHeight), float(pf3DEntropyField.iDepth), szStreamlineFilename);
}

void 
CFlowEntropyViewerWin::_BeginDisplay()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	float fMaxDim = max(pf3DEntropyField.iWidth, max(pf3DEntropyField.iHeight, pf3DEntropyField.iDepth));
	glScalef(
		pf3DEntropyField.iWidth / fMaxDim,
		pf3DEntropyField.iHeight / fMaxDim,
		pf3DEntropyField.iDepth / fMaxDim);

	glColor4f(0.70f, 0.70f, 0.70f, 1.0f);
	glutWireCube(2.0);

	//////////////////////////////////////////
	// sort the line centroids
	static double pdModelViewMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, pdModelViewMatrix);

	cStreamline._SortSlab(iNrOfSlices, pdModelViewMatrix, tProjectionMatrix, piViewport);

	//////////////////////////////////////////
	switch(iRenderMode)
	{
	case RENDER_MODE_ENTROPY_FIELD:
		glUseProgramObjectARB(	pidRayIntegral);
		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fWindowWidth",				(float)piViewport[2]);
		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fWindowHeight",			(float)piViewport[3]);
		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fThicknessGain",			fThicknessGain);

		SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t2dPrevLayer",				0);
		SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t3dVolume",				1);
		SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t1dTf",					2);

		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fTfDomainMin",				fTfDomainMin);
		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fTfDomainMax",				fTfDomainMax);
		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fDataValueMin",			0);
		SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fDataValueMax",			fMaxEntropy);
		break;

	case RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING:
		glUseProgramObjectARB(	pidImportanceFilling);
		SET_1F_VALUE_BY_NAME(	pidImportanceFilling, "fWindowWidth",		(float)piViewport[2]);
		SET_1F_VALUE_BY_NAME(	pidImportanceFilling, "fWindowHeight",		(float)piViewport[3]);

		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "t2dPrevLayer",		0);
		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "t3dVolume",			1);

		glUseProgramObjectARB(	pidImportanceCulling);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fWindowWidth",		(float)piViewport[2]);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fWindowHeight",		(float)piViewport[3]);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fThicknessGain",			fThicknessGain);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fOcclusionSaturation",	fOcclusionSaturation);

		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t2dPrevLayer",		0);
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t3dVolume",			1);
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t1dTf",				2);

		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fTfDomainMin",		fTfDomainMin);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fTfDomainMax",		fTfDomainMax);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDataValueMin",		0);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDataValueMax",		fMaxEntropy);
		break;
	}

	glUseProgramObjectARB(0);
	//////////////////////////////////////////
						// bind the volume, range, and the lookup table as textures
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_3D, t3dEntropyField);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_1D, t1dTf);

	glActiveTexture(GL_TEXTURE0);
}

void CFlowEntropyViewerWin::_RenderSlab(
	int iSlab, int iNrOfSlabs,
	double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
	double dMinX, double dMaxX, 
	double dMinY, double dMaxY, 
	double dMinZ, double dMaxZ)
{
	/*
	*/
	switch(iRenderMode)
	{
	case RENDER_MODE_ENTROPY_FIELD:
		glUseProgramObjectARB(pidRayIntegral);
		CDvrWin2::_RenderSlab(
			iSlab, iNrOfSlabs, 

			pdModelviewMatrix, pdProjectionMatrix, piViewport,
			
			dMinX, dMaxX, 
			dMinY, dMaxY, 
			dMinZ, dMaxZ);
		break;

	case RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING:
		if( iMinSlab <= iSlab && iSlab < min(iNrOfSlabs, iMaxSlab) )
		{
			glUseProgramObjectARB(pidImportanceCulling);
			cStreamline._RenderLinesInSlab(iSlab);
		}

		glUseProgramObjectARB(pidImportanceFilling);
		CDvrWin2::_RenderSlab(
			iSlab, iNrOfSlabs, 

			pdModelviewMatrix, pdProjectionMatrix, piViewport,
			
			dMinX, dMaxX, 
			dMinY, dMaxY, 
			dMinZ, dMaxZ);
		break;

	case RENDER_MODE_STREAMLINES_IN_SLABS:
		if( iMinSlab <= iSlab && iSlab < min(iNrOfSlabs, iMaxSlab) )
			cStreamline._RenderLinesInSlab(iSlab, true);

		break;
	}
}


void 
CFlowEntropyViewerWin::_EndDisplay()
{
	glUseProgramObjectARB(0);

	/////////////////////////////////////////
	glPopMatrix();
}

void 
CFlowEntropyViewerWin::_DisplayFunc()
{
	CDvrWin2::_DisplayFunc();
}

void 
CFlowEntropyViewerWin::_ReshapeFunc(int w, int h)
{
	CDvrWin2::_ReshapeFunc(w, h);
}

void 
CFlowEntropyViewerWin::_InitFunc()
{
	CDvrWin2::_InitFunc();

	_DisableVerticalSync();
	_KeepUpdateOn();
	_DisplayFpsOn();

	///////////////////////////////////////////////////////////////////
	pidRayIntegral = CSetShadersByString(
		NULL
		,
		#include "ray_integral.frag.h"	
	);
	assert( pidRayIntegral );	

	///////////////////////////////////////////////////////////////////
	pidImportanceCulling = CSetShadersByString(
		NULL
		,
		#include "importance_culling.frag.h"	
	);
	assert( pidImportanceCulling );	

	///////////////////////////////////////////////////////////////////
	pidImportanceFilling = CSetShadersByString(
		NULL
		,
		#include "importance_filling.frag.h"	
	);
	assert( pidImportanceFilling );	

	///////////////////////////////////////////////////////////////////
	// set up UI
	GLUI *pcGlui = PCGetGluiWin();

	GLUI_Spinner *pcSpinner_NrOfSlices = PCGetGluiWin()->add_spinner("#Slices", GLUI_SPINNER_INT, &iNrOfSlices);	
		pcSpinner_NrOfSlices->set_int_limits(1, 4096);

						// create a spinner to control the brightness gain 
	GLUI_Spinner *pcSpinner_ThicknessGain = PCGetGluiWin()->add_spinner("Thickness Gain", GLUI_SPINNER_FLOAT, &fThicknessGain);	
	pcSpinner_ThicknessGain->set_float_limits(0.0f, 4096.0f);

						// create a spinner to control the brightness gain 
	GLUI_Panel *pcPanel_RenderMode = PCGetGluiWin()->add_panel("Render Mode");
	GLUI_RadioGroup *pcRadioGroup_RenderMode = PCGetGluiWin()->add_radiogroup_to_panel(pcPanel_RenderMode, &iRenderMode);
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "Entropy Field");
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "Streamlines w/ Importance Culling");
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "Streamlines in Slabs");

	GLUI_Panel *pcPanel_Slabs = PCGetGluiWin()->add_panel("Slab");
	PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Min Slab", GLUI_SPINNER_INT, &iMinSlab);	
	PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Max Slab", GLUI_SPINNER_INT, &iMaxSlab);	
	GLUI_Spinner *pcSpinner_OcclusionSaturation = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Occlusion Saturation ", GLUI_SPINNER_FLOAT, &fOcclusionSaturation );	
	pcSpinner_OcclusionSaturation->set_float_limits(0.0f, 1.0f);

	cStreamline._AddGlui(PCGetGluiWin());
}

/////////////////////////////////////////////////////////////////////
CFlowEntropyViewerWin::CFlowEntropyViewerWin(void)
{
	iMinSlab = 0;
	iMaxSlab = 1024;

	fOcclusionSaturation = 0.0f;

	fThicknessGain = 1.0f;

	ibIsFboEnabled = 1;			// enable the rendering to FBO
	_SetInternalColorFormat(GL_RGBA32F_ARB);	// set the depths of each chanel of the FBO as 32 bits 

	iNrOfSlices = 128;			// set up the default number of slices

	iRenderMode = RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING;
	_AddGluiWin();
}

CFlowEntropyViewerWin::~CFlowEntropyViewerWin(void)
{
}

/*

$Log: not supported by cvs2svn $

*/
