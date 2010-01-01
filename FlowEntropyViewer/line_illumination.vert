varying vec3 v3Tangent_eye;

void 
main()
{	
	v3Tangent_eye = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	gl_TexCoord[0] = vec4(0.5 * (gl_Vertex.xyz + vec3(1.0)), 1.0);
	// ADD-BY-LEETEN 01/01/2010-BEGIN
	gl_FrontColor = gl_Color;
	// ADD-BY-LEETEN 01/01/2010-END
}

/*

$Log: not supported by cvs2svn $

*/
