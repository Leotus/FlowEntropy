#version	130 
#extension	GL_EXT_geometry_shader4 : enable

uniform float	fOpacity;
uniform int		ibDrawTop;
uniform float	fMaxCount;
uniform float	fMaxRadius;
in		float	pfBin[];

float fBin2Radius(float fBin)
{
	return 2.0 + fBin * (fMaxRadius - 2.0) / fMaxCount;
}

void 
main()
{	
	vec4 v4Color;
	if( 0 != ibDrawTop )
	{
		gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
		gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
		gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
		gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
		EndPrimitive();																						
	}
	else
	{
		if( pfBin[0] > fMaxCount * 0.05 )
		{
			gl_FrontColor = vec4(vec3(pfBin[0], pfBin[1], pfBin[2])/fMaxCount, fOpacity);

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(vec3(0.0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(vec3(0.0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			EndPrimitive();																						


/*
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]),	1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]),	1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			EndPrimitive();																						
*/
		}
	}

}

/*

$Log: not supported by cvs2svn $

*/
