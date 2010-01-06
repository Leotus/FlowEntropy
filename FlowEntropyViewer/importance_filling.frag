/*

This is the shader program to fill the importance

*/

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots
	// ADD-BY-LEETEN 01/01/2010-BEGIN
	uniform sampler2D t2dLineFlag;
	// ADD-BY-LEETEN 01/01/2010-END

	uniform float fWindowWidth;
	uniform float fWindowHeight;

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	#include "clip_frag_func.frag.h"

	uniform vec4	v4ClippingPlaneFrontColor;
	uniform int		ibIsClippingPlaneFrontColorMono;
	uniform vec4	v4ClippingPlaneBackColor;
	uniform int		ibIsClippingPlaneBackColorMono;
	// ADD-BY-LEETEN 01/05/2010-END

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	#include "tf1d_frag_func.frag.h"
	// ADD-BY-LEETEN 01/05/2010-END

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
	#if	0	// MOD-BY-LEETEN 01/01/2010-FROM:
		vec4 v4Data = vec4(
			v4FragCoord.z, 
			max(fPrevV_normalized, fV_normalized),
			0.0, 
			1.0);
	#else	// MOD-BY-LEETEN 01/01/2010-TO:
	vec4 v4Data = vec4(
		v4FragCoord.z, 
		0.0,
		0.0, 
		1.0);
	vec4 v4PrevColor = texture2D(t2dLineFlag, v4FragCoord.xy);
	if( v4PrevColor.a > 0.0 )
		v4Data.g = max(fPrevV_normalized, fV_normalized);
	else
		// MOD-BY-LEETEN 01/02/2010-FROM:
			// v4Data.g = 0;	
		// TO:
		v4Data.g = 0.0;	
		// MOD-BY-LEETEN 01/02/2010-END
	#endif	// MOD-BY-LEETEN 01/01/2010-END

	// MOD-BY-LEETEN 01/05/2010-FROM:
		// gl_FragData[0] = vec4(0.0);
	// TO:
	float fDepth = v4FragCoord.z;

	vec4 v4Color = vec4(0.0);

	/*
	float fThicknessRatio = FAdjustThickness(v4FragCoord.z, v4PrevFragData.r, v4FragCoord.xy);
	if( 0.0 < fThicknessRatio && fThicknessRatio < 1.0 )
	{
		if( true == BIsInClipvolume(v4FragCoord.z, v4FragCoord.xy) && false == BIsInClipvolume(v4PrevFragData.r, v4FragCoord.xy))
		{
			fDepth = FGetZFar(v4FragCoord.xy);
			if( 0 != ibIsClippingPlaneBackColorMono )
				v4Color = v4ClippingPlaneBackColor;
			else
				v4Color = F4GetColorFrom1DTf(fV_normalized);
			v4Color.a = v4ClippingPlaneBackColor.a;
		}
		else
		{
			fDepth = FGetZNear(v4FragCoord.xy);
			if( 0 != ibIsClippingPlaneFrontColorMono )
				v4Color = v4ClippingPlaneFrontColor;
			else
				v4Color = F4GetColorFrom1DTf(fV_normalized);
			v4Color.a = v4ClippingPlaneFrontColor.a;
		}
	}
	*/
	float fThicknessRatio = FAdjustThickness(v4FragCoord.z, v4PrevFragData.r, v4FragCoord.xy);
	if( 0.0 < fThicknessRatio && fThicknessRatio < 1.0 )
	{
		float fZNear	= FGetZNear(v4FragCoord.xy);
		float fZFar		= FGetZFar(v4FragCoord.xy);
		if( v4FragCoord.z < fZFar && fZFar < v4PrevFragData.r )
		{
			fDepth = fZFar;
			if( 0 != ibIsClippingPlaneBackColorMono )
				v4Color = v4ClippingPlaneBackColor;
			else
				v4Color = F4GetColorFrom1DTf(fV_normalized);
			v4Color.a = v4ClippingPlaneBackColor.a;
		}

		if( v4FragCoord.z < fZNear && fZNear < v4PrevFragData.r )
		{
			fDepth = fZNear;
			if( 0 != ibIsClippingPlaneFrontColorMono )
				v4Color = v4ClippingPlaneFrontColor;
			else
				v4Color = F4GetColorFrom1DTf(fV_normalized);
			v4Color.a = v4ClippingPlaneFrontColor.a;
		}
	}

	gl_FragDepth = fDepth;
	gl_FragData[0] = v4Color;
	// MOD-BY-LEETEN 01/05/2010-END

	gl_FragData[1] = v4Data;
}

/*

$Log: not supported by cvs2svn $
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
