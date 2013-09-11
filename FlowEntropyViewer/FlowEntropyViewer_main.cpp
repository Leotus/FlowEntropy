	#include <GL/glew.h>
	#include <stdio.h>
	#include <stdlib.h>

	// ADD-BY-LEETEN 08/09/2010-BEGIN
	#include "liblog.h"
	// ADD-BY-LEETEN 08/09/2010-END

						// incldue the headers of my own libraries
	#include "libopt.h"

	#include "FlowEntropyViewerWin.h"
	#include "libtfw/TransFunc.h"
	#include "libtfw/TfWin.h"
	#include "libtfw/Tfui.h"

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	#include "libclip/ClipWin.h"
	// ADD-BY-LEETEN 01/05/2010-END

	// the main display window
	CFlowEntropyViewerWin cFlowEntropyViewerWin;

	// the transfer function
	// MOD-BY-LEETEN 01/08/2010-FROM:
		// int iNrOfTfEntries = 4096;
	// TO:
	int iNrOfTfEntries = 256;
	// MOD-BY-LEETEN 01/08/2010-END

	// ADD-BY-LEETEN 01/02/2010-BEGIN
	float fMaxCount;
	// ADD-BY-LEETEN 01/02/2010-END
	CTransFunc cTransFunc;
	TBuffer<float> pfTransFunc;
	CTfWin	cTfWin;
	CTfUi	cTfUi;

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	CClipWin cClipWin;
	// ADD-BY-LEETEN 01/05/2010-END
	// the histogram
	float fMinEntropy;
	float fMaxEntropy;
	TBuffer<float> pfHist;

	TBuffer3D<float> pf3DEntropyField;

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	TBuffer3D<float>	p3df3VectorField;
	// ADD-BY-LEETEN 01/30/2010-END

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
	// MOD-BY-LEETEN 01/02/2010-BEGIN
		// float fMaxCount = -HUGE_VAL;
	// TO:
	fMaxCount = -HUGE_VAL;
	// MOD-BY-LEETEN 01/02/2010-END
	for(int b = 0; b < iNrOfTfEntries; b++) 
		fMaxCount = max(fMaxCount, pfHist[b]);

	for(int b = 0; b < iNrOfTfEntries; b++) 
		pfHist[b] /= fMaxCount;
}

#if	0	// DEL-BY-LEETEN 03/28/2010-BEGIN
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
#endif	// DEL-BY-LEETEN 03/28/2010-END

// ADD-BY-LEETEN 01/30/2010-BEGIN
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
// ADD-BY-LEETEN 01/30/2010-END

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

// ADD-BY-LEETEN 01/05/2010-BEGIN
void 
_GlobalFunc(int iWid, unsigned int uiCbId, va_list vaArgs)
{
	// ADD-BY-LEETEN 2009/08/26-BEGIN
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
	// ADD-BY-LEETEN 2009/08/26-END
}
// ADD-BY-LEETEN 01/05/2010-END

int
main(int argn, char* argv[])
{
	CGlutWin::_Init(	// initialize GlutWin
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

				
	////////////////////////////////////////////////////////
	// parse the arguments
	_OPTInit();			// initialize the option parser 

	// ADD-BY-LEETEN 01/12/2010-BEGIN
	int iDataName;
	_OPTAddEnum(
		"--data-name", (int*)&iDataName, (int)CFlowEntropyViewerWin::DATA_NAME_NONE, 3, 
		"none",			CFlowEntropyViewerWin::DATA_NAME_NONE, 
		"isabel",		CFlowEntropyViewerWin::DATA_NAME_ISABEL,
		"earthquake",	CFlowEntropyViewerWin::DATA_NAME_EARTHQUAKE);
	// ADD-BY-LEETEN 01/12/2010-END

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	char *szVectorFieldFilename;
	_OPTAddStringVector(
		"--vector-field-filename", 1,
		&szVectorFieldFilename, NULL);
	// ADD-BY-LEETEN 01/30/2010-END

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	int iMaxNrOfLoadedStreamlines = -1;
	_OPTAddIntegerVector(
		"--max-nr-of-loaded-streamlines", 1,
		&iMaxNrOfLoadedStreamlines, iMaxNrOfLoadedStreamlines);
	// ADD-BY-LEETEN 01/30/2010-END

						// specify the .gtc file
	char *szEntropyFieldFilename;
	_OPTAddStringVector(
		"--entropy-field-filename", 1,
		&szEntropyFieldFilename, NULL);

	char *szStreamlineFilename;
	_OPTAddStringVector(
		"--streamline-filename", 1,
		&szStreamlineFilename, NULL);

	// MOD-BY-LEETEN 08/09/2010-FROM:
		// assert( BOPTParse(argv, argn, 1) );
	// TO:
	bool bIsOptParsed = BOPTParse(argv, argn, 1);
	assert( bIsOptParsed );
	// MOD-BY-LEETEN 08/09/2010-END

	assert(szEntropyFieldFilename);
	assert(szStreamlineFilename);

	////////////////////////////////////////////////////////
	// read the data
	_ReadEntropyField(szEntropyFieldFilename);

	////////////////////////////////////////////////////////
	cFlowEntropyViewerWin.ICreate("Flow Entropy Viewer");
		cFlowEntropyViewerWin._SetEntropyField(pf3DEntropyField.iWidth, pf3DEntropyField.iHeight, pf3DEntropyField.iDepth, &pf3DEntropyField[0]);
		// MOD-BY-LEETEN 03/28/2010-FROM:
			// cFlowEntropyViewerWin._ReadStreamlines(szStreamlineFilename);
		// TO:
		cFlowEntropyViewerWin._ReadStreamlines(szStreamlineFilename, iMaxNrOfLoadedStreamlines);
		// MOD-BY-LEETEN 03/28/2010-END
		// ADD-BY-LEETEN 01/12/2010-BEGIN
		cFlowEntropyViewerWin._LoadData(iDataName);
		// ADD-BY-LEETEN 01/12/2010-END

		#if	0	// DEL-BY-LEETEN 2013/09/10-BEGIN
		// ADD-BY-LEETEN 01/30/2010-BEGIN
		#if	0	// MOD-BY-LEETEN 02/01/2010-FROM:
			assert( szVectorFieldFilename );
			_ReadVectorField(szVectorFieldFilename);
			cFlowEntropyViewerWin.cSphericalHistogram._ComputeHistogram(
				p3df3VectorField.iWidth,
				p3df3VectorField.iHeight,
				p3df3VectorField.iDepth,
				&p3df3VectorField[0]);
		#else	// MOD-BY-LEETEN 02/01/2010-TO:
		if( szVectorFieldFilename )
		{
			_ReadVectorField(szVectorFieldFilename);
			cFlowEntropyViewerWin.cSphericalHistogram._ComputeHistogram
				(
				p3df3VectorField.iWidth,
				p3df3VectorField.iHeight,
				p3df3VectorField.iDepth,
				&p3df3VectorField[0]
			);
		}
		else
		{
			cFlowEntropyViewerWin.cSphericalHistogram._ComputeHistogramFromStreamlineTangents
			(
				cFlowEntropyViewerWin.cStreamline.pfTangent.USize()/3,
				&cFlowEntropyViewerWin.cStreamline.pfTangent[0]
			);
		}
		cFlowEntropyViewerWin.cSphericalHistogram._Init();
		#endif	// MOD-BY-LEETEN 02/01/2010-END
		// ADD-BY-LEETEN 01/30/2010-END
		#endif	// DEL-BY-LEETEN 2013/09/10-END

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
		// ADD-BY-LEETEN 01/02/2010-BEGIN
		cTfWin._SetHistogram(&pfHist[0], iNrOfTfEntries, fMaxCount, fMinEntropy, fMaxEntropy);
		// ADD-BY-LEETEN 01/02/2010-END
		cTfWin._Reshape(512, 128);
		cTfWin._KeepUpdateOn();
		cTfWin._DisableVerticalSync();

	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, fMinEntropy, fMaxEntropy);
		cTfUi._SetUpdateFunc(_UpdateTf);
		cTfUi._DisableVerticalSync();

	// ADD-BY-LEETEN 01/05/2010-BEGIN
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
	// ADD-BY-LEETEN 01/05/2010-END

	glutMainLoop();		// enter the GLUT event loop
}

/*

$Log: not supported by cvs2svn $
Revision 1.10  2010/03/29 04:13:47  leeten

[03/28/2010]
1. [DEL] Delete the function _ReadStreamlines().

Revision 1.9  2010/03/29 04:04:25  leeten

[03/28/2010]
1. [ADD] Add one new switches '--max-nr-of-loaded-streamlines <int>' sepcify the max. #streamlines to be loaded. The parameter is then passed to the method CFlowEntropyViewerWin::_ReadStreamlines().

Revision 1.8  2010/02/02 03:32:50  leeten

[02/01/2010]
1. [MOD] If the switch --vector-field-filename is absent, the tangents of the streamlines are used to construct the polar histogram.
2. [ADD] Call the function cFlowEntropyViewerWin.cSphericalHistogram._Init() to initialize the OpenGL context.

Revision 1.7  2010/02/01 06:18:59  leeten

[01/31/2010]
1. [ADD] Add a new switch "--vector-field-filename <filename>" to define the input vector field.
2. [ADD] Add a new function _ReadVectorField to load the vector field.

Revision 1.6  2010/01/12 23:57:13  leeten

[01/12/2010]
1. [ADD] Add a new switch '--data-name  <string>' to load the builtin dataset.

Revision 1.5  2010/01/09 22:24:33  leeten

[01/09/2010]
1. [MOD] Chnage #entries in transfe rfunction from 4096 to 256 in order to draw the histogram w/o aliasing.

Revision 1.4  2010/01/06 17:07:20  leeten

[01/06/2010]
1. [ADD] Add a window cClipWin to specify the clipping planes.
2. [ADD] Add a function _GlobalFunc to bypass the message among the windows.

Revision 1.3  2010/01/04 18:19:18  leeten

[01/04/2010]
1. [MOD] Move the variable fMaxCount as a global variable, and specify the histogram to the window cTfWin.

Revision 1.2  2009/12/31 01:59:53  leeten

[12/30/2009]
1. [ADD] Add the log section.


*/
