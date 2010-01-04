
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

	// ADD-BY-LEETEN 01/02/2010-BEGIN
	unsigned int uSamplingRate;
	// ADD-BY-LEETEN 01/02/2010-END

	// ADD-BY-LEETEN 12/31/2009-BEGIN
	TBuffer<float> pfTangent;
	// ADD-BY-LEETEN 12/31/2009-END

	// ADD-BY-LEETEN 01/02/2010-BEGIN
	TBuffer<unsigned int> puLineSegmentIndicesToStreamlines;
	// ADD-BY-LEETEN 01/02/2010-END

	// MOD-BY-LEETEN 01/02/2010-FROM:	
		// TBuffer<unsigned int> puLineIndices;
	// TO:
	TBuffer<unsigned int> pu2LineSegmentIndicesToVertices;
	// MOD-BY-LEETEN 01/02/2010-END

	TBuffer<int2>	pi2Slabs;
	TBuffer<float>	pfLineCentroids;
	// MOD-BY-LEETEN 01/02/2010-FROM:
		// TBuffer<unsigned int> puSortedLineIndices;
	// TO:
	TBuffer<unsigned int> pu2SortedLineSegmentIndicesToVertices;
	// MOD-BY-LEETEN 01/02/2010-END
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
Revision 1.2  2010/01/01 18:19:40  leeten

[01/01/2010]
1. [ADD] Declare a new field pfTangent as the tangent vector on each vertex.

Revision 1.1  2009/12/31 01:53:59  leeten

[12/30/2009]
1. [1ST] First time checkin.

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
