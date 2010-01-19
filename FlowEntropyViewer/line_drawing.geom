// MOD-BY-LEETEN 01/19/2010-FROM:
	// #version	150 compatibility
// TO:
#version	130 
// MOD-BY-LEETEN 01/19/2010-END
#extension	GL_EXT_geometry_shader4 : enable

// ADD-BY-LEETEN 01/19/2010-BEGIN
uniform sampler3D	t3dVolume;	// the texture hold the depths of each knots
uniform float		fDashThreshold;
// ADD-BY-LEETEN 01/19/2010-END

uniform float	fWindowWidth;
uniform float	fWindowHeight;
uniform int		ibIsGlyphEnabled;
uniform int		iGlyphStep;
uniform float	fGlyphLength;
uniform float	fGlyphWidth;
		in		vec3	pv3Tangents_eye[];
smooth	out		vec3	v3Tangent_eye;

void 
main()
{	
	// ADD-BY-LEETEN 01/19/2010-BEGIN
	float fEntropy0_normalized = texture3D(t3dVolume, gl_TexCoordIn[0][0].xyz).r;
	float fEntropy1_normalized = texture3D(t3dVolume, gl_TexCoordIn[1][0].xyz).r;
	float fEntropy_normalized = 0.5 * (fEntropy0_normalized + fEntropy1_normalized);
	bool bIsEntropyHigherThanThreshold = ( fEntropy_normalized > fDashThreshold )?true:false;
	// ADD-BY-LEETEN 01/19/2010-END


	// MOD-BY-LEETEN 01/19/2010-FROM:
		// if( 0 == ibIsGlyphEnabled )
	// TO:
	if( 0 == ibIsGlyphEnabled || true == bIsEntropyHigherThanThreshold )
	// MOD-BY-LEETEN 01/19/2010-END
	{
		for(int i=0; i< gl_VerticesIn; i++)
		{
			gl_Position = gl_PositionIn[i];
			gl_TexCoord[0] = gl_TexCoordIn[i][0];
			gl_TexCoord[1] = gl_TexCoordIn[i][1];
			gl_FrontColor = gl_FrontColorIn[i];
			gl_ClipVertex = gl_ClipVertexIn[i];
			v3Tangent_eye = pv3Tangents_eye[i];
			EmitVertex();
		}

		EndPrimitive();																						
	}
	else
	{

		#if	0	// MOD-BY-LEETEN 01/19/2010-FROM:
			int iVertexIndexInStreamline = int(gl_TexCoordIn[0][1]);
			if( 0 != iVertexIndexInStreamline % iGlyphStep )
		#else	// MOD-BY-LEETEN 01/19/2010-TO:
		int iVertexIndexInStreamline = int(gl_TexCoordIn[0][1].x);
		int iStreamline = int(gl_TexCoordIn[0][1].y);
		if( 0 != (iVertexIndexInStreamline + iStreamline ) % iGlyphStep )
		#endif	// MOD-BY-LEETEN 01/19/2010-END
			return;

		vec4 v4LineDir_ndc = gl_PositionIn[1] - gl_PositionIn[0];
		vec4 v4NormalDir_ndc = v4LineDir_ndc;
		v4NormalDir_ndc.x = -v4LineDir_ndc.y;
		v4NormalDir_ndc.y =  v4LineDir_ndc.x;
		v4NormalDir_ndc = normalize(v4NormalDir_ndc);

		gl_Position = gl_PositionIn[0];
		gl_TexCoord[0] = gl_TexCoordIn[0][0];
		gl_TexCoord[1] = gl_TexCoordIn[0][1];
		gl_FrontColor = gl_FrontColorIn[0];
		gl_ClipVertex = gl_ClipVertexIn[0];
		v3Tangent_eye = pv3Tangents_eye[0];
		EmitVertex();

		vec4 v4NewPosition1 = gl_PositionIn[0] + v4LineDir_ndc * fGlyphLength;
		gl_Position = v4NewPosition1;
		gl_TexCoord[0] = gl_TexCoordIn[1][0];
		gl_TexCoord[1] = gl_TexCoordIn[1][1];
		gl_FrontColor = gl_FrontColorIn[1];
		gl_ClipVertex = gl_ClipVertexIn[1];
		v3Tangent_eye = pv3Tangents_eye[1];
		EmitVertex();

		EndPrimitive();																						

		for(int i = 0; i < 2; i++)
		{
			gl_Position = v4NewPosition1;
			EmitVertex();

			vec4 v4Offset_ndc = 0.5 * (gl_PositionIn[0] + v4NewPosition1);

			float fDir = ( 0 == i )?-1.0:+1.0;
			v4Offset_ndc.x += fDir * v4NormalDir_ndc.x * fGlyphWidth * 2.0 / fWindowWidth;
			v4Offset_ndc.y += fDir * v4NormalDir_ndc.y * fGlyphWidth * 2.0 / fWindowHeight;
			gl_Position =		v4Offset_ndc;
			EmitVertex();


			EndPrimitive();																						
		}
	}
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/01/12 23:37:44  leeten

[01/12/2010]
1. [ADD] First time checkin.


*/
