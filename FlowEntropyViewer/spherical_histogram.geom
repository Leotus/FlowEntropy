#version	130 
#extension	GL_EXT_geometry_shader4 : enable

uniform float	fOpacity;
uniform int		ibDrawTop;
uniform float	fMaxCount;
uniform float	fMaxRadius;

uniform float	fRelativeThreshold;	
in		float	pfBin[];


float fBin2Radius(float fBin)
{
	return 1.0 + fBin * (fMaxRadius - 1.0) / fMaxCount;
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
		if( pfBin[0] > fMaxCount * fRelativeThreshold )
		{
			gl_FrontColor = vec4(0.0, 0.0, pfBin[0]/fMaxCount, fOpacity);

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			EndPrimitive();																						

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

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]),	1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			EndPrimitive();																						

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			EndPrimitive();																						
		}
	}

}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/02/01 06:20:11  leeten

[01/31/2010]
1. [1ST] First time checkin.


*/
