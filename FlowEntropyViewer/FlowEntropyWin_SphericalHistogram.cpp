	#include <GL/glew.h>

	#include <stdio.h>
	#include <stdlib.h>

	#include <vector_functions.h>

	#include "shader.h"
	#include "liblog.h"
	
	#include "FlowEntropyViewerWin.h"
	#include "FlowDiffusionCudaLib/FlowDiffusion.h"

void
CFlowEntropyViewerWin::CSphericalHistogram::_ComputeHistogram
(
	int iWidth, 
	int iHeight, 
	int iDepth, 
	float pfVectorField[]
)
{
	_FlowDiffusionInit(iWidth, iHeight, iDepth);
	_FlowDiffusionSetAngleMap(&ppiAngleMap[0][0], NR_OF_PHIS, NR_OF_THETAS);

	_ComputeSrcBinVolume
	(
		iWidth, 
		iHeight, 
		iDepth, 
		1, 
		&pfVectorField[0]
	);

	// get the src bin volume
	TBuffer3D<int>		p3diSrcBinVolume;
	p3diSrcBinVolume.alloc(iWidth, iHeight, iDepth);
	_GetSrcBinVolume(&p3diSrcBinVolume[0]);

	// construct the hisotgram
	pfHistogram.alloc(iNrOfBins);
	for(int v = 0,	z = 0; z < p3diSrcBinVolume.iDepth; z++)
		for(int		y = 0; y < p3diSrcBinVolume.iHeight;y++)
			for(int x = 0; x < p3diSrcBinVolume.iWidth; x++, v++)
			{
				pfHistogram[p3diSrcBinVolume[v]] += 1.0f;
			}

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	// output the entropy
	float fEntropy = 0.0f;
	for(int b = 0; b < int(this->pfHistogram.USize()); b++)
	{
		float fProb = this->pfHistogram[b] / float(iWidth * iHeight * iDepth);
		if( fProb > 0.0f )
			fEntropy += -fProb * logf(fProb) / logf(2.0f);
	}
	LOG_VAR(logf(float(iNrOfBins))/logf(2.0f));
	LOG_VAR(fEntropy);
	// ADD-BY-LEETEN 01/30/2010-END

	fMaxCount = -(float)HUGE_VAL;
	for(int b = 0; b < int(this->pfHistogram.USize()); b++)
		fMaxCount = max(fMaxCount, this->pfHistogram[b]);

	// convert the histogram into colors
	pfBinCounters.alloc(4 * iNrOfBins);	

	for(int b = 0; b < int(this->pfHistogram.USize()); b++)
		for(int c = 0; c < 4; c++)
		{
			this->pfBinCounters[4 * b + c] = this->pfHistogram[b];
		}

	#if	0	// DEL-BY-LEETEN 02/01/2010-BEGIN
		// convert the patches from spherical coordinates to cartesian coordinates
		pf4BinCartCoords.alloc(4 * iNrOfBins);
		for(int b = 0; b < iNrOfBins; b++)
		{
			float fTheta0 = pf2Thetas[b].x - M_PI;
			float fTheta1 = pf2Thetas[b].y - M_PI;

			float fPhi0 = M_PI / 2.0f - pf2Phis[b].x;
			float fPhi1 = M_PI / 2.0f - pf2Phis[b].y;

			float4 f4Vec;
			float fLength;

			#define SPHERICAL2CARTESIAN(fTheta, fPhi, f4Dst)	\
			{	\
				f4Vec.x = cosf(fTheta);	\
				f4Vec.y = sinf(fTheta);	\
				f4Vec.z = tan(fPhi);	\
				fLength = sqrtf(f4Vec.x * f4Vec.x + f4Vec.y * f4Vec.y + f4Vec.z * f4Vec.z);\
				f4Vec.x /= fLength;\
				f4Vec.y /= fLength;\
				f4Vec.z /= fLength;\
				f4Vec.w = 1.0f;\
				f4Dst = f4Vec;\
			}\

			SPHERICAL2CARTESIAN(fTheta0, fPhi0, pf4BinCartCoords[b * 4 + 0]);
			SPHERICAL2CARTESIAN(fTheta1, fPhi0, pf4BinCartCoords[b * 4 + 1]);
			SPHERICAL2CARTESIAN(fTheta1, fPhi1, pf4BinCartCoords[b * 4 + 2]);
			SPHERICAL2CARTESIAN(fTheta0, fPhi1, pf4BinCartCoords[b * 4 + 3]);
		}

		glGenBuffers(1, &vidPatches);
		glBindBuffer(GL_ARRAY_BUFFER,	vidPatches);
		glBufferData(GL_ARRAY_BUFFER,	
			sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize() + 
			sizeof(pfBinCounters[0]) * pfBinCounters.USize(),
			NULL, 
			GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,	
			0, 
			sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize(), 
			&pf4BinCartCoords[0]);
		glBufferSubData(GL_ARRAY_BUFFER,	
			sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize(), 
			sizeof(pfBinCounters[0]) * pfBinCounters.USize(),
			&pfBinCounters[0]);
		glBindBuffer(GL_ARRAY_BUFFER,	0);

		// allocate the buffer for the triangle indices 
		piTriangleIndices.alloc(iNrOfBins * 2 * 3);
		for(int b = 0; b < iNrOfBins; b++)
		{
			piTriangleIndices[b*6+0] = 4*b + 0;
			piTriangleIndices[b*6+1] = 4*b + 1;
			piTriangleIndices[b*6+2] = 4*b + 2;

			piTriangleIndices[b*6+3] = 4*b + 2;
			piTriangleIndices[b*6+4] = 4*b + 3;
			piTriangleIndices[b*6+5] = 4*b + 0;
		}
		glGenBuffers(1, &vidTriangleIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	vidTriangleIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,	
			sizeof(piTriangleIndices[0]) * piTriangleIndices.USize(),
			&piTriangleIndices[0],
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	0);

		// compile and link the shader program
		pidHistogram = HCreateProgramHandle();
		_AddShaderProgram(
			pidHistogram, GL_VERTEX_SHADER_ARB,		
			#include "spherical_histogram.vert.h"	
		);

		_AddShaderProgram(
			pidHistogram, GL_GEOMETRY_SHADER_EXT,		
			#include "spherical_histogram.geom.h"
		);
		glProgramParameteriEXT(pidHistogram, GL_GEOMETRY_INPUT_TYPE_EXT,	GL_TRIANGLES);
		glProgramParameteriEXT(pidHistogram, GL_GEOMETRY_OUTPUT_TYPE_EXT,	GL_TRIANGLE_STRIP);
		glProgramParameteriEXT(pidHistogram, GL_GEOMETRY_VERTICES_OUT_EXT,	6 * 3);	

		_LinkPrograms(pidHistogram);

		assert( pidHistogram );	
	#endif	// DEL-BY-LEETEN 02/01/2010-END
}

void 
CFlowEntropyViewerWin::CSphericalHistogram::_DrawHistogrm()
{
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glBindBuffer(GL_ARRAY_BUFFER,			vidPatches);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	vidTriangleIndices);
	glVertexPointer(
		4, 
		GL_FLOAT,	
		sizeof(pf4BinCartCoords[0]) - 4 * sizeof(float),	
		(void*)0
	);
	glEnableClientState(GL_VERTEX_ARRAY);

	glUseProgramObjectARB(	pidHistogram);
	const int iAttribIndex = glGetAttribLocation(pidHistogram, "fBin");
	glVertexAttribPointer(
		iAttribIndex,			// index
		1,			// size
		GL_FLOAT,	// type
		false,		// normalized
		0,			// stride
		(void*)(sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize()));

	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);

	glPushAttrib(GL_POLYGON_BIT);
	glPushAttrib(GL_LINE_BIT);
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glEnableVertexAttribArray(iAttribIndex);

	SET_1F_VALUE_BY_NAME(	pidHistogram, "fOpacity",	fOpacity);
	SET_1F_VALUE_BY_NAME(	pidHistogram, "fMaxCount",	fMaxCount);
	SET_1F_VALUE_BY_NAME(	pidHistogram, "fMaxRadius",	fMaxRadius);
	SET_1F_VALUE_BY_NAME(	pidHistogram, "fRelativeThreshold",  fRelativeThreshold);	// ADD-BY-LEETEN 02/01/2010

	SET_1I_VALUE_BY_NAME(	pidHistogram, "ibDrawTop",	0);
	glDrawElements(GL_TRIANGLES, 6 * (iNrOfBins - 2), GL_UNSIGNED_INT, (void*)(6 * sizeof(piTriangleIndices[0])));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);

	SET_1I_VALUE_BY_NAME(	pidHistogram, "ibDrawTop",	1);
	glDrawElements(GL_TRIANGLES, 6 * (iNrOfBins - 2), GL_UNSIGNED_INT, (void*)(6 * sizeof(piTriangleIndices[0])));

	glDisableVertexAttribArray(iAttribIndex);
	glUseProgramObjectARB(0);

	glPopAttrib();	// glPushAttrib(GL_POLYGON_BIT);
	glPopAttrib();	// glPushAttrib(GL_LINE_BIT);
	glPopAttrib();	// glPushAttrib(GL_COLOR_BUFFER_BIT);
	glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,	0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	0);
	glPopClientAttrib();	// glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

}

void
CFlowEntropyViewerWin::CSphericalHistogram::_AddGlui(GLUI* pcGlui, GLUI_Panel	*pcPanel_Parent)
{
	GLUI_Panel *pcPanel = pcGlui->add_rollout("Spherical Histogram");
	pcGlui->add_checkbox_to_panel(pcPanel, "Enabled?", &ibIsEnabled);
	GLUI_Spinner *pcSpinner_Opacity = pcGlui->add_spinner_to_panel(pcPanel, "Opacity?", GLUI_SPINNER_FLOAT, &fOpacity);
	pcSpinner_Opacity->set_float_limits(0.0f, 1.0f);
	GLUI_Spinner *pcSpinner_MaxRadius = pcGlui->add_spinner_to_panel(pcPanel, "Max Radius", GLUI_SPINNER_FLOAT, &fMaxRadius);
	pcSpinner_MaxRadius->set_float_limits(4.0f, 16.0f);

	// ADD-BY-LEETEN 02/01/2010-BEGIN
	GLUI_Spinner *pcSpinner_PercentageThreshold = pcGlui->add_spinner_to_panel(pcPanel, "Threshold", GLUI_SPINNER_FLOAT, &fRelativeThreshold);
	pcSpinner_PercentageThreshold ->set_float_limits(0.0f, 1.0f);
	// ADD-BY-LEETEN 02/01/2010-END
}

void 
CFlowEntropyViewerWin::CSphericalHistogram::_ReadPatches()
{
	iNrOfBins = 360;

	pf2Thetas.alloc(iNrOfBins);
	pf2Phis.alloc(iNrOfBins);

	FILE* fp=fopen("patch.txt","r");
	assert(fp);

	for(int b = 0; b<iNrOfBins ; b++)
	{
		fscanf(fp,"%f, %f", &pf2Thetas[b].x,	&pf2Thetas[b].y);
		fscanf(fp,"%f, %f", &pf2Phis[b].x,		&pf2Phis[b].y);
	}
	fclose(fp);
}

static
int 
IGetBinByAngle(float fTheta, float fPhi, int iNrOfBins, float2* pf2Thetas, float2* pf2Phis)
{
	for(int i=0; i<iNrOfBins;i++)
	{
		if( 
			(fTheta >= pf2Thetas[i].x)	&& (fTheta<=pf2Thetas[i].y)&&
			(fPhi	>= pf2Phis[i].x)	&& (fPhi<=pf2Phis[i].y)
			)
		{
			return i;
		}
	}
/*
	for(int i=0; i<iNrOfBins;i++)
	{
		if	(
			(fTheta + 2.0 * M_PI >= pf2Thetas[i].x) && (fTheta	+ 2.0 * M_PI	<= pf2Thetas[i].y)&&
			(fPhi				 >= pf2Phis[i].x)	&& (fPhi					<= pf2Phis[i].y)
			)
		{
			return i;
		}
	}
	for(int i=0; i<iNrOfBins;i++)
	{
		if	(
			(fTheta				>= pf2Thetas[i].x)	&& (fTheta					<= pf2Thetas[i].y)&&
			(fPhi + 2.0 * M_PI	>= pf2Phis[i].x)	&& (fPhi	+ 2.0 * M_PI	<= pf2Phis[i].y)
			)
		{
			return i;
		}
	}
	for(int i=0; i<iNrOfBins;i++)
	{
		if	(
			(fTheta + 2.0 * M_PI >= pf2Thetas[i].x) && (fTheta	+ 2.0 * M_PI	<= pf2Thetas[i].y)&&
			(fPhi	+ 2.0 * M_PI >= pf2Phis[i].x)	&& (fPhi	+ 2.0 * M_PI	<= pf2Phis[i].y)
			)
		{
			return i;
		}
	}
*/
	assert(true);	
	return -1;
}

// ADD-BY-LEETEN 02/01/2010-BEGIN
void 
CFlowEntropyViewerWin::CSphericalHistogram::_Init()
{
	// convert the patches from spherical coordinates to cartesian coordinates
	pf4BinCartCoords.alloc(4 * iNrOfBins);
	for(int b = 0; b < iNrOfBins; b++)
	{
		float fTheta0 = pf2Thetas[b].x - M_PI;
		float fTheta1 = pf2Thetas[b].y - M_PI;

		float fPhi0 = M_PI / 2.0f - pf2Phis[b].x;
		float fPhi1 = M_PI / 2.0f - pf2Phis[b].y;

		float4 f4Vec;
		float fLength;

		#define SPHERICAL2CARTESIAN(fTheta, fPhi, f4Dst)	\
		{	\
			f4Vec.x = cosf(fTheta);	\
			f4Vec.y = sinf(fTheta);	\
			f4Vec.z = tan(fPhi);	\
			fLength = sqrtf(f4Vec.x * f4Vec.x + f4Vec.y * f4Vec.y + f4Vec.z * f4Vec.z);\
			f4Vec.x /= fLength;\
			f4Vec.y /= fLength;\
			f4Vec.z /= fLength;\
			f4Vec.w = 1.0f;\
			f4Dst = f4Vec;\
		}\

		SPHERICAL2CARTESIAN(fTheta0, fPhi0, pf4BinCartCoords[b * 4 + 0]);
		SPHERICAL2CARTESIAN(fTheta1, fPhi0, pf4BinCartCoords[b * 4 + 1]);
		SPHERICAL2CARTESIAN(fTheta1, fPhi1, pf4BinCartCoords[b * 4 + 2]);
		SPHERICAL2CARTESIAN(fTheta0, fPhi1, pf4BinCartCoords[b * 4 + 3]);
	}

	glGenBuffers(1, &vidPatches);
	glBindBuffer(GL_ARRAY_BUFFER,	vidPatches);
	glBufferData(GL_ARRAY_BUFFER,	
		sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize() + 
		sizeof(pfBinCounters[0]) * pfBinCounters.USize(),
		NULL, 
		GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,	
		0, 
		sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize(), 
		&pf4BinCartCoords[0]);
	glBufferSubData(GL_ARRAY_BUFFER,	
		sizeof(pf4BinCartCoords[0]) * pf4BinCartCoords.USize(), 
		sizeof(pfBinCounters[0]) * pfBinCounters.USize(),
		&pfBinCounters[0]);
	glBindBuffer(GL_ARRAY_BUFFER,	0);

	// allocate the buffer for the triangle indices 
	piTriangleIndices.alloc(iNrOfBins * 2 * 3);
	for(int b = 0; b < iNrOfBins; b++)
	{
		piTriangleIndices[b*6+0] = 4*b + 0;
		piTriangleIndices[b*6+1] = 4*b + 1;
		piTriangleIndices[b*6+2] = 4*b + 2;

		piTriangleIndices[b*6+3] = 4*b + 2;
		piTriangleIndices[b*6+4] = 4*b + 3;
		piTriangleIndices[b*6+5] = 4*b + 0;
	}
	glGenBuffers(1, &vidTriangleIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	vidTriangleIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,	
		sizeof(piTriangleIndices[0]) * piTriangleIndices.USize(),
		&piTriangleIndices[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,	0);

	// compile and link the shader program
	pidHistogram = HCreateProgramHandle();
	_AddShaderProgram(
		pidHistogram, GL_VERTEX_SHADER_ARB,		
		#include "spherical_histogram.vert.h"	
	);

	_AddShaderProgram(
		pidHistogram, GL_GEOMETRY_SHADER_EXT,		
		#include "spherical_histogram.geom.h"
	);
	glProgramParameteriEXT(pidHistogram, GL_GEOMETRY_INPUT_TYPE_EXT,	GL_TRIANGLES);
	glProgramParameteriEXT(pidHistogram, GL_GEOMETRY_OUTPUT_TYPE_EXT,	GL_TRIANGLE_STRIP);
	glProgramParameteriEXT(pidHistogram, GL_GEOMETRY_VERTICES_OUT_EXT,	6 * 3);	

	_LinkPrograms(pidHistogram);

	assert( pidHistogram );	
}

void 
CFlowEntropyViewerWin::CSphericalHistogram::_ComputeHistogramFromStreamlineTangents
(
	int		iNrOfLines, 
	float	pf3Tangents[]
)
{
	pfHistogram.alloc(iNrOfBins);
	for(int l = 0; l < iNrOfLines; l++)
	{
		float fX = pf3Tangents[3*l+0];
		float fY = pf3Tangents[3*l+1];
		float fZ = pf3Tangents[3*l+2];
		float fTheta = (fY != 0.0 || fX != 0.0f)?atan2f(fY, fX):0.0f;
		fTheta += M_PI;
		float fLength = sqrtf(fX * fX + fY * fY);
		float fPhi = (fZ != 0.0 || fLength != 0.0f)?atan2f(fZ, fLength):0.0f;
		fPhi = M_PI/2.0f - fPhi;

		int iTheta	= int(float(NR_OF_THETAS) * fTheta / (2.0 * M_PI)); 
		iTheta = min(max(iTheta, 0), NR_OF_THETAS - 1);
		int iPhi	= int(float(NR_OF_PHIS) * fPhi / (M_PI)); 
		iPhi =	min(max(iPhi, 0), NR_OF_PHIS);

		pfHistogram[ppiAngleMap[iTheta][iPhi]] += 1.0f;
	}

	fMaxCount = -(float)HUGE_VAL;
	for(int b = 0; b < int(this->pfHistogram.USize()); b++)
		fMaxCount = max(fMaxCount, this->pfHistogram[b]);

	// convert the histogram into colors
	pfBinCounters.alloc(4 * iNrOfBins);	

	for(int b = 0; b < int(this->pfHistogram.USize()); b++)
		for(int c = 0; c < 4; c++)
		{
			this->pfBinCounters[4 * b + c] = this->pfHistogram[b];
		}
}
// ADD-BY-LEETEN 02/01/2010-END

CFlowEntropyViewerWin::CSphericalHistogram::CSphericalHistogram()
{
	fMaxRadius = 1.0f;
	fOpacity = 1.0f;
	fRelativeThreshold = 0.05f;	// ADD-BY-LEETEN 02/01/2010-BEGIN

	_ReadPatches();

	// setup the bin lookup table to map the angle to bin index
	for(int t = 0; t < NR_OF_THETAS; t++)
		for(int p = 0; p < NR_OF_PHIS; p++)
		{
			double dTheta =	M_PI * 2.0 * double(t) / double(NR_OF_THETAS);
			double dPhi =	M_PI * double(p) / double(NR_OF_PHIS);
			int iBin = IGetBinByAngle(float(dTheta), float(dPhi), iNrOfBins, &pf2Thetas[0], &pf2Phis[0]);
			if( iBin >= 0 )
				ppiAngleMap[t][p] = iBin;
		}
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/02/01 06:13:29  leeten

[01/31/2010]
1. [1ST] First time checkin.


*/
