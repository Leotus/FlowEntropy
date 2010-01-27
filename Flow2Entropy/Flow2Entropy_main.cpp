/*

This program is used to convert a vector field into a entroopy field.

*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	// standard libraries
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>

	////////////////////////////////////////////////////////////
	// my libraries
	#include "libopt.h"
	#include "libbuf.h"
	#include "libbuf3D.h"

	////////////////////////////////////////////////////////////
	// project-related libraries
	#include "FlowDiffusionCudaLib/FlowDiffusion.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	int iNrOfBins = 360;
	TBuffer3D<float>	p3df3VectorField;
	TBuffer3D<int>		p3diSrcBinVolume;
	TBuffer<float2>		pf2Thetas;
	TBuffer<float2>		pf2Phis;
	TBuffer3D<float>	p3dfEntropyVolume;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
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
_WriteEntropyField(char *szEntorpyFieldFilename)
{
	FILE *fpFile;
	fpFile = fopen(szEntorpyFieldFilename, "wb");
	assert(fpFile);

	fwrite(&p3dfEntropyVolume.iWidth,	sizeof(p3dfEntropyVolume.iWidth),	1, fpFile);
	fwrite(&p3dfEntropyVolume.iHeight,	sizeof(p3dfEntropyVolume.iHeight),	1, fpFile);
	fwrite(&p3dfEntropyVolume.iDepth,	sizeof(p3dfEntropyVolume.iDepth),	1, fpFile);
	fwrite(&p3dfEntropyVolume[0],		sizeof(p3dfEntropyVolume[0]),		p3dfEntropyVolume.USize(), fpFile);
	fclose(fpFile);
}

int IGetBinByAngle(float fTheta, float fPhi, int iNrOfBins, float2* pf2Thetas, float2* pf2Phis)
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
	return -1;
}

void _ReadPatches()
{
	pf2Thetas.alloc(iNrOfBins );
	pf2Phis.alloc(iNrOfBins );

	FILE* fp=fopen("patch.txt","r");
	assert(fp);

	for(int b = 0; b<iNrOfBins ; b++)
	{
		fscanf(fp,"%f, %f", &pf2Thetas[b].x, &pf2Thetas[b].y);
		fscanf(fp,"%f, %f", &pf2Phis[b].x,&pf2Phis[b].y);
	}
	fclose(fp);
}


int 
main(int argn, char* argv[])
{
	////////////////////////////////////////////////////////
	// parse the arguments
	_OPTInit();			// initialize the option parser 

						// specify the .gtc file
	char *szEntropyFieldFilename;
	_OPTAddStringVector(
		"--entropy-field-filename", 1,
		&szEntropyFieldFilename, NULL);

	int iBlockRadius;
	_OPTAddIntegerVector(
		"--entropy-block-radius", 1,
		&iBlockRadius, NULL);

	char *szVectorFieldFilename;
	_OPTAddStringVector(
		"--vector-field-filename", 1,
		&szVectorFieldFilename, NULL);

	assert( BOPTParse(argv, argn, 1) );

	assert( szEntropyFieldFilename );
	assert( szVectorFieldFilename );

	////////////////////////////////////////////////////////
	// read the volume
	_ReadVectorField(szVectorFieldFilename);

	////////////////////////////////////////////////////////
	// read the volume
	_ReadVectorField(szVectorFieldFilename);

	// initialize flow diffusion library
	_FlowDiffusionInit(p3df3VectorField.iWidth, p3df3VectorField.iHeight, p3df3VectorField.iDepth);

	_ReadPatches();

	// setup the bin lookup table to map the angle to bin index
	static const int iNrOfThetas = 720;
	static const int iNrOfPhis = 360;
	static int	ppiAngleMap[iNrOfThetas][iNrOfPhis];
	for(int t = 0; t < iNrOfThetas; t++)
		for(int p = 0; p < iNrOfPhis; p++)
		{
			double dTheta =	M_PI * 2.0 * double(t) / double(iNrOfThetas);
			double dPhi =	M_PI * double(p) / double(iNrOfPhis);
			int iBin = IGetBinByAngle(float(dTheta), float(dPhi), iNrOfBins, &pf2Thetas[0], &pf2Phis[0]);
			if( iBin >= 0 )
				ppiAngleMap[t][p] = iBin;
		}

	// upload the bin lookup table on the GPU device
	_FlowDiffusionSetAngleMap(&ppiAngleMap[0][0], iNrOfPhis, iNrOfThetas);

	// compute the volume of bins on the device side
	_ComputeSrcBinVolume
	(
		p3df3VectorField.iWidth, 
		p3df3VectorField.iHeight, 
		p3df3VectorField.iDepth, 
		iBlockRadius, 
		&p3df3VectorField[0]
	);

	// compute the entropy field on the device side
	p3dfEntropyVolume.alloc(
		p3df3VectorField.iWidth, 
		p3df3VectorField.iHeight, 
		p3df3VectorField.iDepth);
	_ComputeSrcEntropyVolume(iNrOfBins, iBlockRadius, iBlockRadius, iBlockRadius, &p3dfEntropyVolume[0]);

	// write the entropy field to the file
	_WriteEntropyField(szEntropyFieldFilename);

	// terminate the application
	return EXIT_SUCCESS;

}

/*

$Log: not supported by cvs2svn $

*/
