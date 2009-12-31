
#pragma once

	// ADD-BY-LEETEN 2009/05/15-BEGIN
	#include <vector>

	using namespace std;
	// ADD-BY-LEETEN 2009/05/15-END

	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>

	#include <vector_types.h>

	// ADD-BY-LEETEN 2009/05/15-BEGIN
	#include <lib3ds/vector.h>
	// ADD-BY-LEETEN 2009/05/15-END

	#include "libbuf.h"

	#include "GlutWin.h"

struct CStreamline
{
	unsigned int uNrOfVertices;
	unsigned int uNrOfLines;
	TBuffer<float> pfCoords;

	TBuffer<unsigned int> puLineIndices;

	TBuffer<int2>	pi2Slabs;
	TBuffer<float>	pfLineCentroids;
	TBuffer<unsigned int> puSortedLineIndices;
	TBuffer<int2>	pi2BaseLengths;

	GLuint uLid;

	// ADD-BY-LEETEN 2009/05/15-BEGIN
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
	// ADD-BY-LEETEN 2009/05/15-END

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

	// ADD-BY-LEETEN 2009/05/15-BEGIN
	void _CreateTubes(float fTubeWidth);
	// ADD-BY-LEETEN 2009/05/15-END

	void _Read(float fScaleX, float fScalyY, float fScaleZ, char *szStreamlineFilename);
	void _Render();

	void _AddGlui(GLUI* pcGlui);

	CStreamline(void);
	~CStreamline(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.2  2009/05/15 20:44:17  leeten

[2009/05/15]
1. [MOD] Move the vector of unsigned int vuNrOfVertices to the class.
2. [ADD] Declare a member method _RenderLines()/_RenderTubes() as the code segment to render the streamlines as line segments and the tubes, respectivelty.
3. [ADD] Define a subclass CTubes to group the variables for the 3D tubes.
4. [ADD] Declare a member method _CreateTube() to create the 3D tubes.

Revision 1.1  2009/05/12 18:54:27  leeten

[2009/05/12]
1. [1ST] This project defines an GLUT/GLUI viewer to display the PRIs.


*/
