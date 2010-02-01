	#include <GL/glew.h>

	#include <stdio.h>
	#include <stdlib.h>

	#include "shader.h"

	// ADD-BY-LEETEN 01/12/2010-BEGIN
	#include "liblog.h"
	// ADD-BY-LEETEN 01/12/2010-END

	#include "FlowEntropyViewerWin.h"

// ADD-BY-LEETEN 01/12/2010-BEGIN

static
GLuint 
UReadIsosurface
(
	char *szIsosurfaceFilename, 
	unsigned int uXDim, 
	unsigned int uYDim, 
	unsigned int uZDim,
	float fIsosurfaceSpacingX,
	float fIsosurfaceSpacingY,
	float fIsosurfaceSpacingZ,
	int ibBothFacesIsosurface
)
{
	size_t uNrOfVertices;
	size_t uNrOfFaces;
	size_t uNrOfFaceIndices;
	float *pfVertices = NULL;
	float *pfNormals = NULL;
	size_t *puFaceIndices = NULL;

	FILE *fpIsosurface;
	fopen_s(&fpIsosurface, szIsosurfaceFilename, "rt");
	assert(fpIsosurface);

	#define SZ_TEMP_LENGTH 1024

	static char szTemp[SZ_TEMP_LENGTH+1];

	fgets(szTemp, SZ_TEMP_LENGTH, fpIsosurface);	// # vtk DataFile Version 3.0
	fgets(szTemp, SZ_TEMP_LENGTH, fpIsosurface);	// vtk output
	fgets(szTemp, SZ_TEMP_LENGTH, fpIsosurface);	// ASCII
	fgets(szTemp, SZ_TEMP_LENGTH, fpIsosurface);	// DATASET POLYDATA
	fscanf(fpIsosurface, "%s", szTemp);	// POINTS
	fscanf(fpIsosurface, "%d", &uNrOfVertices);
	fscanf(fpIsosurface, "%s", szTemp);	// float
	pfVertices = (float*)calloc(uNrOfVertices * 3, sizeof(pfVertices[0]));
	assert( pfVertices );
	for(size_t v = 0; v < uNrOfVertices; v++)
		fscanf(fpIsosurface, "%f %f %f", &pfVertices[3 * v], &pfVertices[3 * v + 1], &pfVertices[3 * v + 2]);

	fscanf(fpIsosurface, "%s", szTemp);	// POLYGONS 29314 117256
	fscanf(fpIsosurface, "%d %d", &uNrOfFaces, &uNrOfFaceIndices);
	puFaceIndices = (size_t*)calloc(uNrOfFaceIndices, sizeof(puFaceIndices[0]));
	assert( puFaceIndices );
	for(size_t f = 0, p = 0; f < uNrOfFaces; f++)
	{
		size_t uNrOfIndices;
		fscanf(fpIsosurface, "%d", &uNrOfIndices);
		puFaceIndices[p] = uNrOfIndices;
		p++;

		for(size_t i = 0; i < uNrOfIndices; i++, p++)
			fscanf(fpIsosurface, "%d", &puFaceIndices[p]);
	}

	fgets(szTemp, SZ_TEMP_LENGTH, fpIsosurface);	// NORMALS normals float
	fgets(szTemp, SZ_TEMP_LENGTH, fpIsosurface);	// NORMALS normals float
	pfNormals = (float*)calloc(uNrOfVertices * 3, sizeof(pfNormals[0]));
	assert( pfNormals );
	for(size_t v = 0; v < uNrOfVertices; v++)
		fscanf(fpIsosurface, "%f %f %f", &pfNormals[3 * v], &pfNormals[3 * v + 1], &pfNormals[3 * v + 2]);
	fclose(fpIsosurface);

	GLuint lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);

	glPushMatrix();
	glTranslatef(-1.0f, -1.0f, -1.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	glScaled(1.0/(double)uXDim, 1.0/(double)uYDim, 1.0/(double)uZDim);
	glScalef(1.0f/fIsosurfaceSpacingX, 1.0f/fIsosurfaceSpacingY, 1.0f/fIsosurfaceSpacingZ);

	for(size_t i = 0, f = 0, uNrOfIndices = 0; f < uNrOfFaces; f++, i += uNrOfIndices)
	{
		uNrOfIndices = puFaceIndices[i++];
		glBegin(GL_POLYGON);

		for(size_t v = 0; v < uNrOfIndices; v++)
		{
			glNormal3fv(&pfNormals[3*puFaceIndices[i+v]]);
			glVertex3fv(&pfVertices[3*puFaceIndices[i+v]]);
		}

		glEnd();

		if( ibBothFacesIsosurface )
		{
			glBegin(GL_POLYGON);

			for(size_t v = 0; v < uNrOfIndices; v++)
			{
				size_t rv = uNrOfIndices - 1 - v;
				glNormal3f(-pfNormals[3*puFaceIndices[i+rv]], -pfNormals[3*puFaceIndices[i+rv] + 1], -pfNormals[3*puFaceIndices[i+rv] + 2]);
				glVertex3fv(&pfVertices[3*puFaceIndices[i+rv]]);
			}
			glEnd();
		}
	}
	glPopMatrix();
	glEndList();

	free(pfVertices);
	free(pfNormals);
	free(puFaceIndices);
	return lid;
}


void
CFlowEntropyViewerWin::_LoadData(int iDataName)
{
	this->iDataName = iDataName;
	switch(iDataName)
	{
	case DATA_NAME_ISABEL:
		{
		IplImage *pcMap = cvLoadImage("fusion_color.png");
		CREATE_2D_TEXTURE(GL_TEXTURE_2D, cData.cFusion.t2dMap, GL_LINEAR, GL_RGBA, pcMap->width, pcMap->height, GL_BGR, GL_UNSIGNED_BYTE, pcMap->imageData);
		}
		break;

	case DATA_NAME_EARTHQUAKE:
		{
			cData.cEarthquake.lid = UReadIsosurface("basin.vtk", pf3DEntropyField.iWidth, pf3DEntropyField.iHeight, pf3DEntropyField.iDepth, 1.0, 1.0, 1.0, 1);
		}
		break;
	}
}

// ADD-BY-LEETEN 01/12/2010-END

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

	// ADD-BY-LEETEN 01/08/2010-BEGIN
				// create the user control after the streamlines have been read so #streamlines can be knewn
	cStreamline._AddGlui(PCGetGluiWin());
	// ADD-BY-LEETEN 01/08/2010-END
}

void 
CFlowEntropyViewerWin::_BeginDisplay()
{
	// MOD-BY-LEETEN 01/02/2010-BEGIN
		// glClearColor(1.0, 1.0, 1.0, 0.0);
	// TO:
	// MOD-BY-LEETEN 01/08/2010-FROM:
		// glClearColor(0.0, 0.0, 0.0, 0.0);
	// TO:
	glClearColor(1.0, 1.0, 1.0, 0.0);
	// MOD-BY-LEETEN 01/08/2010-END
	// MOD-BY-LEETEN 01/02/2010-END
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	float fMaxDim = max(pf3DEntropyField.iWidth, max(pf3DEntropyField.iHeight, pf3DEntropyField.iDepth));
	glScalef(
		pf3DEntropyField.iWidth / fMaxDim,
		pf3DEntropyField.iHeight / fMaxDim,
		pf3DEntropyField.iDepth / fMaxDim);

	// ADD-BY-LEETEN 01/12/2010-BEGIN
						// flip the Z axis
	glScalef(1.0f, 1.0f, -1.0f);
	// ADD-BY-LEETEN 01/12/2010-END

	glColor4f(0.70f, 0.70f, 0.70f, 1.0f);
	glutWireCube(2.0);

	// ADD-BY-LEETEN 01/12/2010-BEGIN
	switch(iDataName)
	{
	case DATA_NAME_ISABEL:
		glPushAttrib(GL_TEXTURE_BIT);
		glBindTexture(GL_TEXTURE_2D, cData.cFusion.t2dMap);
		glEnable(GL_TEXTURE_2D);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0);	glVertex3i(-1,-1,-1);
			glTexCoord2i(1, 0);	glVertex3i( 1,-1,-1);
			glTexCoord2i(1, 1);	glVertex3i( 1, 1,-1);
			glTexCoord2i(0, 1);	glVertex3i(-1, 1,-1);
		glEnd();
		glPopAttrib();	// glPushAttrib(GL_TEXTURE_BIT);
		break;

	case DATA_NAME_EARTHQUAKE:
		{
		glPushAttrib(GL_CURRENT_BIT);
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		glPushAttrib(GL_ENABLE_BIT);

		// ADD-By-LEETEN 03/12/2008-BEGIN
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		static float pfColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
		glColor4fv(pfColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pfColor);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		// ADD-BY-LEETEN 03/07/2008-BEGIN
		glShadeModel(GL_SMOOTH);
		// ADD-BY-LEETEN 03/07/2008-END

		glPushMatrix();
		glLoadIdentity();

		static float pfLight0Pos[4] = {0.0, 0.0, -1.0, 0.0};
		glLightfv(GL_LIGHT0, GL_POSITION, pfLight0Pos);

		glPopMatrix();

		glCallList(cData.cEarthquake.lid);

		glPopAttrib();
		glPopAttrib();
		glPopAttrib();
		}
		break;
	}
	// ADD-BY-LEETEN 01/12/2010-END

	// ADD-BY-LEETEN 01/10/2010-BEGIN
					// plot the basis vectors
	// ADD-BY-LEETEN 01/12/2010-BEGIN
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	// ADD-BY-LEETEN 01/12/2010-END
	glPushMatrix();

		glTranslatef(-1.0f, -1.0f, -1.0f);

		// plot the origin
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		_DrawString3D("0");

		double pdScales[3];
		pdScales[0] = 1.0/(double)this->pf3DEntropyField.iWidth;
		pdScales[1] = 1.0/(double)this->pf3DEntropyField.iHeight;
		pdScales[2] = 1.0/(double)this->pf3DEntropyField.iDepth;

		double dLength = 33.0;
		// calculate the vector
		for(int k =0; k < 3; k++) // direction
		{
			double pdColor[4];
			memset(pdColor, 0, sizeof(pdColor));
			pdColor[k] = 1.0;
			pdColor[3] = 1.0;
			glColor4dv(pdColor);

			double pdDir[3];
			memset(pdDir, 0, sizeof(pdDir));
			pdDir[k] = dLength * pdScales[k];

			glBegin(GL_LINES);
				glVertex3d(0.0, 0.0, 0.0);
				glVertex3dv(pdDir);

				double dOffsetLength = dLength * 0.25;
				double pdDirOffset[3];

				for(int j = 0; j < 2; j++)
				{
					for(int i = 0; i< 3; i++)
						pdDirOffset[i] = dOffsetLength * pdScales[i];

					pdDirOffset[k] *= -1.0;
					pdDirOffset[(k + j + 1)%3] = 0.0;
					for(int i = 0; i< 3; i++)
						pdDirOffset[i] = pdDir[i] + pdDirOffset[i];

					glVertex3dv(pdDir);
					glVertex3dv(pdDirOffset);
				}
			glEnd();

			static char* pszDir[3] = {
				"X",
				"Y",
				"Z",
			};
			_DrawString3D(pszDir[k], pdDir[0], pdDir[1], pdDir[2]);
		}
	glPopMatrix();
	// ADD-BY-LEETEN 01/12/2010-BEGIN
	glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);
	// ADD-BY-LEETEN 01/12/2010-END

	// ADD-BY-LEETEN 01/10/2010-END

	//////////////////////////////////////////
	// sort the line centroids
	static double pdModelViewMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, pdModelViewMatrix);

	cStreamline._SortSlab(iNrOfSlices, pdModelViewMatrix, tProjectionMatrix, piViewport);

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	CClipVolume::_Create();
	// ADD-BY-LEETEN 01/05/2010-END

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

		// ADD-BY-LEETEN 01/05/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t2dClipVolume",			4);
		// ADD-BY-LEETEN 01/05/2010-END

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
		// ADD-BY-LEETEN 01/01/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "t2dLineFlag",		3);
		// ADD-BY-LEETEN 01/01/2010-END

		// ADD-BY-LEETEN 01/05/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "t2dClipVolume",		4);
		// ADD-BY-LEETEN 01/12/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "t2dsDepth",			5);
		// ADD-BY-LEETEN 01/12/2010-END

		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "t1dTf",				2);
		SET_1F_VALUE_BY_NAME(	pidImportanceFilling, "fTfDomainMin",		fTfDomainMin);
		SET_1F_VALUE_BY_NAME(	pidImportanceFilling, "fTfDomainMax",		fTfDomainMax);
		SET_1F_VALUE_BY_NAME(	pidImportanceFilling, "fDataValueMin",		0);
		SET_1F_VALUE_BY_NAME(	pidImportanceFilling, "fDataValueMax",		fMaxEntropy);

		SET_4FV_VALUE_BY_NAME(	pidImportanceFilling, "v4ClippingPlaneOutsideColor",		1, (float*)&cClippingPlaneOutsideProp.v4Color);
		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "ibIsClippingPlaneOutsideColorMono",cClippingPlaneOutsideProp.ibMonoColor);
		SET_4FV_VALUE_BY_NAME(	pidImportanceFilling, "v4ClippingPlaneInsideColor",		1, (float*)&cClippingPlaneInsideProp.v4Color);
		SET_1I_VALUE_BY_NAME(	pidImportanceFilling, "ibIsClippingPlaneInsideColorMono",	cClippingPlaneInsideProp.ibMonoColor);
		// ADD-BY-LEETEN 01/05/2010-END

		glUseProgramObjectARB(	pidImportanceCulling);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fWindowWidth",		(float)piViewport[2]);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fWindowHeight",		(float)piViewport[3]);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fThicknessGain",			fThicknessGain);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fOcclusionSaturation",	fOcclusionSaturation);

		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t2dPrevLayer",		0);
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t3dVolume",			1);
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t1dTf",				2);
		// ADD-BY-LEETEN 01/05/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t2dClipVolume",		4);

		// ADD-BY-LEETEN 01/12/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "t2dsDepth",			5);
		// ADD-BY-LEETEN 01/12/2010-END

		// MOD-BY-LEETEN 01/07/2010-FROM:
			// SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fClippingThreshold", fClippingThreshold);
		// TO:
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fClipVolumeOutsideThreshold",	cClippingPlaneOutsideProp.fThreshold);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fClipVolumeInsideThreshold",		cClippingPlaneInsideProp.fThreshold);
		// MOD-BY-LEETEN 01/07/2010-END
		// ADD-BY-LEETEN 01/05/2010-END

		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fTfDomainMin",		fTfDomainMin);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fTfDomainMax",		fTfDomainMax);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDataValueMin",		0);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDataValueMax",		fMaxEntropy);

		// ADD-BY-LEETEN 01/10/2010-BEGIN
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDashPeriod",						float(cStreamline.cDash.iPeriod) );
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDashOffset",						cStreamline.cDash.fOffset);
		// ADD-BY-LEETEN 01/12/2010-BEGIN
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDashThreshold",						cStreamline.cDash.fThreshold);
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsEntropyDependentDashed",	cStreamline.cDash.ibIsEntropyDependent);
		// ADD-BY-LEETEN 01/12/2010-END
		// SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fDashThreshold",						cStreamline.cDash.fThreshold);
		// SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsHigherEntropyWithLongerLine",	cStreamline.cDash.ibIsHigherEntropyWithLongerLine);
		// ADD-BY-LEETEN 01/10/2010-END

		// ADD-BY-LEETEN 01/01/2010-BEGIN
		#if	0	// DEL-BY-LEETEN 01/03/2010-BEGIN
			SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsLightingEnabled",		(SHADING_LIGHTING == iShading)?1:0 );
			SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsColorMono",			(SHADING_HALO == iShading)?1:0 );
		#endif	// DEL-BY-LEETEN 01/03/2010-END

		// ADD-BY-LEETEN 01/03/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsLightingEnabled",		ibIsLightingEnabled );
		// ADD-BY-LEETEN 01/03/2010-END

		// ADD-BY-LEETEN 01/03/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "iMaxDistanceToNeighbors_screen",	iMaxDistanceToNeighbors_screen);
		// ADD-BY-LEETEN 01/03/2010-END

		// ADD-BY-LEETEN 01/12/2010-BEGIN
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsGlyphEnabled",	cStreamline.cGlyph.ibIsEnabled);
		SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "iGlyphStep",			cStreamline.cGlyph.iStep);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fGlyphLength",		cStreamline.cGlyph.fLength);
		SET_1F_VALUE_BY_NAME(	pidImportanceCulling, "fGlyphWidth",		cStreamline.cGlyph.fWidth);
		// ADD-BY-LEETEN 01/12/2010-END

		float pfAmbient[4];
		float pfDiffuse[4];
		float pfSpecular[4];
		for(int i = 0; i < 4; i++)
		{
			pfAmbient[i] = cMaterial.fAmbient;
			pfDiffuse[i] = cMaterial.fDiffuse;
			pfSpecular[i] = cMaterial.fSpecular;
		}
		glMaterialfv(GL_FRONT, GL_AMBIENT, pfAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, pfDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, pfSpecular);
		glMaterialf(GL_FRONT, GL_SHININESS, cMaterial.fShininess);
		// ADD-BY-LEETEN 01/01/2010-END
		
		break;
	}

	glUseProgramObjectARB(0);

	//////////////////////////////////////////
						// bind the volume, range, and the lookup table as textures
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_3D, t3dEntropyField);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_1D, t1dTf);

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(CDvrWin2::cColor.eTarget, CDvrWin2::cColor.t2d);
	// ADD-BY-LEETEN 01/01/2010-END

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(CClipVolume::cTexture.eTarget, CClipVolume::cTexture.t2d);
	// ADD-BY-LEETEN 01/05/2010-END
	
	// ADD-BY-LEETEN 01/12/2010-BEGIN
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(CDvrWin2::cDepth.eTarget, CDvrWin2::cDepth.t2d);
	// ADD-BY-LEETEN 01/12/2010-END

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
	// ADD-BY-LEETEN 01/12/2010-BEGIN
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	// ADD-BY-LEETEN 01/12/2010-END

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
		// MOD-BY-LEETEN 01/01/2010-FROM:
			// if( iMinSlab <= iSlab && iSlab < min(iNrOfSlabs, iMaxSlab) )
		// TO:
		if( iMinSlab <= iSlab && iSlab < min(iNrOfSlabs, iMinSlab + iNrOfSlabsToRender) )
		// MOD-BY-LEETEN 01/01/2010-END
		{
			#if	0	// MOD-BY-LEETEN 01/01/2010-FROM:
				glUseProgramObjectARB(pidImportanceCulling);
				cStreamline._RenderLinesInSlab(iSlab);
			#else	// MOD-BY-LEETEN 01/01/2010-TO:

			#if	0	// MOD-BY-LEETEN 01/03/2010-FROM:
				switch(iShading)
				{
				case SHADING_LIGHTING:
				case SHADING_NO_LIGHTING:
					glUseProgramObjectARB(pidImportanceCulling);
					cStreamline._RenderLinesInSlab(iSlab, false);
					break;

				case SHADING_HALO:
					glPushAttrib(GL_DEPTH_BUFFER_BIT);
					glPushAttrib(GL_COLOR_BUFFER_BIT);

					glDepthFunc(GL_LEQUAL);

					glUseProgramObjectARB(pidImportanceCulling);
					glColorMask(true, true, true, false);
					cStreamline._RenderLinesInSlab(iSlab, true);
					cStreamline._RenderLinesInSlab(iSlab, false);

					glColorMask(false, false, false, true);
					cStreamline._RenderLinesInSlab(iSlab, true);

					glPopAttrib();	// glPushAttrib(GL_DEPTH_BUFFER_BIT);
					glPopAttrib();	// glPushAttrib(GL_COLOR_BUFFER_BIT);
					break;
				}
			#else	// MOD-BY-LEETEN 01/03/2010-TO:
			glPushAttrib(GL_DEPTH_BUFFER_BIT);
			glPushAttrib(GL_COLOR_BUFFER_BIT);

			glColorMask(true, true, true, false);
			// MOD-BY-LEETEN 01/05/2010-FROM:
				// glDepthFunc(GL_LEQUAL);
			// TO:
			glDepthFunc(GL_ALWAYS);
			// MOD-BY-LEETEN 01/05/2010-END

			glUseProgramObjectARB(pidImportanceCulling);

			// render the streamlin to the color buffer
			if( 0 != ibIsHaloEnabled )
			{
				SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsColorMono",			1 );
				cStreamline._RenderLinesInSlab(iSlab, true);
			}

			SET_1I_VALUE_BY_NAME(	pidImportanceCulling, "ibIsColorMono",			ibIsColorMono );
			cStreamline._RenderLinesInSlab(iSlab, false);

			// update the alpha channel
			glColorMask(false, false, false, true);
			cStreamline._RenderLinesInSlab(iSlab, (0!=ibIsHaloEnabled)?true:false);

			glPopAttrib();	// glPushAttrib(GL_DEPTH_BUFFER_BIT);
			glPopAttrib();	// glPushAttrib(GL_COLOR_BUFFER_BIT);
			#endif	// MOD-BY-LEETEN 01/03/2010-END
			#endif	// MOD-BY-LEETEN 01/01/2010-END

		}

		// ADD-BY-LEETEN 01/05/2010-BEGIN
		glPushAttrib(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);
		// ADD-BY-LEETEN 01/05/2010-END

		glUseProgramObjectARB(pidImportanceFilling);
		CDvrWin2::_RenderSlab(
			iSlab, iNrOfSlabs, 

			pdModelviewMatrix, pdProjectionMatrix, piViewport,
			
			dMinX, dMaxX, 
			dMinY, dMaxY, 
			dMinZ, dMaxZ);

		// ADD-BY-LEETEN 01/05/2010-BEGIN
		glPopAttrib();	// glPushAttrib(GL_DEPTH_BUFFER_BIT);
		// ADD-BY-LEETEN 01/05/2010-END

		break;
		
	case RENDER_MODE_STREAMLINES_IN_SLABS:
		#if	0	// MOD-BY-LEETEN 01/01/2010-FROM:
			if( iMinSlab <= iSlab && iSlab < min(iNrOfSlabs, iMaxSlab) )
				cStreamline._RenderLinesInSlab(iSlab, true);
		#else	// MOD-BY-LEETEN 01/01/2010-TO:
		if( iMinSlab <= iSlab && iSlab < min(iNrOfSlabs, iMinSlab + iNrOfSlabsToRender) )
		{
			glPushAttrib(GL_DEPTH_BUFFER_BIT);
			glDepthFunc(GL_LEQUAL);
			cStreamline._RenderLinesInSlab(iSlab, true);
			cStreamline._RenderLinesInSlab(iSlab, false);
			glPopAttrib();	// glPushAttrib(GL_DEPTH_BUFFER_BIT);
		}
		#endif	// MOD-BY-LEETEN 01/01/2010-END

		break;
	}
	// ADD-BY-LEETEN 01/12/2010-BEGIN
	glPopAttrib();	// glPushAttrib(GL_DEPTH_BUFFER_BIT);
	// ADD-BY-LEETEN 01/12/2010-END
}


void 
CFlowEntropyViewerWin::_EndDisplay()
{
	glUseProgramObjectARB(0);

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	if( 0 != cSphericalHistogram.ibIsEnabled )
		cSphericalHistogram._DrawHistogrm();
	// ADD-BY-LEETEN 01/30/2010-END

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
	// ADD-BY-LEETEN 01/05/2010-BEGIN
	CClipVolume::_ReshapeFunc(w, h);
	// ADD-BY-LEETEN 01/05/2010-END
	CDvrWin2::_ReshapeFunc(w, h);
}

void 
CFlowEntropyViewerWin::_InitFunc()
{
	CDvrWin2::_InitFunc();

	#if	0	// DEL-BY-LEETEN 01/08/2010-BEGIN
		_DisableVerticalSync();
		_KeepUpdateOn();
		_DisplayFpsOn();
	#endif	// DEL-BY-LEETEN 01/08/2010-END

	// ADD-BY-LEETEN 01/10/2010-BEGIN
	_KeepUpdateOn();
	// ADD-BY-LEETEN 01/10/2010-END

	_DisplayFpsOn();	// TEST-ADD

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	CClipVolume::_InitFunc();
	// ADD-BY-LEETEN 01/05/2010-END

	///////////////////////////////////////////////////////////////////
	pidRayIntegral = CSetShadersByString(
		NULL
		,
		#include "ray_integral.frag.h"	
	);
	assert( pidRayIntegral );	

	///////////////////////////////////////////////////////////////////
	#if	0	// MOD-BY-LEETEN 12/31/2009-FROM:
		pidImportanceCulling = CSetShadersByString(
			NULL
			,
			#include "importance_culling.frag.h"	
		);
	#else	// MOD-BY-LEETEN 12/31/2009-TO:
	#if	0	// MOD-BY-LEETEN 01/12/2010-FROM:
		pidImportanceCulling = CSetShadersByString(
			#include "line_illumination.vert.h"	
			,
			#include "importance_culling.frag.h"	
		);
	#else	// MOD-BY-LEETEN 01/12/2010-TO:
	pidImportanceCulling = HCreateProgramHandle();
	_AddShaderProgram(
		pidImportanceCulling, GL_VERTEX_SHADER_ARB,		
		#include "line_illumination.vert.h"
	);

	_AddShaderProgram(
		pidImportanceCulling, GL_FRAGMENT_SHADER_ARB,		
		#include "importance_culling.frag.h"
	);

	_AddShaderProgram(
		pidImportanceCulling, GL_GEOMETRY_SHADER_EXT,		
		#include "line_drawing.geom.h"
	);
	glProgramParameteriEXT(pidImportanceCulling, GL_GEOMETRY_INPUT_TYPE_EXT,	GL_LINES);
	glProgramParameteriEXT(pidImportanceCulling, GL_GEOMETRY_OUTPUT_TYPE_EXT,	GL_LINE_STRIP);
	glProgramParameteriEXT(pidImportanceCulling, GL_GEOMETRY_VERTICES_OUT_EXT,	2 * 9);	

	_LinkPrograms(pidImportanceCulling);
	#endif	// MOD-BY-LEETEN 01/12/2010-END

	#endif	// MOD-BY-LEETEN 12/31/2009-END
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

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	// MOD-BY-LEETEN 01/10/2010-FROM:
		// GLUI_Panel *pcPanel_ClippingPlane = PCGetGluiWin()->add_panel("Clipping Plane");
	// TO:
	GLUI_Panel *pcPanel_ClippingPlane = PCGetGluiWin()->add_rollout("Clipping Plane");
	// MOD-BY-LEETEN 01/10/2010-END
	{
		#if	0		// DEL-BY-LEETEN 01/07/2010-BEGIN
			GLUI_Spinner *pcSpinner_Threshold = PCGetGluiWin()->add_spinner_to_panel(pcPanel_ClippingPlane, "Clipping Threshold", GLUI_SPINNER_FLOAT, &fClippingThreshold);	
				pcSpinner_Threshold->set_float_limits(0.0f, 1.0f);
		#endif		// DEL-BY-LEETEN 01/07/2010-END

		#if	0	// MOD-BY-LEETEN 01/07/2010-FROM:
			GLUI_Panel *pcPanel_Colors = PCGetGluiWin()->add_panel_to_panel(pcPanel_ClippingPlane, "Colors");	
				GLUI_Panel *pcPanel_Front = PCGetGluiWin()->add_panel_to_panel(pcPanel_Colors, "Front");	
		#else	// MOD-BY-LEETEN 01/07/2010-TO:
		GLUI_Panel *pcPanel_Outside = PCGetGluiWin()->add_panel_to_panel(pcPanel_ClippingPlane, "Outside");	
		#endif	// MOD-BY-LEETEN 01/07/2010-END
			{
				// ADD-BY-LEETEN 01/07/2010-BEGIN
				GLUI_Spinner *pcSpinner_Threshold = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Outside, "Threshold", GLUI_SPINNER_FLOAT, &cClippingPlaneOutsideProp.fThreshold);	
					pcSpinner_Threshold->set_float_limits(0.0f, 1.0f);
				// ADD-BY-LEETEN 01/07/2010-END
				PCGetGluiWin()->add_checkbox_to_panel(pcPanel_Outside, "mono?", &cClippingPlaneOutsideProp.ibMonoColor);
				GLUI_Spinner *pcSpinner;
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Outside, "R", GLUI_SPINNER_FLOAT, &cClippingPlaneOutsideProp.v4Color.x);	pcSpinner->set_float_limits(0.0, 1.0);
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Outside, "G", GLUI_SPINNER_FLOAT, &cClippingPlaneOutsideProp.v4Color.y);	pcSpinner->set_float_limits(0.0, 1.0);
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Outside, "B", GLUI_SPINNER_FLOAT, &cClippingPlaneOutsideProp.v4Color.z);	pcSpinner->set_float_limits(0.0, 1.0);
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Outside, "A", GLUI_SPINNER_FLOAT, &cClippingPlaneOutsideProp.v4Color.w);	pcSpinner->set_float_limits(0.0, 1.0);
			}
		#if	0	// MOD-BY-LEETEN 01/07/2010-FROM:
			PCGetGluiWin()->add_column_to_panel(pcPanel_Colors);	
			GLUI_Panel *pcPanel_Back = PCGetGluiWin()->add_panel_to_panel(pcPanel_Colors, "Back");	
		#else	// MOD-BY-LEETEN 01/07/2010-TO:
		PCGetGluiWin()->add_column_to_panel(pcPanel_ClippingPlane);	
		GLUI_Panel *pcPanel_Inside = PCGetGluiWin()->add_panel_to_panel(pcPanel_ClippingPlane, "Inside");	
		#endif	// MOD-BY-LEETEN 01/07/2010-END
			{
				// ADD-BY-LEETEN 01/07/2010-BEGIN
				GLUI_Spinner *pcSpinner_Threshold = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Inside, "Threshold", GLUI_SPINNER_FLOAT, &cClippingPlaneInsideProp.fThreshold);	
					pcSpinner_Threshold->set_float_limits(0.0f, 1.0f);
				// ADD-BY-LEETEN 01/07/2010-END
				PCGetGluiWin()->add_checkbox_to_panel(pcPanel_Inside, "mono?", &cClippingPlaneInsideProp.ibMonoColor);
				GLUI_Spinner *pcSpinner;
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Inside, "R", GLUI_SPINNER_FLOAT, &cClippingPlaneInsideProp.v4Color.x);	pcSpinner->set_float_limits(0.0, 1.0);
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Inside, "G", GLUI_SPINNER_FLOAT, &cClippingPlaneInsideProp.v4Color.y);	pcSpinner->set_float_limits(0.0, 1.0);
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Inside, "B", GLUI_SPINNER_FLOAT, &cClippingPlaneInsideProp.v4Color.z);	pcSpinner->set_float_limits(0.0, 1.0);
				pcSpinner = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Inside, "A", GLUI_SPINNER_FLOAT, &cClippingPlaneInsideProp.v4Color.w);	pcSpinner->set_float_limits(0.0, 1.0);
			}
	}
	// ADD-BY-LEETEN 01/05/2010-END

						// create a spinner to control the brightness gain 
	// MOD-BY-LEETEN 01/10/2010-FROM:
		// GLUI_Panel *pcPanel_RenderMode = PCGetGluiWin()->add_panel("Render Mode");
	// TO:
	GLUI_Panel *pcPanel_RenderMode = PCGetGluiWin()->add_rollout("Render Mode");
	// MOD-BY-LEETEN 01/10/2010-END
	GLUI_RadioGroup *pcRadioGroup_RenderMode = PCGetGluiWin()->add_radiogroup_to_panel(pcPanel_RenderMode, &iRenderMode);
	// ADD-BY-LEETEN 01/30/2010-BEGIN
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "None");
	// ADD-BY-LEETEN 01/30/2010-END
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "Entropy Field");
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "Streamlines w/ Importance Culling");
	PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_RenderMode, "Streamlines in Slabs");

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	cSphericalHistogram._AddGlui(PCGetGluiWin(), NULL);
	// ADD-BY-LEETEN 01/30/2010-END

	// ADD-BY-LEETEN 01/01/2010-BEGIN
		// MOD-BY-LEETEN 01/10/2010-FROM:
			// GLUI_Panel *pcPanel_Shading = PCGetGluiWin()->add_panel("Shading");
		// TO:
		GLUI_Panel *pcPanel_Shading = PCGetGluiWin()->add_rollout("Shading");
		// MOD-BY-LEETEN 01/10/2010-END

	#if	0	// MOD-BY-LEETEN 01/03/2010-FROM:
		GLUI_RadioGroup *pcRadioGroup_Shading = PCGetGluiWin()->add_radiogroup_to_panel(pcPanel_Shading, &iShading);
		PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_Shading, "no lighting");
		PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_Shading, "lighting");
		PCGetGluiWin()->add_radiobutton_to_group(pcRadioGroup_Shading, "halo");
	#else	// MOD-BY-LEETEN 01/03/2010-TO:
	PCGetGluiWin()->add_checkbox_to_panel(pcPanel_Shading, "mono?",		&ibIsColorMono);
	PCGetGluiWin()->add_checkbox_to_panel(pcPanel_Shading, "lighting?", &ibIsLightingEnabled);
	PCGetGluiWin()->add_checkbox_to_panel(pcPanel_Shading, "halo?",		&ibIsHaloEnabled);
	#endif	// MOD-BY-LEETEN 01/03/2010-END

	// ADD-BY-LEETEN 01/03/2010-BEGIN
	#if	0	// DEL-BY-LEETEN 01/10/2010-BEGIN
		GLUI_Spinner *pcSpinner_MaxLengthToNeighbors = PCGetGluiWin()->add_spinner("Max. Dist.",	GLUI_SPINNER_INT, &iMaxDistanceToNeighbors_screen);	
			pcSpinner_MaxLengthToNeighbors->set_int_limits(0, 5);
	#endif	// DEL-BY-LEETEN 01/10/2010-END
	// ADD-BY-LEETEN 01/03/2010-END

	// MOD-BY-LEETEN 01/10/2010-FROM:
		// cMaterial._AddGlui(PCGetGluiWin());
	// TO:
	cMaterial._AddGlui(PCGetGluiWin(), pcPanel_Shading);
	// MOD-BY-LEETEN 01/10/2010-END
	// ADD-BY-LEETEN 01/01/2010-END
	

	// MOD-BY-LEETEN 01/10/2010-FROM:
		// GLUI_Panel *pcPanel_Slabs = PCGetGluiWin()->add_panel("Slab");
	// TO:
	GLUI_Panel *pcPanel_Slabs = PCGetGluiWin()->add_rollout("Slab");
	// MOD-BY-LEETEN 01/10/2010-END
	PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Min Slab", GLUI_SPINNER_INT, &iMinSlab);	
	// MOD-BY-LEETEN 01/01/2010-FROM:
		// PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Max Slab", GLUI_SPINNER_INT, &iMaxSlab);	
	// TO:
	PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "#Slabs",	GLUI_SPINNER_INT, &iNrOfSlabsToRender);	
	// MOD-BY-LEETEN 01/01/2010-END
	GLUI_Spinner *pcSpinner_OcclusionSaturation = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Occlusion Saturation ", GLUI_SPINNER_FLOAT, &fOcclusionSaturation );	
	pcSpinner_OcclusionSaturation->set_float_limits(0.0f, 1.0f);

	// ADD-BY-LEETEN 01/10/2010-BEGIN
	GLUI_Spinner *pcSpinner_MaxLengthToNeighbors = PCGetGluiWin()->add_spinner_to_panel(pcPanel_Slabs, "Max. Dist.",	GLUI_SPINNER_INT, &iMaxDistanceToNeighbors_screen);	
		pcSpinner_MaxLengthToNeighbors->set_int_limits(0, 5);
	// ADD-BY-LEETEN 01/10/2010-END

	// DEL-BY-LEETEN 01/08/2010-BEGIN
		// cStreamline._AddGlui(PCGetGluiWin());
	// DEL-BY-LEETEN 01/08/2010-END

}

/////////////////////////////////////////////////////////////////////
CFlowEntropyViewerWin::CFlowEntropyViewerWin(void)
{
	iMinSlab = 0;
	// MOD-BY-LEETEN 01/01/2010-FROM:
		// iMaxSlab = 1024;
	// TO:
	iNrOfSlabsToRender = 1024;
	// MOD-BY-LEETEN 01/01/2010-END

	// MOD-BY-LEETEN 01/04/2010-FROM:
		// fOcclusionSaturation = 0.0f;
	// TO:
	fOcclusionSaturation = 1.0f;
	// MOD-BY-LEETEN 01/04/2010-END

	fThicknessGain = 1.0f;

	ibIsFboEnabled = 1;			// enable the rendering to FBO
	_SetInternalColorFormat(GL_RGBA32F_ARB);	// set the depths of each chanel of the FBO as 32 bits 
	// ADD-BY-LEETEN 01/06/2010-BEGIN
	_SetInternalDepthFormat(GL_DEPTH_COMPONENT);
	// ADD-BY-LEETEN 01/06/2010-END

	iNrOfSlices = 128;			// set up the default number of slices

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	// DEL-BY-LEETEN 01/07/2010-BEGIN
		// fClippingThreshold = 1.0;
	// DEL-BY-LEETEN 01/07/2010-END
	// ADD-BY-LEETEN 01/05/2010-END

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	// MOD-BY-LEETEN 01/03/2010-FROM:
		// iShading = SHADING_NO_LIGHTING;	
	// TO:
	ibIsLightingEnabled = 0;
	ibIsColorMono		= 1;
	ibIsHaloEnabled		= 1;
	// MOD-BY-LEETEN 01/03/2010-END
	// ADD-BY-LEETEN 01/01/2010-END

	// ADD-BY-LEETEN 01/03/2010-BEGIN
	iMaxDistanceToNeighbors_screen = 0;
	// ADD-BY-LEETEN 01/03/2010-END

	iRenderMode = RENDER_MODE_STREAMLINES_IMPORTANCE_CULLING;
	_AddGluiWin();
}

CFlowEntropyViewerWin::~CFlowEntropyViewerWin(void)
{
}

/*

$Log: not supported by cvs2svn $
Revision 1.9  2010/01/12 23:56:15  leeten

[01/12/2010]
1. [ADD] Incldue the header 'liblog.h'
2. [ADD] Define a function UReadIsosurface() to load the isosurfaces as the basins in the IEEE Vis'06 Design contest benchmark.
3. [ADD] Define a function _LoadData to load the data for builtin datasets.
4. [MOD] Flipe the Z axis s.t the coordinate becomes right hand coordinate.
5. [ADD] Enable depth test when rendering the backfground, and then disable the depth mask so the depth won't be changed.
6. [MOD] Add the geometry shader 'lkine_drawing.geom' to the shader program pidImportanceCulling.

Revision 1.8  2010/01/11 19:16:39  leeten

[01/10/2010]
1. [ADD] Plot the XYZ axes.
2. [ADD] Support the rendering of the streamlines with dashed lines.
3. [ADD] Add back the code to automatically update the frame.
4. [MOD] Change the panels into rollouts.

Revision 1.7  2010/01/09 22:22:42  leeten

[01/09/2010]
1. [MOD] Create the GLUI user control after the streamlins have been loaded in order to get #streamlines.
2. [MOD] chnage the background color from black to white.
3. [MOD] Disable the auto update of the frame and displaying of FPS.

Revision 1.6  2010/01/07 14:58:33  leeten

[01/07/2010]
1. [MOD] Change the variable names such that the front/back sides of the clipping volume become the outside/inside of the clipping volume.
2. [ADD] Add thresholds for both sides of the clipping volume.

Revision 1.5  2010/01/06 17:23:49  leeten

[01/06/2010]
1. [ADD] Support clipping against clipping planes.
2. [ADD] Specify the variable t2dClipVolume to the shader program pidImportanceFilling.
3. [ADD] Specify the variables t1dTf, fTfDomainMin, fTfDomainMax, fDataValueMin, and fDataValueMax to the shader program pidImportanceFilling.
4. [ADD] Specify the variables v4ClippingPlaneOutsideColor, ibIsClippingPlaneFrontColorMon, v4ClippingPlaneInsideColor, and ibIsClippingPlaneInsideColorMono .
5. [ADD] Specify the variable fClippingThreshold and t2dClipVolume to the shader program pidImportanceCulling.
6. [ADD] Bind the 4th texture to the clipping volume.
7. [MOD] Change the depth func for importance filling stage from GL_LEQUAL to GL_ALWAYS.
8. [ADD] Add use controls to specify the color scheme for the clipping planes.
9. [ADD] Explictly specify the internal format of the depth component.

Revision 1.4  2010/01/04 18:23:56  leeten

[01/04/2010]
1. [MOD] Change the background color to black.
2. [MOD] Remove the variable 'ibIsHaloEnabled' in the fragment shader and add two new variables: ibIsColorMono, ibIsLightingEnabled and iMaxDistanceToNeighbors_screen.
3. [ADD] Change the shading schemes: three binary options can control the shading style: mono (or via transfer function), lighting and halo.

Revision 1.3  2010/01/01 18:36:32  leeten

[01/01/2010]
1. [ADD] Pass the material for lighting.
2. [ADD] PAss the currnet color buffer for the fragment shader 'importance_filling' to decide whether the importance should be updated.
3. [ADD] Support lighting and halo.
4. [ADD] Add the vertex shader 'line_illuimation.vert' to the shader program pidImportanceCulling.

Revision 1.2  2009/12/31 01:59:54  leeten

[12/30/2009]
1. [ADD] Add the log section.


*/
