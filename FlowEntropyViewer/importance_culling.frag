/*

This is the shader program for direct volume rendering

*/

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler1D t1dTf;		// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots

	uniform float fThicknessGain;
	uniform float fOcclusionSaturation;

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	// range of the data
	uniform float fDataValueMin;
	uniform float fDataValueMax;

	// range of the TF's support 
	uniform float fTfDomainMin;
	uniform float fTfDomainMax;

	// ADD-BY-LEETEN 12/31/2009-BEGIN
	varying vec3 v3Tangent_eye;
	uniform int ibIsLightingEnabled;
	// ADD-BY-LEETEN 12/31/2009-END

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	uniform int ibIsHaloEnabled;

	uniform sampler2D t2dLineFlag;
	// ADD-BY-LEETEN 01/01/2010-END

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
	float fPrevV_normalized = v4PrevFragData.g;

	float fThickness_obj;
	fThickness_obj = fThicknessGain;

				// read the value 
	float fV_normalized = texture3D(t3dVolume, gl_TexCoord[0].xyz).x;

				// convert the value into color via the transfer function
	vec4 v4Color;
	float fV_data = fDataValueMin + fV_normalized * (fDataValueMax - fDataValueMin);
	float fV_tf = (fV_data - fTfDomainMin) / (fTfDomainMax - fTfDomainMin);
	if( fV_tf < 0.0 || fV_tf > 1.0 )
		v4Color = vec4(0.0);
	else
		v4Color = texture1D(t1dTf, fV_tf);

	if( 1.0 / 255.0 < v4Color.a && v4Color.a < 1.0 - 1.0 / 255.0 )
		v4Color.a = 1.0 - pow(1.0 - v4Color.a, fThickness_obj);
	else if( 1.0 / 255.0 > v4Color.a )
		v4Color.a = 0.0;
	else if( 1.0 - 1.0 / 255.0 < v4Color.a )
		v4Color.a = 1.0;

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

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	if( 0 != ibIsHaloEnabled )
	{
		v4Color.rgb = gl_Color.rgb;
	}
	// ADD-BY-LEETEN 01/01/2010-END

	/////////////////////////////////////////////////////////////////
	// reduce the opacity if the importance is smaller than the previous one
	if( fV_normalized < fPrevV_normalized )
	{
		v4Color.a *= fOcclusionSaturation;
	}

	gl_FragData[0] = v4Color;
	gl_FragData[1] = vec4(0.0);
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/12/31 01:53:59  leeten

[12/30/2009]
1. [1ST] First time checkin.


*/
