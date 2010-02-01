#version	130	
#extension	GL_EXT_gpu_shader4 : enable

uniform float	fOpacity;
uniform int		ibUniformColor;
in		float fBin;
out		float pfBin;

void 
main()
{	
	gl_Position = gl_Vertex;

	pfBin = fBin;

	vec4 v4Color;
	if( 0 != ibUniformColor )
		v4Color = vec4(1.0, 1.0, 1.0, 1.0);
	else
		v4Color = vec4(fBin, fBin, fBin, fOpacity);
	gl_FrontColor = v4Color;
}

/*

$Log: not supported by cvs2svn $

*/
