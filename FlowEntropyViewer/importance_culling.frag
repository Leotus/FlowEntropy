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

*/
