/*

This is the shader program to apply diffusion

*/

uniform float fWidth;
uniform float fHeight;
uniform float fDepth;
uniform float fZ;
uniform sampler3D	t3dWeightOffset;
uniform sampler3D	t3dSrc;
uniform vec4		f4MaxWeightOffset;
uniform vec4		f4MinWeightOffset;
uniform float		fAttenuation;
uniform int			iNrOfTargets;

void 
main()
{
	float fX_normalized = gl_FragCoord.x / fWidth;
	float fY_normalized = gl_FragCoord.y / fHeight;
	float fZ_normalized = fZ / fDepth;
	vec3 v3TexCoord = vec3(fX_normalized, fY_normalized, fZ_normalized);

	vec4 f4NX, f4PX, f4NY, f4PY, f4NZ, f4PZ;
	vec4 f4Vec;
	for(int f = 0; f < iNrOfTargets; f++)
	{
		if( 0 == f )
		{
			f4Vec = texture3D(t3dSrc, v3TexCoord);
			f4Vec = 2.0 * f4Vec - 1.0;

			f4PZ = texture3D(t3dSrc, vec3(fX_normalized, fY_normalized, (fZ - 1.0) / fDepth));
			f4PZ = 2.0 * f4PZ - 1.0;
		}
		else
		{
			f4PZ = f4Vec;
			f4Vec = f4NZ;
		}
		f4PX = texture3D(t3dSrc, vec3((gl_FragCoord.x - 1.0)/ fWidth, fY_normalized, fZ_normalized));
		f4NX = texture3D(t3dSrc, vec3((gl_FragCoord.x + 1.0)/ fWidth, fY_normalized, fZ_normalized));
		f4PY = texture3D(t3dSrc, vec3(fX_normalized, (gl_FragCoord.y +-1.0) / fHeight, fZ_normalized));
		f4NY = texture3D(t3dSrc, vec3(fX_normalized, (gl_FragCoord.y -+1.0) / fHeight, fZ_normalized));
		f4NZ = texture3D(t3dSrc, vec3(fX_normalized, fY_normalized, (fZ + 1.0) / fDepth));

		f4PX =	2.0 * f4PX - 1.0;
		f4NX =	2.0 * f4NX - 1.0;
		f4PY =	2.0 * f4PY - 1.0;
		f4NY =	2.0 * f4NY - 1.0;
		f4NZ =  2.0 * f4NZ - 1.0;

		vec4 f4WeightOffset = texture3D(t3dWeightOffset, v3TexCoord);
		f4WeightOffset.x = f4WeightOffset.x * (f4MaxWeightOffset.x - f4MinWeightOffset.x) + f4MinWeightOffset.x;
		f4WeightOffset.y = f4WeightOffset.y * (f4MaxWeightOffset.y - f4MinWeightOffset.y) + f4MinWeightOffset.y;
		f4WeightOffset.z = f4WeightOffset.z * (f4MaxWeightOffset.z - f4MinWeightOffset.z) + f4MinWeightOffset.z;
		f4WeightOffset.w = f4WeightOffset.w * (f4MaxWeightOffset.w - f4MinWeightOffset.w) + f4MinWeightOffset.w;
		vec3 v3NewVec = vec3(
			(1.0 - f4WeightOffset.w) * f4Vec.x + (f4PX.x + f4NX.x + f4PY.x + f4NY.x + f4PZ.x + f4NZ.x - 6.0 * f4Vec.x) * fAttenuation + f4WeightOffset.x,
			(1.0 - f4WeightOffset.w) * f4Vec.y + (f4PX.y + f4NX.y + f4PY.y + f4NY.y + f4PZ.y + f4NZ.y - 6.0 * f4Vec.y) * fAttenuation + f4WeightOffset.y,
			(1.0 - f4WeightOffset.w) * f4Vec.z + (f4PX.z + f4NX.z + f4PY.z + f4NY.z + f4PZ.z + f4NZ.z - 6.0 * f4Vec.z) * fAttenuation + f4WeightOffset.z
		);

		gl_FragData[f] = vec4(0.5 * (v3NewVec + 1.0), 1.0);
	}
}

/*

$Log: not supported by cvs2svn $

*/
