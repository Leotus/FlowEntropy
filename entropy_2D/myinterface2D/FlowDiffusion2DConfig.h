#ifndef __FLOW_DIFFUSION_CUDA_H__
#define	__FLOW_DIFFUSION_CUDA_H__

#include "FlowDiffusionCudaLib/FlowDiffusion.h"

// ADD-BY-LEETEN 03/10/2010-BEGIN
#define ENTER_GLUT_LOOP	0
// ADD-BY-LEETEN 03/10/2010-END

// ADD-BY-LEETEN 02/06/2010-BEGIN
#define	DUMP_WHEN_ENTROPY_INCREASE	0
#define	IMPLEMENTED_BY_XUL			0
// ADD-BY-LEETEN 02/06/2010-END

#define	USE_CUDA	0
#if	USE_CUDA
	#define DIFFUSION_ON_CUDA	1
#endif	// #if	USE_CUDA

#define	GENERATE_PRIMITIVE_GLYPHS		0x0001
#define	GENERATE_PRIMITIVE_STREAMLINES	0x0002

#define GENERATE_PRIMITIVE				GENERATE_PRIMITIVE_STREAMLINES

#define KERNEL_HALF_WIDTH	8	
#define KERNEL_HALF_HEIGHT	8	
#define KERNEL_HALF_DEPTH	8	

// MOD-BY-LEETEN 02/08/2010-FROM:
	// #define IGNORE_BOUNDARY
// TO:
#if		IMPLEMENTED_BY_XUL
		#define DONOT_APPLY_DIFFUSION_ON_BOUNDARY		1
		#define INIT_BOUNDARY_AS_STREAMLINE				1
#endif
// MOD-BY-LEETEN 02/08/2010-END

#define	TARGET_ERROR		0.01	// (1.0f / float(grid_res[0] * grid_res[1]))	// the target error to decide the threshold accroding to Fano's inequality
#define PRUNING_THRESHOLD	0.000
#define	NR_OF_SAMPLES		(max(grid_res[0], grid_res[1]))		// ((grid_res[0] * grid_res[1]) / 64)

// ADD-BY-LEETEN 03/15/2010-BEGIN
#define NR_OF_BINS		60	// 60
#define	KERNEL_SIZE		8		// 8

#define	IMPORTANCE_SAMPLING_CHAIN_RULE			0x01
#define	IMPORTANCE_SAMPLING_REJECTION_METHOD	0x02

#define	IMPORTANCE_SAMPLING	IMPORTANCE_SAMPLING_CHAIN_RULE	// IMPORTANCE_SAMPLING_REJECTION_METHOD
// ADD-BY-LEETEN 03/15/2010-END

// ADD-BY-LEETEN 03/16/2010-BEGIN
#define WRAP_MODE_CLAMP				0x01
#define WRAP_MODE_MIRROR			0x02
#define	WRAP_MODE_REPEAT			0x03
#define WRAP_MODE_CLAMP_TO_BORDER	0x04

#define WRAP_MODE					WRAP_MODE_MIRROR

#define	ENABLE_PRUNING				1	// 1 or 0
// ADD-BY-LEETEN 03/16/2010-END

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $
Revision 1.3  2010/03/15 19:12:51  leeten

[03/15/2010]
1. [ADD] Define a new preprocess KERNEL_SIZE to define the size of the neighborhood.
2. [ADD] Define a the new preprocess NR_OF_BINS to define #histogram bins.
3. [ADD] Define new preprocessors IMPORTANCE_SAMPLING, IMPORTANCE_SAMPLING_CHAIN_RULE, IMPORTANCE_SAMPLING_REJECTION_METHOD to specify the importance sampling algorithm.

Revision 1.2  2010/03/10 20:25:24  leeten

[03/10/2010]
1. [ADD] Define a new preprocessor ENTER_GLUT_LOOP. If the preprocessor ENTER_GLUT_LOOP is zero, immediate call compute_streamlines().

Revision 1.1  2010/02/09 03:48:02  leeten

[02/02/2010]
1. [1ST] First time checkin.


*/
