#version	150	compatibility
#extension	GL_EXT_geometry_shader4 : enable

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
	if( 0 == ibIsGlyphEnabled )
	{
		for(int i=0; i< gl_VerticesIn; i++)
		{
			gl_Position = gl_in[i].gl_Position;
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
		int iVertexIndexInStreamline = int(gl_TexCoordIn[0][1]);
		if( 0 != iVertexIndexInStreamline % iGlyphStep )
			return;

		vec4 v4LineDir_ndc = gl_in[1].gl_Position - gl_in[0].gl_Position;
		vec4 v4NormalDir_ndc = v4LineDir_ndc;
		v4NormalDir_ndc.x = -v4LineDir_ndc.y;
		v4NormalDir_ndc.y =  v4LineDir_ndc.x;
		v4NormalDir_ndc = normalize(v4NormalDir_ndc);

		gl_Position = gl_in[0].gl_Position;
		gl_TexCoord[0] = gl_TexCoordIn[0][0];
		gl_TexCoord[1] = gl_TexCoordIn[0][1];
		gl_FrontColor = gl_FrontColorIn[0];
		gl_ClipVertex = gl_ClipVertexIn[0];
		v3Tangent_eye = pv3Tangents_eye[0];
		EmitVertex();

		vec4 v4NewPosition1 = gl_in[0].gl_Position + v4LineDir_ndc * fGlyphLength;
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

			vec4 v4Offset_ndc = 0.5 * (gl_in[0].gl_Position + v4NewPosition1);

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

*/
