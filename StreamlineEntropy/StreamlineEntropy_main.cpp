	#include <vector>
	using namespace std;
	#include <stdio.h>
	#include <stdlib.h>
	#define	_USE_MATH_DEFINES
	#include <math.h>

	#include <vector_types.h>

						// incldue the headers of my own libraries
	#include "libopt.h"
	#include "libbuf.h"
	#include "libbuf3D.h"
	#include "liblog.h"

//////////////////////////////////////////////////////
	TBuffer3D<float[3]> p3Df3VectorField;
	TBuffer3D<float[3]> p3Df3InitVectorField;
	TBuffer3D<float[3]> p3Df3TempVectorField;
	TBuffer3D<float[3]> p3Df3ResultVectorField;
	TBuffer3D<float4>	p3Df4OffsetWeight;

	TBuffer<float[3]>	pf3Coords;
	TBuffer<uint2>		pu2IndicesToVerticesEachStreamline;

	const int iNrOfBins = 360;
	TBuffer<float2>		pf2Thetas;
	TBuffer<float2>		pf2Phis;
	#if	0	// MOD-BY-LEETEN 03/10/2010-FROM:
		int ppiJointHistogram[iNrOfBins][iNrOfBins];
		// MOD-BY-LEETEN 02/19/2010-FROM:
			// int piMarginalHistogram[iNrOfBins];
		// TO:
		int piInputMarginalHistogram[iNrOfBins];
		int piOutputMarginalHistogram[iNrOfBins];
		// MOD-BY-LEETEN 02/19/2010-END
	#else	// MOD-BY-LEETEN 03/10/2010-TO:
	int ppiJointHistogram[iNrOfBins+1][iNrOfBins+1];
	int piInputMarginalHistogram[iNrOfBins+1];
	int piOutputMarginalHistogram[iNrOfBins+1];
	#endif	// MOD-BY-LEETEN 03/10/2010-END

	const int iNrOfThetas = 720;
	const int iNrOfPhis = 360;
	int	ppiAngleMap[iNrOfThetas][iNrOfPhis];

void
_ReadVectorField(char *szVectorFieldFilename)
{
	FILE *fpFile;
	fpFile = fopen(szVectorFieldFilename, "rb");
	assert(fpFile);

	fread(&p3Df3VectorField.iWidth,	sizeof(p3Df3VectorField.iWidth),	1, fpFile);
	fread(&p3Df3VectorField.iHeight,	sizeof(p3Df3VectorField.iHeight),	1, fpFile);
	fread(&p3Df3VectorField.iDepth,	sizeof(p3Df3VectorField.iDepth),	1, fpFile);
	p3Df3VectorField.alloc(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight, p3Df3VectorField.iDepth);
	assert( p3Df3VectorField.BIsAllocated() );
	fread(&p3Df3VectorField[0],		sizeof(p3Df3VectorField[0]),		p3Df3VectorField.USize(), fpFile);
	fclose(fpFile);

	LOG_VAR(szVectorFieldFilename);
}

void
_ReadStreamlines
(
	char *szStreamlineFilename
)
{
	FILE *fpStreamline;
	fpStreamline = fopen(szStreamlineFilename, "rb");
	assert(fpStreamline);

	unsigned int uNrOfStreamlines = 0;
	unsigned int uNrOfVertices = 0;
	unsigned int uNrOfLineSegments = 0;


	fread(&uNrOfStreamlines, sizeof(uNrOfStreamlines), 1, fpStreamline);
	pu2IndicesToVerticesEachStreamline.alloc(uNrOfStreamlines);
	for(unsigned int uS = 0; uS < uNrOfStreamlines; uS++)
	{
		unsigned int uV;
		fread(&uV, sizeof(uV), 1, fpStreamline);
		uNrOfVertices += uV;
		pu2IndicesToVerticesEachStreamline[uS].x = uV;
		if( uS > 0 )
			pu2IndicesToVerticesEachStreamline[uS].y = 
				pu2IndicesToVerticesEachStreamline[uS-1].y + 
				pu2IndicesToVerticesEachStreamline[uS-1].x;
	}

	// LOG_VAR(uNrOfLineSegments);
	// LOG_VAR(uNrOfVertices);

	pf3Coords.alloc(uNrOfVertices);

	fread(&pf3Coords[0], sizeof(pf3Coords[0]), pf3Coords.USize(), fpStreamline);

	fclose(fpStreamline);

	LOG_VAR(szStreamlineFilename);
	LOG_VAR(uNrOfStreamlines);
	LOG_VAR(uNrOfVertices);
}

void 
_ReadPatches()
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

void
_BuildLookupTable()
{
	for(int t = 0; t < iNrOfThetas; t++)
		for(int p = 0; p < iNrOfPhis; p++)
		{
			double dTheta =	M_PI * 2.0 * double(t) / double(iNrOfThetas);
			double dPhi =	M_PI * double(p) / double(iNrOfPhis);
			int iBin = IGetBinByAngle(float(dTheta), float(dPhi), iNrOfBins, &pf2Thetas[0], &pf2Phis[0]);
			// MOD-BY-LEETEN 03/10/2010-FROM:
				// if( iBin >= 0 )
			// TO:
			assert(iBin>=0);
			// MOD-BY-LEETEN 03/10/2010-END

				// MOD-BY-LEETEN 03/10/2010-FROM:
					// ppiAngleMap[t][p] = iBin;
				// TO:
				ppiAngleMap[t][p] = iBin + 1;	// bin zero is preserved
				// MOD-BY-LEETEN 03/10/2010-END
		}
}

double DGetAngle(double x, double y)
{
	if((0.0 == x)&&(0.0 == y))
		return 0.0;
	else
	{
		return (M_PI+(atan2(y,x)));
	}
}

double DGetAngle2(double x, double y)
{
	if((0.0 == x)&&(0.0 == y))
		return 0.0;
	else
	{
		return fabs(M_PI/2.0-(atan2(y,x)));
	}
}


int 
IGetBin(double dX, double dY, double dZ)
{
	double dLength = dX * dX + dY * dY + dZ * dZ;

	if( dLength  > 0.0 )
	{
		dLength  = sqrt(dLength );
		dX /= dLength;
		dY /= dLength;
		dZ /= dLength;
	}
	else
		return 0;

	double dTheta=	DGetAngle(dX, dY);//0~2pi
	double dPhi	=	DGetAngle2(sqrt(dX * dX + dY * dY), dZ);//0~pi
	int iTheta	=	min(iNrOfThetas - 1,	int(double(iNrOfThetas)	* dTheta	/ (M_PI * 2.0)));
	int iPhi	=	min(iNrOfPhis - 1,		int(double(iNrOfPhis)	* dPhi		/ M_PI));
	return ppiAngleMap[iTheta][iPhi];
}

int
main(int argn, char* argv[])
{
	////////////////////////////////////////////////////////
	// parse the arguments
	_OPTInit();			// initialize the option parser 

	char *szVectorFieldFilename;
	_OPTAddStringVector(
		"--vector-field-filename", 1,
		&szVectorFieldFilename, NULL);
	_OPTAddComment(
		"--vector-field-filename", 
		"Specify the filename of the input vector field.");

	char *szStreamlineFilename;
	_OPTAddStringVector(
		"--streamline-filename", 1,
		&szStreamlineFilename, NULL);
	_OPTAddComment(
		"--streamline-filename", 
		"Specify the file that contains the streamlines.");

	// ADD-BY-LEETEN 03/10/2010-BEGIN
	int iMaxNrOfStreamlines;
	_OPTAddIntegerVector(
		"--max-nr-of-streamlines", 1,
		&iMaxNrOfStreamlines, 500);
	_OPTAddComment(
		"--max-nr-of-streamlines", 
		"Specify the max. #streamlines to be tested.");
	// ADD-BY-LEETEN 03/10/2010-END

	assert( BOPTParse(argv, argn, 1) );

	assert(szStreamlineFilename);
	assert(szVectorFieldFilename);

	// read the input data
	_ReadStreamlines(szStreamlineFilename);
	_ReadVectorField(szVectorFieldFilename);

	_ReadPatches();
	_BuildLookupTable();

	int iMaxNrOfIterations = 9 * max(max(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight), p3Df3VectorField.iDepth);
	float fMu = 0.1f;
	double dDiffusionThreshold = 0.995;

	// initialize the vector field
	p3Df3InitVectorField.alloc(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight, p3Df3VectorField.iDepth);
	p3Df3TempVectorField.alloc(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight, p3Df3VectorField.iDepth);
	p3Df3ResultVectorField.alloc(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight, p3Df3VectorField.iDepth);
	p3Df4OffsetWeight.alloc(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight, p3Df3VectorField.iDepth);

	// create the lookup table that map 3D vectors to discrete indices

	for(unsigned int uVI = 0,	uS = 0; uS < pu2IndicesToVerticesEachStreamline.USize(); uS++)
	{
		for(unsigned int		uV = 0; uV < pu2IndicesToVerticesEachStreamline[uS].x; uV++, uVI++)
		{
			int iX = int(floor(pf3Coords[uVI][0]));
			iX = max(min(iX, p3Df3VectorField.iWidth - 1), 0);
			int iY = int(floor(pf3Coords[uVI][1]));
			iY = max(min(iY, p3Df3VectorField.iHeight - 1), 0);
			int iZ = int(floor(pf3Coords[uVI][2]));
			iZ = max(min(iZ, p3Df3VectorField.iDepth - 1), 0);
			int iVoxel = iX + iY * p3Df3VectorField.iWidth + iZ * p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;

			#if	0	// MOD-BY-LEETEN 03/10/2010-FROM:
				float fLength = 0.0;
				for(int i = 0; i < 3; i++)
					fLength += powf(p3Df3VectorField[iVoxel][i], 2.0);

				if(fLength  > 0.0)
				{
					fLength  = sqrt(fLength);
					for(int i = 0; i < 3; i++)
						p3Df3InitVectorField[iVoxel][i] = p3Df3VectorField[iVoxel][i] / fLength;
				}
			#else	// MOD-BY-LEETEN 03/10/2010-TO:
			double dLength = 0.0;
			for(int i = 0; i < 3; i++)
			{
				double d = double(p3Df3VectorField[iVoxel][i]);
				dLength += d * d;
			}

			if(dLength  > 0.0)
			{
				dLength  = sqrt(dLength);
				for(int i = 0; i < 3; i++)
					p3Df3InitVectorField[iVoxel][i] = p3Df3VectorField[iVoxel][i] / float(dLength);
			}
			#endif	// MOD-BY-LEETEN 03/10/2010-END

			// compute the coefficients 
			p3Df4OffsetWeight[iVoxel].w = 
				powf(p3Df3InitVectorField[iVoxel][0], 2.0) + 
				powf(p3Df3InitVectorField[iVoxel][1], 2.0) + 
				powf(p3Df3InitVectorField[iVoxel][2], 2.0);
			p3Df4OffsetWeight[iVoxel].x = p3Df4OffsetWeight[iVoxel].w * p3Df3InitVectorField[iVoxel][0];
			p3Df4OffsetWeight[iVoxel].y = p3Df4OffsetWeight[iVoxel].w * p3Df3InitVectorField[iVoxel][1];
			p3Df4OffsetWeight[iVoxel].z = p3Df4OffsetWeight[iVoxel].w * p3Df3InitVectorField[iVoxel][2];
		}

		memcpy(&p3Df3TempVectorField[0], &p3Df3InitVectorField[0], sizeof(p3Df3TempVectorField[0]) * p3Df3TempVectorField.USize());
		int iIter;
		for(iIter = 0; iIter < iMaxNrOfIterations; iIter++)
		{
			for(int	iVoxel = 0, iZ = 0; iZ < p3Df3VectorField.iDepth;	iZ++)
				for(int			iY = 0; iY < p3Df3VectorField.iHeight;	iY++)
					for(int		iX = 0; iX < p3Df3VectorField.iWidth;	iX++, iVoxel++)
					{
						int iLeft =		max(iX-1, 0)							+ iY * p3Df3VectorField.iWidth + iZ * p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;
						int iRight =	min(iX+1, p3Df3VectorField.iWidth - 1)	+ iY * p3Df3VectorField.iWidth + iZ * p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;
						int iBottom =	iX + max(iY-1, 0)								* p3Df3VectorField.iWidth + iZ * p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;
						int iTop =		iX + min(iY+1, p3Df3VectorField.iHeight - 1)	* p3Df3VectorField.iWidth + iZ * p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;
						int iFront =	iX + iY * p3Df3VectorField.iWidth + max(iZ-1, 0)							* p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;
						int iBack	=	iX + iY * p3Df3VectorField.iWidth + min(iZ+1, p3Df3VectorField.iDepth-1)	* p3Df3VectorField.iWidth * p3Df3VectorField.iHeight;
						for(int j = 0; j < 3; j++)
							p3Df3ResultVectorField[iVoxel][j] =	
								(1.0f - p3Df4OffsetWeight[iVoxel].w) * p3Df3TempVectorField[iVoxel][j] + 
								fMu *(	
										p3Df3TempVectorField[iLeft][j]
									+	p3Df3TempVectorField[iRight][j]
									+	p3Df3TempVectorField[iBottom][j]
									+	p3Df3TempVectorField[iTop][j]
									+	p3Df3TempVectorField[iFront][j]
									+	p3Df3TempVectorField[iBack][j]
									-	6.0f * p3Df3TempVectorField[iVoxel][j]);

						p3Df3ResultVectorField[iVoxel][0] += p3Df4OffsetWeight[iVoxel].x;
						p3Df3ResultVectorField[iVoxel][1] += p3Df4OffsetWeight[iVoxel].y;
						p3Df3ResultVectorField[iVoxel][2] += p3Df4OffsetWeight[iVoxel].z;
					}

			double dDiffusionError = 0.0;
			for(int	iVoxel = 0, iZ = 0; iZ < p3Df3VectorField.iDepth;	iZ++)
				for(int			iY = 0; iY < p3Df3VectorField.iHeight;	iY++)
					for(int		iX = 0; iX < p3Df3VectorField.iWidth;	iX++, iVoxel++)
						for(int j = 0; j < 3; j++)
						{
							float fDiff = p3Df3TempVectorField[iVoxel][j] - p3Df3ResultVectorField[iVoxel][j];
							dDiffusionError += double(fDiff * fDiff);
						}


			dDiffusionError = sqrt(dDiffusionError / double(3 * p3Df3VectorField.USize()));

			// LOG_VAR(dDiffusionError);	
			// ADD-BY-LEETEN 03/10/2010-BEGIN
			fprintf(stderr, "%e\r", dDiffusionError);
			// ADD-BY-LEETEN 03/10/2010-END
			static double dPrevDiffusionError = 0.0;

			if( iIter  > max(max(p3Df3VectorField.iWidth, p3Df3VectorField.iHeight), p3Df3VectorField.iDepth) )
			{
				double dErrorRate = dDiffusionError / dPrevDiffusionError;
				if( dDiffusionThreshold  <= dErrorRate && dErrorRate <= 1.0 )
				{
					// LOG_VAR(dDiffusionError);
					break;
				}

			}
			// ADD-BY-LEETEN 03/10/2010-BEGIN
			if( iIter > 0 && dPrevDiffusionError < dDiffusionError)
			{
				fprintf(stderr, "Error increases. Stop. \n");
				break;
			}
			// ADD-BY-LEETEN 03/10/2010-END

			dPrevDiffusionError  = dDiffusionError;
			memcpy(&p3Df3TempVectorField[0], &p3Df3ResultVectorField[0], sizeof(p3Df3TempVectorField[0]) * p3Df3TempVectorField.USize());
		}
		// LOG_VAR(iIter); 

		// construct the histogram
		memset(ppiJointHistogram, 0,	sizeof(ppiJointHistogram));
		// MOD-BY-LEETEN 02/19/2010-FROM:
			// memset(piMarginalHistogram, 0,	sizeof(piMarginalHistogram));
		// TO:
		memset(piInputMarginalHistogram,	0,	sizeof(piInputMarginalHistogram));
		memset(piOutputMarginalHistogram,	0,	sizeof(piOutputMarginalHistogram));
		// MOD-BY-LEETEN 02/19/2010-END

		// ADD-BY-LEETEN 03/10/2010-BEGIN
		double dRMSE = 0.0;
		double dErrorRate = 0.0;
		// ADD-BY-LEETEN 03/10/2010-END

		for(int	iVoxel = 0, iZ = 0; iZ < p3Df3VectorField.iDepth;	iZ++)
			for(int			iY = 0; iY < p3Df3VectorField.iHeight;	iY++)
				for(int		iX = 0; iX < p3Df3VectorField.iWidth;	iX++, iVoxel++)
				{

					// ADD-BY-LEETEN 03/10/2010-BEGIN
					double dLength;
					
					dLength = 
						pow(double(p3Df3VectorField[iVoxel][0]), 2.0) +  
						pow(double(p3Df3VectorField[iVoxel][1]), 2.0) +  
						pow(double(p3Df3VectorField[iVoxel][2]), 2.0);
					if( dLength > 0.0)
					{
						dLength = sqrt(dLength);
						for(int j = 0; j < 3; j++)
							p3Df3VectorField[iVoxel][j] = float(double(p3Df3VectorField[iVoxel][j])/dLength);
					}

					dLength = 
						pow(double(p3Df3ResultVectorField[iVoxel][0]), 2.0) +  
						pow(double(p3Df3ResultVectorField[iVoxel][1]), 2.0) +  
						pow(double(p3Df3ResultVectorField[iVoxel][2]), 2.0);
					if( dLength > 0.0)
					{
						dLength = sqrt(dLength);
						for(int j = 0; j < 3; j++)
							p3Df3ResultVectorField[iVoxel][j] = float(double(p3Df3ResultVectorField[iVoxel][j])/dLength);
					}

					for(int j = 0; j < 3; j++)
					{
						double dDiff  = double(p3Df3ResultVectorField[iVoxel][j] - p3Df3VectorField[iVoxel][j]);
						dRMSE += dDiff * dDiff;
					}
					// ADD-BY-LEETEN 03/10/2010-END

					int iBin = IGetBin(
						p3Df3VectorField[iVoxel][0], 
						p3Df3VectorField[iVoxel][1], 
						p3Df3VectorField[iVoxel][2]);
					int iNewBin = IGetBin(
						p3Df3ResultVectorField[iVoxel][0], 
						p3Df3ResultVectorField[iVoxel][1], 
						p3Df3ResultVectorField[iVoxel][2]);

					ppiJointHistogram[iNewBin][iBin]++;
					// MOD-BY-LEETEN 02/19/2010-FROM:
						// piMarginalHistogram[iNewBin]++;
					// TO:
					piInputMarginalHistogram[iBin]++;
					piOutputMarginalHistogram[iNewBin]++;
					// MOD-BY-LEETEN 02/19/2010-END

					// ADD-BY-LEETEN 03/10/2010-BEGIN
					if( iNewBin != iBin )
						dErrorRate += 1.0;
					// ADD-BY-LEETEN 03/10/2010-END
				}

		double dJointEntropy = 0.0;
		// MOD-BY-LEETEN 02/19/2010-FROM:
			// double dMarginalEntropy = 0.0;
		// TO:
		double dOutputMarginalEntropy = 0.0;
		double dInputMarginalEntropy = 0.0;
		// MOD-BY-LEETEN 02/19/2010-END

		double dCondEntropy;
		double dNrOfVoxels = double(p3Df3VectorField.USize());

		for(int h = 0; h < iNrOfBins; h++)
		{
			#if	0	// MOD-BY-LEETEN 02/19/2010-FROM:
				if( 0 == piMarginalHistogram[h] )
					continue;

				double dProb = double(piMarginalHistogram[h])/dNrOfVoxels;
				dMarginalEntropy += -dProb * log(dProb)/log(2.0);
			#else	// MOD-BY-LEETEN 02/19/2010-TO:
			if( piInputMarginalHistogram[h] > 0 )
			{
				double dProb = double(piInputMarginalHistogram[h])/dNrOfVoxels;
				dInputMarginalEntropy += -dProb * log(dProb)/log(2.0);
			}

			if( piOutputMarginalHistogram[h] > 0 )
			{
				double dProb = double(piOutputMarginalHistogram[h])/dNrOfVoxels;
				dOutputMarginalEntropy += -dProb * log(dProb)/log(2.0);
			}

			#if	0	// DEL-BY-LEETEN 03/10/2010-BEGIN
				if( 0 == piOutputMarginalHistogram[h] )
					continue;

				double dProb;
			#endif	// DEL-BY-LEETEN 03/10/2010-END

			#endif	// MOD-BY-LEETEN 02/19/2010-END

			for(int w = 0; w < iNrOfBins; w++)
			{
				if( 0 == ppiJointHistogram[h][w] )
					continue;

				double dProb = double(ppiJointHistogram[h][w])/dNrOfVoxels;
				dJointEntropy += -dProb * log(dProb)/log(2.0);
			}
		}

		// ADD-BY-LEETEN 03/10/2010-BEGIN
		LOG_VAR(uS);
		// ADD-BY-LEETEN 03/10/2010-END

		// ADD-BY-LEETEN 03/10/2010-BEGIN
		dRMSE /= double(3 * p3Df3VectorField.USize());
		dRMSE = sqrt(dRMSE);
		LOG_VAR(dRMSE);

		dErrorRate /= double(p3Df3VectorField.USize());
		LOG_VAR(dErrorRate);
		// ADD-BY-LEETEN 03/10/2010-END

		// MOD-BY-LEETEN 02/19/2010-FROM:
			// dCondEntropy = dJointEntropy - dMarginalEntropy;
		// TO:
		dCondEntropy = dJointEntropy - dOutputMarginalEntropy;
		// MOD-BY-LEETEN 02/19/2010-END
		LOG_VAR(dCondEntropy);

		// ADD-BY-LEETEN 02/19/2010-BEGIN
		#if	0	// MOD-BY-LEETEN 03/10/2010-FROM:
		static double dPrevCondEntropy;

		if( uS > 0 )
		{
			double dCondEntropyRatio = dCondEntropy / dPrevCondEntropy;
			if( 0.9999 < dCondEntropyRatio && dCondEntropyRatio <= 1.0)
				break;
		}

		dPrevCondEntropy = dCondEntropy;
		#else	// MOD-BY-LEETEN 03/10/2010-TO:
		if( int(uS) > iMaxNrOfStreamlines )
			break;
		#endif	// MOD-BY-LEETEN 03/10/2010-END
		// ADD-BY-LEETEN 02/19/2010-END

		#if	0	// DEL-BY-LEETEN 03/10/2010-BEGIN
			// ADD-BY-LEETEN 02/19/2010-BEGIN
			double dMutualInformation = dInputMarginalEntropy + dOutputMarginalEntropy - dJointEntropy;
			LOG_VAR(dMutualInformation);
			// ADD-BY-LEETEN 02/19/2010-END
		#endif	// DEL-BY-LEETEN 03/10/2010-END
	}

	// compute the cond. entropy

	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/02/16 19:51:27  leeten

[02/16/2010]
1. [1ST] First time checkin.


*/
