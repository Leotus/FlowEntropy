/*

This is the shader program to fill the importance

*/

	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots

	uniform float fWindowWidth;
	uniform float fWindowHeight;
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
		max(fPrevV_normalized, fV_normalized),
		0.0, 
		1.0);

	gl_FragData[0] = vec4(0.0);
	gl_FragData[1] = v4Data;
}

/*

$Log: not supported by cvs2svn $

*/
