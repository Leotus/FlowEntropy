/*

This is the shader program for direct volume rendering

*/

#extension GL_EXT_gpu_shader4 : enable

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots

	uniform sampler2D t2dsDepth;	// the texture hold the depths of each knots

	#include "/clip_frag_func.frag.h"

	uniform float fThicknessGain;
	uniform float fOcclusionSaturation;

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	#include "/tf1d_frag_func.frag.h"

	in		vec3 v3Tangent_eye;
	uniform int ibIsLightingEnabled;

	uniform int ibIsColorMono;

	uniform int iMaxDistanceToNeighbors_screen;

	uniform float fClipVolumeOutsideThreshold;
	uniform float fClipVolumeInsideThreshold;

	uniform float	fDashPeriod;
	uniform float	fDashOffset;
	uniform float	fDashThreshold;
	uniform int		ibIsEntropyDependentDashed;

float FBias(float b, float t)
{
	if( 0.0 == b )
		return 0.0;

	if( 0.0 == t )
		return 0.0;

	if( 1.0 == t )
		return 1.0;

	return pow(t, log(b) / log(0.5));
}

void 
main()
{
	vec4 v4FragCoord = gl_FragCoord;
	v4FragCoord.x /= fWindowWidth;
	v4FragCoord.y /= fWindowHeight;
	v4FragCoord.w = 1.0 / v4FragCoord.w;

				// read the previous layer
	vec4 v4PrevFragData = texture2D(t2dPrevLayer, v4FragCoord.xy);
	float fPrevDepth		= v4PrevFragData.r;
	float fPrevV_normalized;
	fPrevV_normalized = 1.0;

	for(int		dy = -iMaxDistanceToNeighbors_screen; dy <= +iMaxDistanceToNeighbors_screen; dy++)
		for(int dx = -iMaxDistanceToNeighbors_screen; dx <= +iMaxDistanceToNeighbors_screen; dx++)
		{
			vec2 v2NeighboringFragCoord;
			v2NeighboringFragCoord.x = gl_FragCoord.x + float(dx);
			v2NeighboringFragCoord.y = gl_FragCoord.y + float(dy);
			v2NeighboringFragCoord.x /= fWindowWidth;
			v2NeighboringFragCoord.y /= fWindowHeight;
			vec4 v4PrevNeighboringFragData = texture2D(t2dPrevLayer, v2NeighboringFragCoord);
			fPrevV_normalized = min(fPrevV_normalized, v4PrevNeighboringFragData.g);
		}

	float fThickness_obj;
	fThickness_obj = fThicknessGain;

				// read the value 
	float fV_normalized = texture3D(t3dVolume, gl_TexCoord[0].xyz).x;

				// convert the value into color via the transfer function
	vec4 v4Color = F4GetColorFrom1DTf(fV_normalized);

	if( 1.0 / 255.0 < v4Color.a && v4Color.a < 1.0 - 1.0 / 255.0 )
		v4Color.a = 1.0 - pow(1.0 - v4Color.a, fThickness_obj);
	else if( 1.0 / 255.0 > v4Color.a )
		v4Color.a = 0.0;
	else if( 1.0 - 1.0 / 255.0 < v4Color.a )
		v4Color.a = 1.0;

	if( 0 != ibIsColorMono )
	{
		v4Color.rgb = gl_Color.rgb;
	}

						// apply lighting
	if( 0 != ibIsLightingEnabled )
	{
		vec4 v4Coord_ndc = v4FragCoord;
		v4Coord_ndc.xyz = v4Coord_ndc.xyz * 2.0 - 1.0; 
		v4Coord_ndc.w = 1.0;
		vec4 v4Coord_eye = gl_ProjectionMatrixInverse * v4Coord_ndc;
		v4Coord_eye = v4Coord_eye / v4Coord_eye.w;

		vec4 v4LightPos_eye =	vec4(0.0, 0.0, 0.0, 1.0);
		vec4 v4LightDir_eye =	v4LightPos_eye - v4Coord_eye;
		vec3 v3LightDir_eye =	normalize(v4LightDir_eye.xyz);
		vec4 v4ViewDir_eye =	-v4Coord_eye;;
		vec3 v3ViewDir_eye =	normalize(v4ViewDir_eye.xyz);

		float LT = clamp(dot(v3LightDir_eye,	v3Tangent_eye), -1.0, 1.0);
		float VT = clamp(dot(v3ViewDir_eye,	v3Tangent_eye),		-1.0, 1.0);

		float LN = clamp(sqrt( 1.0 -  LT*LT ), 0.0, 1.0);
		float VR = clamp(sqrt( (1.0 - LT*LT) *  (1.0 - VT*VT) ) - LT*VT, 0.0, 1.0);

		if( gl_FrontMaterial.shininess > 1.0 / 255.0 )
			VR = pow(VR, gl_FrontMaterial.shininess);
		else
			VR = 1.0;
		
		v4Color.r = gl_FrontMaterial.ambient.r * v4Color.r + gl_FrontMaterial.diffuse.r * v4Color.r * LN + gl_FrontMaterial.specular.r * v4Color.r * VR;
		v4Color.g = gl_FrontMaterial.ambient.g * v4Color.g + gl_FrontMaterial.diffuse.g * v4Color.g * LN + gl_FrontMaterial.specular.g * v4Color.g * VR;
		v4Color.b = gl_FrontMaterial.ambient.b * v4Color.b + gl_FrontMaterial.diffuse.b * v4Color.b * LN + gl_FrontMaterial.specular.b * v4Color.b * VR;
	}

	/////////////////////////////////////////////////////////////////
	// reduce the opacity if the importance is smaller than the previous one
	if( fV_normalized < fPrevV_normalized )
	{
		v4Color.a *= FBias(fOcclusionSaturation, 1.0 - (fPrevV_normalized - fV_normalized));
	}

	bool bPassClipPlanes = BIsInClipvolume(v4FragCoord.z, v4FragCoord.xy);
	if( false == bPassClipPlanes )
	{
		if( fV_normalized < fClipVolumeOutsideThreshold )
		{
			v4Color = vec4(0.0);
		}
	}
	else
	{
		if( fV_normalized < fClipVolumeInsideThreshold )
			v4Color = vec4(0.0);
	}

	if( fDashPeriod > 0.0 )
	{
		float fIndexInStreamline = gl_TexCoord[1].r;
		float fFrequency = 2.0 * 3.14159 / fDashPeriod;
		if( 0 != ibIsEntropyDependentDashed )
		fFrequency /= (0.01 + fV_normalized);

		float fPeriod = sin( fDashOffset + fIndexInStreamline * fFrequency );
		fPeriod = (fPeriod + 1.0) / 2.0;
		if( (0 == ibIsEntropyDependentDashed && fPeriod  > fDashThreshold) ||
			(0 != ibIsEntropyDependentDashed && fPeriod  > fV_normalized) )
			v4Color.a = 0.0;
	}

	float fDepth = texture2D(t2dsDepth, v4FragCoord.xy).r;
	if( v4FragCoord.z > fDepth )
		v4Color.a = 0.0;

	gl_FragData[0] = v4Color;
	gl_FragData[1] = vec4(0.0);
}

/*

$Log: not supported by cvs2svn $
Revision 1.7  2010/01/12 23:50:32  leeten

[01/12/2010]
1. [ADD] Add a new texture unit t2dsDepth, which define the background depth. The fragment will be cull out if its depth is larger than the background depth.
2. [DEL] Remove the variable ibIsHigherEntropyWithLongerLine.
3. [ADD] Add a new variable ibIsEntropyDependentDashed. If this variable is not zero, the space btw the dashed lines become the same regardless of the local entropy.
4. [MOD] Change the declaration fo the variable v3Tangent_eye from 'varying' to 'in.'

Revision 1.6  2010/01/11 19:19:03  leeten

[01/10/2010]
1. [DEL] Remove deleted codes.
2. [ADD] Add variables fDashPeriod and fDashOffset to support the rendering of dashed lines.

Revision 1.5  2010/01/07 15:03:22  leeten

[01/07/2010]
1. [MOD] Change the variable names such that the front/back sides of the clipping volume become the outside/inside of the clipping volume.
2. [ADD] Add thresholds for both sides of the clipping volume.

Revision 1.4  2010/01/06 17:13:05  leeten

[01/06/2010]
1. [MOD] Dlelete the variables t1dTf. Instead incldue the header tf1d_frag_func.frag.h.
2. [ADD] Incldue the header clip_frag_func.frag.h.
3. [ADD] Add variable fClippingThreshold to specify the entropy threshold for fragments outside the clipping volume.
4. [MOD] Change the computation of fPrevV_normalized s.t. it becomes the min. of the neighboring pixels.
5. [MOD] Lookup the transfer function via a function F4GetColorFrom1DTf().
6. [MOD] Change the color when do color lookup with halo effect: only the RGB channels will be changed.
7. [ADD] Compare fragment against the clipping volume. If the fragment is outside the volume, the entropy will be compared against the threshold fClippingThreshold to decide whether the fragment is et to complete transparent.

Revision 1.3  2010/01/04 18:39:28  leeten

[01/04/2010]
1. [ADD] Add new unifrom variables: ibIsLightingEnabled, ibIsColorMono, ibIsHaloEnabled to control the shading scheme.
2. [ADD] Add a new uniform variable iMaxDistanceToNeighbors_screen to decide the region to decide the importance. The importance is defined as the max. entropy in that region surrounding the fragment.
3. [ADD] Define a function FBias to control the drop rate of the opacities for importance culling.

Revision 1.2  2010/01/01 18:27:07  leeten

[01/01/2010]
1. [ADD] Support halo effect.
2. [ADD] Add lighting.

Revision 1.1  2009/12/31 01:53:59  leeten

[12/30/2009]
1. [1ST] First time checkin.


*/
