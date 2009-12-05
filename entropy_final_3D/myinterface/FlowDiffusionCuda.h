#ifndef __FLOW_DIFFUSION_CUDA_H__
#define	__FLOW_DIFFUSION_CUDA_H__

#define USE_CUDA			1
#define ENTER_GLUT_LOOP		0
#define BIN_LOOKUP			1

#define NR_OF_STREAMLINES	100

#define SCALE_SIZE			3.0f

#pragma comment (lib, "cutil32.lib ")      // link with my own library libfps
#pragma comment (lib, "cudart.lib")      // link with my own library libfps

// #if	USE_CUDA
	#include "../../FlowFusion/FlowFusion_test/FlowFusion.h"
// #endif

#define MALLOC(p, type, size)\
	{	\
		if( NULL == (p) )\
		{\
			(p) = (type*)calloc((size), sizeof(type));\
		}\
	}\


#define FREE(p)	\
	{	\
		if( NULL != (p) )	\
		{\
			free(p);\
		}\
		p = NULL;\
	}\

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $

*/
