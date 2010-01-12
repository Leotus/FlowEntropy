// ADD-BY-LEETEN 01/12/2010-BEGIN
#version	150 compatibility
#extension	GL_EXT_gpu_shader4 : enable
// ADD-BY-LEETEN 01/12/2010-END

	// MOD-BY-LEETEN 01/12/2010-FROM:
		// varying vec3 v3Tangent_eye;
	// TO:
	out		vec3	pv3Tangents_eye;
	// MOD-BY-LEETEN 01/12/2010-END

void 
main()
{	
	// MOD-BY-LEETEN 01/12/2010-FROM:
		// v3Tangent_eye = normalize(gl_NormalMatrix * gl_Normal);
	// TO:
	pv3Tangents_eye = normalize(gl_NormalMatrix * gl_Normal);
	// MOD-BY-LEETEN 01/12/2010-END
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	gl_TexCoord[0] = vec4(0.5 * (gl_Vertex.xyz + vec3(1.0)), 1.0);

	// ADD-BY-LEETEN 01/10/2010-BEGIN
						// use the texture int 1 to pass the vertex index in its streamline
	gl_TexCoord[1] = gl_MultiTexCoord1;
	// ADD-BY-LEETEN 01/10/2010-END

	// ADD-BY-LEETEN 01/01/2010-BEGIN
	gl_FrontColor = gl_Color;
	// ADD-BY-LEETEN 01/01/2010-END
}

/*

$Log: not supported by cvs2svn $
Revision 1.2  2010/01/11 19:19:46  leeten

[01/10/2010]
1. [ADD] Bypass the textrue coordinate gl_MultiTexCoord1 to gl_TexCoord[1].

Revision 1.1  2010/01/01 18:14:12  leeten

[01/01/2010]
1. [1ST] First time checkin.


*/
