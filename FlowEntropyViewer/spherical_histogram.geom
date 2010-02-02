#version	130 
#extension	GL_EXT_geometry_shader4 : enable

uniform float	fOpacity;
uniform int		ibDrawTop;
uniform float	fMaxCount;
uniform float	fMaxRadius;

uniform float	fRelativeThreshold;	// ADD-BY-LEETEN 02/01/2010
in		float	pfBin[];


float fBin2Radius(float fBin)
{
	// MOD-BY-LEETEN 02/01/2010-FROM:
		// return 2.0 + fBin * (fMaxRadius - 2.0) / fMaxCount;
	// TO:
	return 1.0 + fBin * (fMaxRadius - 1.0) / fMaxCount;
	// MOD-BY-LEETEN 02/01/2010-END
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
		// MOD-BY-LEETEN 02/01/2010-FROM:
			// if( pfBin[0] > fMaxCount * 0.05 )
		// TO:
		if( pfBin[0] > fMaxCount * fRelativeThreshold )
		// MOD-BY-LEETEN 02/01/2010-END
		{
			// MOD-BY-LEETEN 02/01/2010-FROM:
				// gl_FrontColor = vec4(vec3(pfBin[0], pfBin[1], pfBin[2])/fMaxCount, fOpacity);
			// TO:
			gl_FrontColor = vec4(0.0, 0.0, pfBin[0]/fMaxCount, fOpacity);
			// MOD-BY-LEETEN 02/01/2010-END

			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			EndPrimitive();																						

			#if	0	// DEL-BY-LEETEN 02/01/2010-BEGIN
				gl_Position = gl_ModelViewProjectionMatrix * vec4(vec3(0.0),		1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(pfBin[0]), 1.0);	EmitVertex();
				EndPrimitive();																						

				gl_Position = gl_ModelViewProjectionMatrix * vec4(vec3(0.0),		1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]), 1.0);	EmitVertex();
				EndPrimitive();																						
			#endif	// DEL-BY-LEETEN 02/01/2010-END

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

			#if	0	// MOD-BY-LEETEN 02/01/2010-FROM:
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0),		1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]),	1.0);	EmitVertex();
			#else	// MOD-BY-LEETEN 02/01/2010-TO:
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(pfBin[1]),	1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0),		1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(pfBin[2]), 1.0);	EmitVertex();
			#endif	// MOD-BY-LEETEN 02/01/2010-END
			EndPrimitive();																						

			#if	0	// MOD-BY-LEETEN 02/01/2010-FROM:
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(0), 1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0), 1.0);	EmitVertex();
				gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			#else	// MOD-BY-LEETEN 02/01/2010-TO:
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[2].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[1].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_PositionIn[0].xyz * fBin2Radius(0), 1.0);	EmitVertex();
			#endif	// MOD-BY-LEETEN 02/01/2010-END
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
