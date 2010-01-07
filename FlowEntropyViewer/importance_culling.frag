/*

This is the shader program for direct volume rendering

*/

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	// DEL-BY-LEETEN 01/05/2010-BEGIN
		// uniform sampler1D t1dTf;		// the texture hold the depths of each knots
	// DEL-BY-LEETEN 01/05/2010-END
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	#include "clip_frag_func.frag.h"
	// ADD-BY-LEETEN 01/05/2010-END

	uniform float fThicknessGain;
	uniform float fOcclusionSaturation;

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	#if	0	// MOD-BY-LEETEN 01/05/2010-FROM:
		// range of the data
		uniform float fDataValueMin;
		uniform float fDataValueMax;

		// range of the TF's support 
		uniform float fTfDomainMin;
		uniform float fTfDomainMax;
	#else	// MOD-BY-LEETEN 01/05/2010-TO:
	#include "tf1d_frag_func.frag.h"
	#endif	// MOD-BY-LEETEN 01/05/2010-END

	// ADD-BY-LEETEN 12/31/2009-BEGIN
	varying vec3 v3Tangent_eye;
	uniform int ibIsLightingEnabled;
	// ADD-BY-LEETEN 12/31/2009-END

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	// MOD-BY-LEETEN 01/03/2010-FROM:
		// uniform int ibIsHaloEnabled;
	// TO:
	uniform int ibIsColorMono;
	// MOD-BY-LEETEN 01/03/2010-END

	// DEL-BY-LEETEN 01/03/2010-BEGIN
		// uniform sampler2D t2dLineFlag;
	// DEL-BY-LEETEN 01/03/2010-END
	// ADD-BY-LEETEN 01/01/2010-END

	// ADD-BY-LEETEN 01/03/2010-BEGIN
	uniform int iMaxDistanceToNeighbors_screen;
	// ADD-BY-LEETEN 01/03/2010-END

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	// MOD-BY-LEETEN 01/07/2010-FROM:
		// uniform float fClippingThreshold;
	// TO:
	uniform float fClipVolumeOutsideThreshold;
	uniform float fClipVolumeInsideThreshold;
	// MOD-BY-LEETEN 01/07/2010-END
	// ADD-BY-LEETEN 01/05/2010-END

// ADD-BY-LEETEN 01/02/2010-BEGIN
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
// ADD-BY-LEETEN 01/02/2010-END

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
	// MOD-BY-LEETEN 01/03/2010-FROM:
		// float fPrevV_normalized = v4PrevFragData.g;
	// TO:
	float fPrevV_normalized;
	// MOD-BY-LEETEN 01/05/2010-FROM:
		// fPrevV_normalized = 0.0;
	// TO:
	fPrevV_normalized = 1.0;
	// MOD-BY-LEETEN 01/05/2010-END

	for(int		dy = -iMaxDistanceToNeighbors_screen; dy <= +iMaxDistanceToNeighbors_screen; dy++)
		for(int dx = -iMaxDistanceToNeighbors_screen; dx <= +iMaxDistanceToNeighbors_screen; dx++)
		{
			vec2 v2NeighboringFragCoord;
			v2NeighboringFragCoord.x = gl_FragCoord.x + float(dx);
			v2NeighboringFragCoord.y = gl_FragCoord.y + float(dy);
			v2NeighboringFragCoord.x /= fWindowWidth;
			v2NeighboringFragCoord.y /= fWindowHeight;
			vec4 v4PrevNeighboringFragData = texture2D(t2dPrevLayer, v2NeighboringFragCoord);
			// MOD-BY-LEETEN 01/05/2010-FROM:
				// fPrevV_normalized = max(fPrevV_normalized, v4PrevNeighboringFragData.g);
			// TO:
			fPrevV_normalized = min(fPrevV_normalized, v4PrevNeighboringFragData.g);
			// MOD-BY-LEETEN 01/05/2010-END
		}
	// MOD-BY-LEETEN 01/03/2010-END

	float fThickness_obj;
	fThickness_obj = fThicknessGain;

				// read the value 
	float fV_normalized = texture3D(t3dVolume, gl_TexCoord[0].xyz).x;

				// convert the value into color via the transfer function
	#if	0	// MOD-BY-LEETEN 01/05/2010-FROM:
		vec4 v4Color;
		float fV_data = fDataValueMin + fV_normalized * (fDataValueMax - fDataValueMin);
		float fV_tf = (fV_data - fTfDomainMin) / (fTfDomainMax - fTfDomainMin);
		if( fV_tf < 0.0 || fV_tf > 1.0 )
			v4Color = vec4(0.0);
		else
			v4Color = texture1D(t1dTf, fV_tf);
	#else	// MOD-BY-LEETEN 01/05/2010-TO:
	vec4 v4Color = F4GetColorFrom1DTf(fV_normalized);
	#endif	// MOD-BY-LEETEN 01/05/2010-END

	if( 1.0 / 255.0 < v4Color.a && v4Color.a < 1.0 - 1.0 / 255.0 )
		v4Color.a = 1.0 - pow(1.0 - v4Color.a, fThickness_obj);
	else if( 1.0 / 255.0 > v4Color.a )
		v4Color.a = 0.0;
	else if( 1.0 - 1.0 / 255.0 < v4Color.a )
		v4Color.a = 1.0;

	// ADD-BY-LEETEN 01/03/2010-BEGIN
	if( 0 != ibIsColorMono )
	{
		// MOD-BY-LEETEN 01/05/2010-FROM:
			// v4Color = gl_Color;
		// TO:
		v4Color.rgb = gl_Color.rgb;
		// MOD-BY-LEETEN 01/05/2010-END
	}
	// ADD-BY-LEETEN 01/03/2010-END

	// ADD-BY-LEETEN 12/31/2009-BEGIN
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
	// ADD-BY-LEETEN 12/31/2009-END

	#if	0	// DEL-BY-LEETEN 01/03/2010-BEGIN
		// ADD-BY-LEETEN 01/01/2010-BEGIN
		if( 0 != ibIsColorMono )
		{
			v4Color = gl_Color;
		}
		// ADD-BY-LEETEN 01/01/2010-END
	#endif	// DEL-BY-LEETEN 01/03/2010-END

	/////////////////////////////////////////////////////////////////
	// reduce the opacity if the importance is smaller than the previous one
	if( fV_normalized < fPrevV_normalized )
	{
		// MOD-BY-LEETEN 01/02/2010-FROM:
			// v4Color.a *= fOcclusionSaturation;
		// TO:
		v4Color.a *= FBias(fOcclusionSaturation, 1.0 - (fPrevV_normalized - fV_normalized));
		// MOD-BY-LEETEN 01/02/2010-END
	}

	// ADD-BY-LEETEN 01/05/2010-BEGIN
	bool bPassClipPlanes = BIsInClipvolume(v4FragCoord.z, v4FragCoord.xy);
	if( false == bPassClipPlanes )
	{
		// MOD-BY-LEETEN 01/07/2010-FROM:
			// if( fV_normalized < fClippingThreshold )
		// TO:
		if( fV_normalized < fClipVolumeOutsideThreshold )
		// MOD-BY-LEETEN 01/07/2010-END
		{
			v4Color = vec4(0.0);
		}
	}
	// ADD-BY-LEETEN 01/07/2010-BEGIN
	else
	{
		if( fV_normalized < fClipVolumeInsideThreshold )
			v4Color = vec4(0.0);
	}
	// ADD-BY-LEETEN 01/07/2010-END
	// ADD-BY-LEETEN 01/05/2010-END

	gl_FragData[0] = v4Color;
	gl_FragData[1] = vec4(0.0);
}

/*

$Log: not supported by cvs2svn $
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
