
#pragma once

	#include <vector>

	using namespace std;

	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>

	#include <vector_types.h>

	#include <lib3ds/vector.h>

	#include "libbuf.h"

	#include "GlutWin.h"

struct CStreamline
{
	unsigned int uNrOfVertices;
	unsigned int uNrOfLines;
	TBuffer<float> pfCoords;

	GLuint vidLines;	// vbo for the lines

	unsigned int uSamplingRate;
	struct CDash {
		int iPeriod;
		float fOffset;
		float	fThreshold;
		int		ibIsEntropyDependent;

		CDash()
		{
			iPeriod = 0;
			fOffset = 0.0f;
			fThreshold = 0.5f;
			ibIsEntropyDependent = 0;
		}
	}; 
	CDash cDash;

	struct CGlyph {
		int		ibIsEnabled;
		int		iStep;
		float	fLength;
		float	fWidth;

		CGlyph()
		{
			iStep = 1;
			ibIsEnabled = 0;
			fLength = 1.0f;
			fWidth = 1.0f;
		}

	} cGlyph;

	unsigned int uMinNrOfStreamlines;

	unsigned int uMaxNrOfStreamlines;

	TBuffer<float> pfTangent;

						// record the index of each vertex in its streamline
	TBuffer<int4> pi4VertexIndicesInStreamline;

	TBuffer<unsigned int> puLineSegmentIndicesToStreamlines;

	TBuffer<unsigned int> pu2LineSegmentIndicesToVertices;

	TBuffer<int>	piNrOfLinesPerSlab;
	TBuffer<int>	piLineOffsetPerSlab;
	TBuffer<int2>	pi2SlabTemp;
	TBuffer<int2>	pi2Slabs;
	TBuffer<float>	pfLineCentroids;
	TBuffer<unsigned int> pu2SortedLineSegmentIndicesToVertices;
	TBuffer<int2>	pi2BaseLengths;

	GLuint uLid;

	vector<unsigned int> vuNrOfVertices;
	struct CTubes {
		TBuffer<Lib3dsVector> pf3VertexCoords;
		TBuffer<Lib3dsVector> pf3VertexNormals;
		TBuffer<unsigned int> puPatchIndices;

		int iShininess;
		int ifIsWired;

		CTubes()
		{
			iShininess = 0;
			ifIsWired = 0;
		}
	} cTubes;

	void _RenderTubes();
	void _RenderLines();

	void _RenderLinesInSlab(int iSlab, bool bDrawHalo = false);

	float fOuterWidth;
	float fInnerWidth;
	float4 f4Color;

public:
	void _SortSlab
	(
		int iNrOfSlabs, 	
		double pdModelViewMatrix[], 
		double pdProjectionMatrix[], 
		int piViewport[]
	);

	void _CreateTubes(float fTubeWidth);

	void _Read(float fScaleX, float fScalyY, float fScaleZ, char *szStreamlineFilename, int iMaxNrOfLoadedStreamlines);
	void _Render();

	void _AddGlui(GLUI* pcGlui);

	CStreamline(void);
	~CStreamline(void);
};

