	#include <GL/glew.h>
	#include <stdio.h>
	#include <stdlib.h>

						// incldue the headers of my own libraries
	#include "libopt.h"

	#include "FlowEntropyViewerWin.h"
	#include "libtfw/TransFunc.h"
	#include "libtfw/TfWin.h"
	#include "libtfw/Tfui.h"

	// the main display window
	CFlowEntropyViewerWin cFlowEntropyViewerWin;

	// the transfer function
	int iNrOfTfEntries = 4096;
	CTransFunc cTransFunc;
	TBuffer<float> pfTransFunc;
	CTfWin	cTfWin;
	CTfUi	cTfUi;

	// the histogram
	float fMinEntropy;
	float fMaxEntropy;
	TBuffer<float> pfHist;

	TBuffer3D<float> pf3DEntropyField;

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
	float fMaxCount = -HUGE_VAL;
	for(int b = 0; b < iNrOfTfEntries; b++) 
		fMaxCount = max(fMaxCount, pfHist[b]);

	for(int b = 0; b < iNrOfTfEntries; b++) 
		pfHist[b] /= fMaxCount;
}

void
_ReadStreamlines(char *szEntropyFieldFilename)
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

int
main(int argn, char* argv[])
{
	CGlutWin::_Init(	// initialize GlutWin
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

				
	////////////////////////////////////////////////////////
	// parse the arguments
	_OPTInit();			// initialize the option parser 

						// specify the .gtc file
	char *szEntropyFieldFilename;
	_OPTAddStringVector(
		"--entropy-field-filename", 1,
		&szEntropyFieldFilename, NULL);

	char *szStreamlineFilename;
	_OPTAddStringVector(
		"--streamline-filename", 1,
		&szStreamlineFilename, NULL);

	assert( BOPTParse(argv, argn, 1) );

	assert(szEntropyFieldFilename);
	assert(szStreamlineFilename);

	////////////////////////////////////////////////////////
	// read the data
	_ReadEntropyField(szEntropyFieldFilename);

	////////////////////////////////////////////////////////
	cFlowEntropyViewerWin.ICreate("Flow Entropy Viewer");
		cFlowEntropyViewerWin._SetEntropyField(pf3DEntropyField.iWidth, pf3DEntropyField.iHeight, pf3DEntropyField.iDepth, &pf3DEntropyField[0]);
		cFlowEntropyViewerWin._ReadStreamlines(szStreamlineFilename);

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
		cTfWin._Reshape(512, 128);
		cTfWin._KeepUpdateOn();
		cTfWin._DisableVerticalSync();

	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, fMinEntropy, fMaxEntropy);
		cTfUi._SetUpdateFunc(_UpdateTf);
		cTfUi._DisableVerticalSync();

	glutMainLoop();		// enter the GLUT event loop
}

/*

$Log: not supported by cvs2svn $

*/
