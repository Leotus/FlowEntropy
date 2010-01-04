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

	gl_FragData[0] = vec4(0.0);
	gl_FragData[1] = v4Data;
}

/*

$Log: not supported by cvs2svn $
Revision 1.2  2010/01/01 18:25:06  leeten

[01/01/2010]
1. [MOD] When update the data, set the importance as zero if no streamline has been drawn.

Revision 1.1  2009/12/31 01:53:59  leeten

[12/30/2009]
1. [1ST] First time checkin.


*/
