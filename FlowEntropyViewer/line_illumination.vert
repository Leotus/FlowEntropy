#version	130	
#extension	GL_EXT_gpu_shader4 : enable

	out		vec3	pv3Tangents_eye;

void 
main()
{	
	pv3Tangents_eye = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	gl_TexCoord[0] = vec4(0.5 * (gl_Vertex.xyz + vec3(1.0)), 1.0);

	gl_TexCoord[1] = gl_MultiTexCoord1;

	gl_FrontColor = gl_Color;
}

/*

$Log: not supported by cvs2svn $
Revision 1.3  2010/01/12 23:40:43  leeten

[01/09/2010]
1. [ADD] Use GLSL 1.50 and gpu shader 4.0.
2. [MOD] Change the varying variable v3Tangent_eye to an out variable pv3Tangents_eye.

Revision 1.2  2010/01/11 19:19:46  leeten

[01/10/2010]
1. [ADD] Bypass the textrue coordinate gl_MultiTexCoord1 to gl_TexCoord[1].

Revision 1.1  2010/01/01 18:14:12  leeten

[01/01/2010]
1. [1ST] First time checkin.


*/
