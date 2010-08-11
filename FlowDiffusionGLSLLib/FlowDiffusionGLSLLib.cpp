#define NR_OF_GLSL_TARGETS	8

#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <math.h>

#if	0	// DEL-BY-LEETEN 08/09/2010-BEGIN
	#pragma comment (lib, "libfbo.lib")   
	#pragma comment (lib, "libshader.lib")
#endif	// DEL-BY-LEETEN 08/09/2010-END

#include "libopengl.h"
#include "shader.h"
#include "libbuf3d.h"
#include "fbo.h"

// res. of the volume
static	GLuint pt3dVolume[2];
static	GLuint t3dWeightOffset;
static	GLuint pidDiffusion;
static	GLuint fidVolume;
static	TBuffer3D<float4> p3df4Volume;
static	TBuffer3D<float4> p3df4WeightOffset;
static	float4	f4MinWeightOffset;
static	float4	f4MaxWeightOffset;

void
_FlowDiffusionGLGLInit
(
	int iWidth,
	int iHeight,
	int iDepth
)
{
	// create the volumes
	p3df4Volume.alloc(iWidth, iHeight, iDepth);
	p3df4WeightOffset.alloc(iWidth, iHeight, iDepth);

	for(int t = 0; t < 2; t++)
	{
		CREATE_3D_TEXTURE(
			GL_TEXTURE_3D, 
			pt3dVolume[t], 
			GL_NEAREST, GL_RGBA32F_ARB, 
			iWidth, iHeight, iDepth, 
			GL_RGBA, GL_FLOAT, 
			NULL);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	}
	CREATE_3D_TEXTURE(
		GL_TEXTURE_3D, 
		t3dWeightOffset, 
		GL_NEAREST, GL_RGBA32F_ARB, 
		iWidth, iHeight, iDepth, 
		GL_RGBA, GL_FLOAT, 
		NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	// create the FBOs
	glGenFramebuffersEXT(1, &fidVolume);

	// load the shader
	pidDiffusion = CSetShadersByString(
		NULL
		,
		#include "diffusion.frag.h"	
	);
	assert( pidDiffusion );	

	CHECK_OPENGL_ERROR(__FUNCTION__, true);
}

void
_FlowDiffusionGLGL
(
	float fAttenuation,
	int	iNrOfIters,
	float pfWeightVolume[],
	float pfXOffsetVolume[],
	float pfYOffsetVolume[],
	float pfZOffsetVolume[],
	float pfSrcVolume[],
	float pfDstVolume[]
)
{
	// bind the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fidVolume);

	glPushAttrib(
		GL_DEPTH_BUFFER_BIT |
		GL_VIEWPORT_BIT |
		GL_TEXTURE_BIT | 
		0 );

	// disable DEPTH test
	glDisable(GL_DEPTH_TEST);

	// set the viewport to [iVolumeWidth, iVolumeHeight]
	glViewport(0, 0, p3df4Volume.iWidth, p3df4Volume.iHeight);

	// set the modelview and projection matrices
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// normalize the volume to [0, 1] and pack the weight and offset
	for(int		v = 0,	z = 0; z < p3df4Volume.iDepth;	z++)
		for(int			y = 0; y < p3df4Volume.iHeight; y++)
			for(int		x = 0; x < p3df4Volume.iWidth;	x++, v++)
			{
				p3df4Volume[v].x = 0.5f * (pfSrcVolume[3 * v + 0] + 1.0f);
				p3df4Volume[v].y = 0.5f * (pfSrcVolume[3 * v + 1] + 1.0f);
				p3df4Volume[v].z = 0.5f * (pfSrcVolume[3 * v + 2] + 1.0f);
				p3df4Volume[v].w = 1.0;

				p3df4WeightOffset[v].x = pfXOffsetVolume[v];
				p3df4WeightOffset[v].y = pfYOffsetVolume[v];
				p3df4WeightOffset[v].z = pfZOffsetVolume[v];
				p3df4WeightOffset[v].w = pfWeightVolume[v];
			}

	f4MaxWeightOffset.x = -(float)HUGE_VAL;
	f4MaxWeightOffset.y = -(float)HUGE_VAL;
	f4MaxWeightOffset.z = -(float)HUGE_VAL;
	f4MaxWeightOffset.w = -(float)HUGE_VAL;

	f4MinWeightOffset.x =  (float)HUGE_VAL;
	f4MinWeightOffset.y =  (float)HUGE_VAL;
	f4MinWeightOffset.z =  (float)HUGE_VAL;
	f4MinWeightOffset.w =  (float)HUGE_VAL;

	for(int		v = 0,	z = 0; z < p3df4Volume.iDepth;	z++)
		for(int			y = 0; y < p3df4Volume.iHeight; y++)
			for(int		x = 0; x < p3df4Volume.iWidth;	x++, v++)
			{
				f4MaxWeightOffset.x = max(f4MaxWeightOffset.x, p3df4WeightOffset[v].x);
				f4MaxWeightOffset.y = max(f4MaxWeightOffset.y, p3df4WeightOffset[v].y);
				f4MaxWeightOffset.z = max(f4MaxWeightOffset.z, p3df4WeightOffset[v].z);
				f4MaxWeightOffset.w = max(f4MaxWeightOffset.w, p3df4WeightOffset[v].w);

				f4MinWeightOffset.x = min(f4MinWeightOffset.x, p3df4WeightOffset[v].x);
				f4MinWeightOffset.y = min(f4MinWeightOffset.y, p3df4WeightOffset[v].y);
				f4MinWeightOffset.z = min(f4MinWeightOffset.z, p3df4WeightOffset[v].z);
				f4MinWeightOffset.w = min(f4MinWeightOffset.w, p3df4WeightOffset[v].w);
			}

	assert( 
		f4MaxWeightOffset.x > f4MinWeightOffset.x &&
		f4MaxWeightOffset.y > f4MinWeightOffset.y &&
		f4MaxWeightOffset.z > f4MinWeightOffset.z &&
		f4MaxWeightOffset.w > f4MinWeightOffset.w );

	for(int		v = 0,	z = 0; z < p3df4Volume.iDepth;	z++)
		for(int			y = 0; y < p3df4Volume.iHeight; y++)
			for(int		x = 0; x < p3df4Volume.iWidth;	x++, v++)
			{
				p3df4WeightOffset[v].x = (p3df4WeightOffset[v].x - f4MinWeightOffset.x) / (f4MaxWeightOffset.x -  f4MinWeightOffset.x) ;
				p3df4WeightOffset[v].y = (p3df4WeightOffset[v].y - f4MinWeightOffset.y) / (f4MaxWeightOffset.y -  f4MinWeightOffset.y) ;
				p3df4WeightOffset[v].z = (p3df4WeightOffset[v].z - f4MinWeightOffset.z) / (f4MaxWeightOffset.z -  f4MinWeightOffset.z) ;
				p3df4WeightOffset[v].w = (p3df4WeightOffset[v].w - f4MinWeightOffset.w) / (f4MaxWeightOffset.w -  f4MinWeightOffset.w) ;
			}

	// upload the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, pt3dVolume[0]);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 
		0, 0, 0, 
		p3df4Volume.iWidth, p3df4Volume.iHeight, p3df4Volume.iDepth,
		GL_RGBA, GL_FLOAT, &p3df4Volume[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, t3dWeightOffset);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 
		0, 0, 0, 
		p3df4WeightOffset.iWidth, p3df4WeightOffset.iHeight, p3df4WeightOffset.iDepth,
		GL_RGBA, GL_FLOAT, &p3df4WeightOffset[0]);

	glActiveTexture(GL_TEXTURE0);

	// enable the shader
	glUseProgramObjectARB(pidDiffusion);
	SET_1I_VALUE_BY_NAME(	pidDiffusion, "t3dSrc",				0);
	SET_1I_VALUE_BY_NAME(	pidDiffusion, "t3dWeightOffset",	1);
	SET_1F_VALUE_BY_NAME(	pidDiffusion, "fWidth",				(float)p3df4Volume.iWidth);
	SET_1F_VALUE_BY_NAME(	pidDiffusion, "fHeight",			(float)p3df4Volume.iHeight);
	SET_1F_VALUE_BY_NAME(	pidDiffusion, "fDepth",				(float)p3df4Volume.iDepth);
	SET_1F_VALUE_BY_NAME(	pidDiffusion, "fAttenuation",		fAttenuation);
	SET_4FV_VALUE_BY_NAME(	pidDiffusion, "f4MaxWeightOffset",	1,		(float*)&f4MaxWeightOffset);
	SET_4FV_VALUE_BY_NAME(	pidDiffusion, "f4MinWeightOffset",	1, 		(float*)&f4MinWeightOffset);

	int iSrc = 0, iDst = 1;
	for(int i = 0; i < iNrOfIters; i++, iSrc = 1 - iSrc)
	{
		// switch the texutre 
		iDst = 1 - iSrc;

		glBindTexture(GL_TEXTURE_3D, pt3dVolume[iSrc]);

		for(int z = 0; z < p3df4Volume.iDepth; z += NR_OF_GLSL_TARGETS)
		{
			SET_1F_VALUE_BY_NAME(pidDiffusion, "fZ",	float(z));
			int iNrOfTargets = (z + NR_OF_GLSL_TARGETS < p3df4Volume.iDepth)?NR_OF_GLSL_TARGETS:(p3df4Volume.iDepth - z);
			SET_1I_VALUE_BY_NAME(pidDiffusion, "iNrOfTargets",	iNrOfTargets);

			static GLuint puRdIds[] = 
			{
				GL_COLOR_ATTACHMENT0_EXT,
				GL_COLOR_ATTACHMENT1_EXT,
				GL_COLOR_ATTACHMENT2_EXT,
				GL_COLOR_ATTACHMENT3_EXT,
				GL_COLOR_ATTACHMENT4_EXT,
				GL_COLOR_ATTACHMENT5_EXT,
				GL_COLOR_ATTACHMENT6_EXT,
				GL_COLOR_ATTACHMENT7_EXT,
			};
			glDrawBuffers(iNrOfTargets, puRdIds);

			// swithc the dst. framebuffer
			for(int f = 0; f < iNrOfTargets; f++)
			{
				glFramebufferTexture3DEXT(
					GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+f,
					GL_TEXTURE_3D, pt3dVolume[iDst], 0, z+f);
			}
			// MOD-BY-LEETEN 08/09/2010-FROM:
				// assert( GL_FRAMEBUFFER_COMPLETE_EXT == EGetFboStatus() );
			// TO:
			unsigned int uFboStatus = EGetFboStatus();
			assert( GL_FRAMEBUFFER_COMPLETE_EXT == uFboStatus );
			// MOD-BY-LEETEN 08/09/2010-END

			glBegin(GL_QUADS);
				glVertex2i(-1, -1);
				glVertex2i(+1, -1);
				glVertex2i(+1, +1);
				glVertex2i(-1, +1);
			glEnd();
		}
	}

	// disable the shader
	glUseProgramObjectARB(0);

	// download the texture back and denormalize the value to [-1, +1]
	glBindTexture(GL_TEXTURE_3D, pt3dVolume[iDst]);
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, &p3df4Volume[0]);
	for(int		v = 0,	z = 0; z < p3df4Volume.iDepth;	z++)
		for(int			y = 0; y < p3df4Volume.iHeight; y++)
			for(int		x = 0; x < p3df4Volume.iWidth;	x++, v++)
			{
				pfDstVolume[3 * v + 0] = 2.0f * p3df4Volume[v].x - 1.0f;
				pfDstVolume[3 * v + 1] = 2.0f * p3df4Volume[v].y - 1.0f;
				pfDstVolume[3 * v + 2] = 2.0f * p3df4Volume[v].z - 1.0f;
			}

	// reset the FBO to 0
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// reset the modelview and projection matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();	

	CHECK_OPENGL_ERROR(__FUNCTION__, true);
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/02/05 01:37:28  leeten

[02/02/2010]
1. [1ST] First time checkin.


*/
