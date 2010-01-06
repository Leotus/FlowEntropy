#ifndef __FLOW_DIFFUSION_CUDA_H__
#define	__FLOW_DIFFUSION_CUDA_H__

#define USE_CUDA			0
#define ENTER_GLUT_LOOP		0
#define BIN_LOOKUP			1

// ADD-BY-LEETEN 12/14/2009-BEGIN
#define BIN_LOOKUP_ON_GPU	1
#define KEEP_BOUNDARY		0
// ADD-BY-LEETEN 12/14/2009-END

#define NR_OF_STREAMLINES	1

#define SCALE_SIZE			3.0

#define SHOW_COMPUTE_STREAMLINE_TIMING	1
#define SHOW_SELECT_NEXT_SEED_3_TIMING	0

#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
	#pragma comment (lib, "cutil32.lib ")      // link with my own library libfps
	#pragma comment (lib, "cudart.lib")      // link with my own library libfps
#endif	// DEL-BY-LEETEN 12/07/2009-END
 
// #if	USE_CUDA
	#include "FlowDiffusionCudaLib/FlowDiffusion.h"
// #endif

#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
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

#endif	// DEL-BY-LEETEN 12/07/2009-END

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $
Revision 1.3  2010/01/04 18:02:38  leeten

[01/04/2010]
1. [MOD] Enable the GPU-based comutation of bins.
2. [MOD] Reduce #streamlines to 1.

Revision 1.2  2009/12/15 20:08:17  leeten

[12/15/2009]
1. [ADD] Define a new preprocessor BIN_LOOKUP_ON_GPU. If it is 1, the vector field is converted to the discrete bin via CUDA.
2. [ADD] Define a new preprocessor KEEP_BOUNDARY. If it is 1, the vector field along the boundary will be preserved.

Revision 1.1  2009/12/07 20:16:04  leeten

[12/07/2009]
1. [1ST] First time checkin.

Revision 1.1.1.1  2009/12/05 21:31:07  leeten

[12/04/2009]
1. [1ST] First time checkin.


*/
