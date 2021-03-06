/*

This is the shader program for direct volume rendering

*/

	#include "/clip_frag_func.frag.h"

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots
	uniform sampler2D t2dsDepth;	// the texture hold the depths of each knots

	uniform float fThicknessGain;

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

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
	float fPrevDepth = v4PrevFragData.r;
	float fPrevV_normalized = v4PrevFragData.g;

						// calcualte the distance between this fragment and the previous fragment in the object space 
	vec4 v4PrevCoord_ndc = v4FragCoord;
	v4PrevCoord_ndc.z = fPrevDepth;
	v4PrevCoord_ndc.xyz = v4PrevCoord_ndc.xyz * 2.0 - 1.0;
	v4PrevCoord_ndc.w = 1.0;
	vec4 v4PrevCoord_obj = gl_ModelViewProjectionMatrixInverse * v4PrevCoord_ndc;
	v4PrevCoord_obj = v4PrevCoord_obj / v4PrevCoord_obj.w;
	vec4 v4PrevCoord_eye = gl_ModelViewMatrix * v4PrevCoord_obj;

	vec4 v4Coord_ndc = v4FragCoord;
	v4Coord_ndc.xyz = v4Coord_ndc.xyz * 2.0 - 1.0; 
	v4Coord_ndc.w = 1.0;
	vec4 v4Coord_obj = gl_ModelViewProjectionMatrixInverse * v4Coord_ndc;
	v4Coord_obj = v4Coord_obj / v4Coord_obj.w;
	vec4 v4Coord_eye = gl_ModelViewMatrix * v4Coord_obj;

	float fThickness_obj;
	fThickness_obj = distance(v4Coord_obj, v4PrevCoord_obj);

	float fThicknessRatio = FAdjustThickness(v4FragCoord.z, fPrevDepth, v4FragCoord.xy);
	fThickness_obj *= fThicknessRatio;

	fThickness_obj *= fThicknessGain;

				// read the value 
	float fV_normalized = texture3D(t3dVolume, gl_TexCoord[0].xyz).x;

				// convert the value into color via the transfer function
	vec4 v4Color = F4GetColorFrom1DTf(fV_normalized);

	v4Color.a = 1.0 - exp(- v4Color.a * fThickness_obj);

	float fBackgroundDepth = texture(t2dsDepth, v4FragCoord.xy).r;
	if( v4FragCoord.z > fBackgroundDepth )
		v4Color.a = 0.0;

	/////////////////////////////////////////////////////////////////
	vec4 v4Data = vec4(
		v4FragCoord.z, 
		fV_normalized,
		0.0, 
		1.0);

	gl_FragData[0] = v4Color;
	gl_FragData[1] = v4Data;
}

/*

$Log: not supported by cvs2svn $
Revision 1.4  2010/05/14 15:42:48  leeten

[05/14/2010]
1. [MOD] Change the ray integral equation from A = 1 - (1 - A)^D to A = 1 - exp(-AD).

Revision 1.3  2010/01/12 23:47:10  leeten

[01/12/2010]
1. [ADD] Add a new texture unit t2dsDepth, which define the background depth. The fragment will be cull out if its depth is larger than the background depth.

Revision 1.2  2010/01/06 17:15:24  leeten

[01/06/2010]
1. [ADD] Adjust the slab thickness according to the clipping volume.
2. [ADD] Include the file clip_frag_func.frag.h.
3. [MOD] nclude the file tf1d_frag_func.frag.h and lookup the transfer function via a function F4GetColorFrom1DTf().

Revision 1.1  2009/12/31 01:53:59  leeten

[12/30/2009]
1. [1ST] First time checkin.


*/
