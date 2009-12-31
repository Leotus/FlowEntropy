/*

This is the shader program for direct volume rendering

*/

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler1D t1dTf;		// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots

	uniform float fThicknessGain;

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	// range of the data
	uniform float fDataValueMin;
	uniform float fDataValueMax;

	// range of the TF's support 
	uniform float fTfDomainMin;
	uniform float fTfDomainMax;

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
	fThickness_obj *= fThicknessGain;

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

	v4Color.a = 1.0 - pow(1.0 - v4Color.a, fThickness_obj);

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

*/
