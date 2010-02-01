
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

	// ADD-BY-LEETEN 01/30/2010-BEGIN
	GLuint vidLines;	// vbo for the lines
	// ADD-BY-LEETEN 01/30/2010-END

	// ADD-BY-LEETEN 01/02/2010-BEGIN
	unsigned int uSamplingRate;
	// ADD-BY-LEETEN 01/02/2010-END
	// ADD-BY-LEETEN 01/10/2010-BEGIN
	struct CDash {
		int iPeriod;
		float fOffset;
		// ADD-BY-LEETEN 01/12/2010-BEGIN
		float	fThreshold;
		int		ibIsEntropyDependent;

		CDash()
		{
			iPeriod = 0;
			fOffset = 0.0f;
			fThreshold = 0.5f;
			ibIsEntropyDependent = 0;
		}
		// ADD-BY-LEETEN 01/12/2010-END
	}; 
	CDash cDash;
	// ADD-BY-LEETEN 01/10/2010-END

	// ADD-BY-LEETEN 01/12/2010-BEGIN
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
	// ADD-BY-LEETEN 01/12/2010-END

	// ADD-BY-LEETEN 01/08/2010-BEGIN
	unsigned int uMaxNrOfStreamlines;
	// ADD-BY-LEETEN 01/08/2010-END

	// ADD-BY-LEETEN 12/31/2009-BEGIN
	TBuffer<float> pfTangent;
	// ADD-BY-LEETEN 12/31/2009-END

	// ADD-BY-LEETEN 01/10/2010-BEGIN
						// record the index of each vertex in its streamline
	// MOD-BY-LEETEN 01/19/2010-FROM:
		// TBuffer<int> piVertexIndicesInStreamline;
	// TO:
	TBuffer<int4> pi4VertexIndicesInStreamline;
	// MOD-BY-LEETEN 01/19/2010-END
	// ADD-BY-LEETEN 01/10/2010-END

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
Revision 1.7  2010/01/19 21:12:37  leeten

[01/19/2010]
1. [MOD] Extend the 2nd texture cooridnate from scalars to 4-tuple vectors.

Revision 1.6  2010/01/12 23:42:59  leeten

[01/12/2010]
1. [ADD] Add a member field ibIsEntropyDependent to specify whether the space between dashed lines are dependent to the local entropy.
2. [ADD] Add a new structure CGlyph to define the variables forthe glyph drawing.

Revision 1.5  2010/01/11 19:26:04  leeten

[01/10/2010]
1. [ADD] Add an array piVertexIndicesInStreamline to record the index of each vertice along its streamline. These indices will be passed to the 1st textre unit.
2. [ADD] Add a structure CDash to collect the variables to control the styles of the dashed lines.

Revision 1.4  2010/01/09 22:16:07  leeten

[01/09/2010]
1. [ADD] Define a new field uMaxNrOfStreamlines as max. #streamlines to be rendered.

Revision 1.3  2010/01/04 18:30:07  leeten

[01/04/2010]
1. [MOD] Change names of variables: puLineIndices --> pu2LineSegmentIndicesToVertices; puSortedLineIndices --> pu2SortedLineSegmentIndicesToVertices.
2. [ADD] Add a new variable uSamplingRate and add one new array puLineSegmentIndicesToStreamlines.

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
