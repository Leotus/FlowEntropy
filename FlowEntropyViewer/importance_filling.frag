/*

This is the shader program to fill the importance

*/

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots
	uniform sampler2D t2dLineFlag;

	uniform sampler2D t2dsDepth;	// the texture hold the depths of each knots

	uniform float fWindowWidth;
	uniform float fWindowHeight;

	#include "/clip_frag_func.frag.h"

	uniform vec4	v4ClippingPlaneOutsideColor;
	uniform int		ibIsClippingPlaneOutsideColorMono;
	uniform vec4	v4ClippingPlaneInsideColor;
	uniform int		ibIsClippingPlaneInsideColorMono;

	#include "/tf1d_frag_func.frag.h"

void 
main()
{
	vec4 v4FragCoord = gl_FragCoord;
	v4FragCoord.x /= fWindowWidth;
	v4FragCoord.y /= fWindowHeight;
	v4FragCoord.w = 1.0 / v4FragCoord.w;

				// read the previous layer
	vec4 v4PrevFragData = texture2D(t2dPrevLayer, v4FragCoord.xy);
	float fPrevV_normalized = v4PrevFragData.g;

				// read the value 
	float fV_normalized = texture3D(t3dVolume, gl_TexCoord[0].xyz).x;

	/////////////////////////////////////////////////////////////////
	vec4 v4Data = vec4(
		v4FragCoord.z, 
		0.0,
		0.0, 
		1.0);
	vec4 v4PrevColor = texture2D(t2dLineFlag, v4FragCoord.xy);
	if( v4PrevColor.a > 0.0 )
		v4Data.g = max(fPrevV_normalized, fV_normalized);
	else
		v4Data.g = 0.0;	

	float fDepth = v4FragCoord.z;

	vec4 v4Color = vec4(0.0);

	float fTolerance = v4PrevFragData.r - v4FragCoord.z; 
	float fDepthWithTolerance = v4FragCoord.z - fTolerance;
	float fPrevDepthWithTolerance = v4PrevFragData.r + fTolerance;
	float fThicknessRatio = FAdjustThickness(fDepthWithTolerance, fPrevDepthWithTolerance, v4FragCoord.xy);
	if( 0.0 < fThicknessRatio && fThicknessRatio < 1.0 )
	{
		float fZNear	= FGetZNear(v4FragCoord.xy);
		float fZFar		= FGetZFar(v4FragCoord.xy);

		if( fDepthWithTolerance < fZFar && fZFar < fPrevDepthWithTolerance )
		{
			fDepth = fZFar;
			if( 0 != ibIsClippingPlaneInsideColorMono )
				v4Color = v4ClippingPlaneInsideColor;
			else
				v4Color = F4GetColorFrom1DTf(fV_normalized);
			v4Color.a = v4ClippingPlaneInsideColor.a;
		}

		if( fDepthWithTolerance < fZNear && fZNear < fPrevDepthWithTolerance )
		{
			fDepth = fZNear;
			if( 0 != ibIsClippingPlaneOutsideColorMono )
				v4Color = v4ClippingPlaneOutsideColor;
			else
				v4Color = F4GetColorFrom1DTf(fV_normalized);
			v4Color.a = v4ClippingPlaneOutsideColor.a;
		}
	}

	float fBackgroundDepth = texture2D(t2dsDepth, v4FragCoord.xy).r;
	if( v4FragCoord.z > fBackgroundDepth )
		v4Color.a = 0.0;

	gl_FragDepth = fDepth;
	gl_FragData[0] = v4Color;

	gl_FragData[1] = v4Data;
}

/*

$Log: not supported by cvs2svn $
Revision 1.6  2010/01/12 23:47:47  leeten

[01/12/2010]
1. [ADD] Add a new texture unit t2dsDepth, which define the background depth. The fragment will be cull out if its depth is larger than the background depth.

Revision 1.5  2010/01/07 15:04:36  leeten

[01/07/2010]
1. [MOD] Change the variable names such that the front/back sides of the clipping volume become the outside/inside of the clipping volume.
2. [ADD] Add thresholds for both sides of the clipping volume.
3. [ADD] Add a threshold when comparing with the clipping volume in order to eliminate the jagging artifact along the intersection between the sclicing planes and the clipping planes.

Revision 1.4  2010/01/06 17:18:42  leeten

[01/06/2010]
1. [ADD] Include the file clip_frag_func.frag.h.
2. [ADD] Add the variable v4ClippingPlaneOutsideColor to specify the color for the front face of the clipping volume and a flag ibIsClippingPlaneOutsideColorMono to decide whether the color is set according to the transfer function when ibIsClippingPlaneOutsideColorMono is 0.
3. [ADD] Add the variable v4ClippingPlaneInsideColor to specify the color for the back face of the clipping volume and a flag ibIsClippingPlaneInsideColorMono to decide whether the color is set according to the transfer function when ibIsClippingPlaneInsideColorMono is 0.
4. [ADD] Include the file tf1d_frag_func.frag.h to look up the transfer function.
5. [ADD] Plot the clipping planes.
6. [ADD] Output the framgent depth. If the fragment is on the boundary of the clipping volume, the depth is adjust to the boundary.

Revision 1.3  2010/01/04 18:33:49  leeten

[01/04/2010]
1. [MOD] Set v4Data.g to 0.0 to avoid the compilation warnings.

Revision 1.2  2010/01/01 18:25:06  leeten

[01/01/2010]
1. [MOD] When update the data, set the importance as zero if no streamline has been drawn.

Revision 1.1  2009/12/31 01:53:59  leeten

[12/30/2009]
1. [1ST] First time checkin.


*/
