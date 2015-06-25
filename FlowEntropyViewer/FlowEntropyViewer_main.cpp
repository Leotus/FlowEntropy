	#include <GL/glew.h>
	#include <stdio.h>
	#include <stdlib.h>

	#include "liblog.h"

						// incldue the headers of my own libraries
	#include "libopt.h"

	#include "FlowEntropyViewerWin.h"
	#include "libtfw/TransFunc.h"
	#include "libtfw/TfWin.h"
	#include "libtfw/Tfui.h"

	#include "libclip/ClipWin.h"

	// the main display window
	CFlowEntropyViewerWin cFlowEntropyViewerWin;

	// the transfer function
	int iNrOfTfEntries = 256;

	float fMaxCount;
	CTransFunc cTransFunc;
	TBuffer<float> pfTransFunc;
	CTfWin	cTfWin;
	CTfUi	cTfUi;

	CClipWin cClipWin;
	// the histogram
	float fMinEntropy;
	float fMaxEntropy;
	TBuffer<float> pfHist;

	TBuffer3D<float> pf3DEntropyField;

	TBuffer3D<float>	p3df3VectorField;

void
_ReadEntropyField(char *szEntropyFieldFilename)
{
	FILE *fpFile;
	fpFile = fopen(szEntropyFieldFilename, "rb");
	assert(fpFile);

	fread(&pf3DEntropyField.iWidth,		sizeof(pf3DEntropyField.iWidth),	1, fpFile);
	fread(&pf3DEntropyField.iHeight,	sizeof(pf3DEntropyField.iHeight),	1, fpFile);
	fread(&pf3DEntropyField.iDepth,		sizeof(pf3DEntropyField.iDepth),	1, fpFile);
	pf3DEntropyField.alloc(pf3DEntropyField.iWidth, pf3DEntropyField.iHeight, pf3DEntropyField.iDepth);
	assert( pf3DEntropyField.BIsAllocated() );
	fread(&pf3DEntropyField[0],			sizeof(pf3DEntropyField[0]),	pf3DEntropyField.iWidth * pf3DEntropyField.iHeight * pf3DEntropyField.iDepth, fpFile);
	fclose(fpFile);

	/////////////////////////////////////////////////////////
	fMinEntropy = 0.0f;
	fMaxEntropy = -(float)HUGE_VAL;
	for(int v = 0,	z = 0; z < pf3DEntropyField.iDepth;		z++)
		for(int		y = 0; y < pf3DEntropyField.iHeight;	y++)
			for(int x = 0; x < pf3DEntropyField.iWidth;		x++, v++)
			{
				fMaxEntropy = max(fMaxEntropy, pf3DEntropyField[v]);
			}

	pfHist.alloc(iNrOfTfEntries);
	for(int v = 0,	z = 0; z < pf3DEntropyField.iDepth;		z++)
		for(int		y = 0; y < pf3DEntropyField.iHeight;	y++)
			for(int x = 0; x < pf3DEntropyField.iWidth;		x++, v++)
			{
				int b = int(float(iNrOfTfEntries) * pf3DEntropyField[v] / fMaxEntropy);
				b = min(b, iNrOfTfEntries - 1);
				pfHist[b] += 1.0f;
			}

	// normalize the histogram
	fMaxCount = -HUGE_VAL;
	for(int b = 0; b < iNrOfTfEntries; b++) 
		fMaxCount = max(fMaxCount, pfHist[b]);

	for(int b = 0; b < iNrOfTfEntries; b++) 
		pfHist[b] /= fMaxCount;
}

void
_ReadVectorField(char *szVectorFieldFilename)
{
	FILE *fpFile;
	fpFile = fopen(szVectorFieldFilename, "rb");
	assert(fpFile);

	fread(&p3df3VectorField.iWidth,	sizeof(p3df3VectorField.iWidth),	1, fpFile);
	fread(&p3df3VectorField.iHeight,	sizeof(p3df3VectorField.iHeight),	1, fpFile);
	fread(&p3df3VectorField.iDepth,	sizeof(p3df3VectorField.iDepth),	1, fpFile);
	p3df3VectorField.alloc(3 * p3df3VectorField.iWidth * p3df3VectorField.iHeight * p3df3VectorField.iDepth);
	assert( p3df3VectorField.BIsAllocated() );
	fread(&p3df3VectorField[0],		sizeof(p3df3VectorField[0]),		p3df3VectorField.USize(), fpFile);
	fclose(fpFile);
}

void
_UpdateTf()
{
	cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);

	float fTfDomainMin, fTfDomainMax;
	cTransFunc._GetTfDomain(&fTfDomainMin, &fTfDomainMax);

	cFlowEntropyViewerWin._SetTfDomain(fTfDomainMin, fTfDomainMax);
	cFlowEntropyViewerWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);
	cFlowEntropyViewerWin._Redisplay();
}

void 
_GlobalFunc(int iWid, unsigned int uiCbId, va_list vaArgs)
{
	if( cClipWin.IGetId() == iWid )
	{
		switch(uiCbId)
		{
		case CGlutWin::CB_MANUAL:
			{
				unsigned int uEvent = va_arg(vaArgs, unsigned int);
				switch(uEvent)
				{
				case CClipWin::EVENT_BUTTON_UPDATE:
					{
						int iNrOfEnabledClipPlane = va_arg(vaArgs, int);
						double* pdEnabledClipPlanes = va_arg(vaArgs, double*);
						cFlowEntropyViewerWin._SetClipPlanes(iNrOfEnabledClipPlane, pdEnabledClipPlanes);
					} // case
					break;

				} // switch
			}
			break;
		}
	}
}

int
main(int argn, char* argv[])
{
	CGlutWin::_Init(	// initialize GlutWin
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

				
	////////////////////////////////////////////////////////
	// parse the arguments
	_OPTInit();			// initialize the option parser 

	int iDataName;
	_OPTAddEnum(
		"--data-name", (int*)&iDataName, (int)CFlowEntropyViewerWin::DATA_NAME_NONE, 3, 
		"none",			CFlowEntropyViewerWin::DATA_NAME_NONE, 
		"isabel",		CFlowEntropyViewerWin::DATA_NAME_ISABEL,
		"earthquake",	CFlowEntropyViewerWin::DATA_NAME_EARTHQUAKE);

	char *szVectorFieldFilename;
	_OPTAddStringVector(
		"--vector-field-filename", 1,
		&szVectorFieldFilename, NULL);

	int iMaxNrOfLoadedStreamlines = -1;
	_OPTAddIntegerVector(
		"--max-nr-of-loaded-streamlines", 1,
		&iMaxNrOfLoadedStreamlines, iMaxNrOfLoadedStreamlines);

						// specify the .gtc file
	char *szEntropyFieldFilename;
	_OPTAddStringVector(
		"--entropy-field-filename", 1,
		&szEntropyFieldFilename, NULL);

	char *szStreamlineFilename;
	_OPTAddStringVector(
		"--streamline-filename", 1,
		&szStreamlineFilename, NULL);

	bool bIsOptParsed = BOPTParse(argv, argn, 1);
	assert( bIsOptParsed );

	assert(szEntropyFieldFilename);
	assert(szStreamlineFilename);

	////////////////////////////////////////////////////////
	// read the data
	_ReadEntropyField(szEntropyFieldFilename);

	////////////////////////////////////////////////////////
	cFlowEntropyViewerWin.ICreate("Flow Entropy Viewer");
		cFlowEntropyViewerWin._SetEntropyField(pf3DEntropyField.iWidth, pf3DEntropyField.iHeight, pf3DEntropyField.iDepth, &pf3DEntropyField[0]);
		cFlowEntropyViewerWin._ReadStreamlines(szStreamlineFilename, iMaxNrOfLoadedStreamlines);
		cFlowEntropyViewerWin._LoadData(iDataName);

		////////////////////////////////////////////////////////
		// initialize the transfer func.
		pfTransFunc.alloc(CTransFunc::NR_OF_COLORS * iNrOfTfEntries);

		if( false ) // TEST NULL != szTfPathFilename )
		{
			// cTransFunc.BOpenFile(szTfPathFilename);
		}
		else
		{
			cTransFunc._LoadRainBow();
			cTransFunc._SetTfDomain(fMinEntropy, fMaxEntropy);
		}
		cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);
		float fTfDomainMin, fTfDomainMax;
		cTransFunc._GetTfDomain(&fTfDomainMin, &fTfDomainMax);

		cFlowEntropyViewerWin._SetTfDomain(fTfDomainMin, fTfDomainMax);
		cFlowEntropyViewerWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);

	cTfWin._SetTransFunc(&cTransFunc);
	cTfWin._SetNrOfEntries(iNrOfTfEntries);
	cTfWin.ICreate("Transfer Function",		false, 100, 100, 256, 128);
		// cTfWin._SetHistogram(&cGtcWin.pfHist[0], iNrOfHistBins, cGtcWin.fMaxCount, cGtcWin.fScalarMin, cGtcWin.fScalarMax);
		cTfWin._SetHistogram(&pfHist[0], iNrOfTfEntries, fMaxCount, fMinEntropy, fMaxEntropy);
		cTfWin._Reshape(512, 128);
		cTfWin._KeepUpdateOn();
		cTfWin._DisableVerticalSync();

	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, fMinEntropy, fMaxEntropy);
		cTfUi._SetUpdateFunc(_UpdateTf);
		cTfUi._DisableVerticalSync();

	float fX0 = -pf3DEntropyField.iWidth;
	float fX1 = +pf3DEntropyField.iWidth;
	float fY0 = -pf3DEntropyField.iHeight;
	float fY1 = +pf3DEntropyField.iHeight;
	float fZ0 = -pf3DEntropyField.iDepth;
	float fZ1 = +pf3DEntropyField.iDepth;
	float fMaxDim = (float)max(max(pf3DEntropyField.iWidth, pf3DEntropyField.iHeight), pf3DEntropyField.iDepth);
	fX0 /= fMaxDim;
	fY0 /= fMaxDim;
	fZ0 /= fMaxDim;
	fX1 /= fMaxDim;
	fY1 /= fMaxDim;
	fZ1 /= fMaxDim;
	cClipWin._SetBBox(
		fX0, fY0, fZ0, 
		fX1, fY1, fZ1);
	cClipWin.ICreate("Clipping Planes Window",		false, 100, 100, 384, 256);

	CGlutWin::_RegisterGlobalFunc(_GlobalFunc);

	glutMainLoop();		// enter the GLUT event loop
}

