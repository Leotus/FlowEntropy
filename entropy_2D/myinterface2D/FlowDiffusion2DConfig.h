#ifndef __FLOW_DIFFUSION_CUDA_H__
#define	__FLOW_DIFFUSION_CUDA_H__

#include "FlowDiffusionCudaLib/FlowDiffusion.h"

// ADD-BY-LEETEN 03/10/2010-BEGIN
#define ENTER_GLUT_LOOP	1
// ADD-BY-LEETEN 03/10/2010-END

// ADD-BY-LEETEN 02/06/2010-BEGIN
#define	DUMP_WHEN_ENTROPY_INCREASE	0
#define	IMPLEMENTED_BY_XUL			0
// ADD-BY-LEETEN 02/06/2010-END

#define	USE_CUDA	1
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

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/02/09 03:48:02  leeten

[02/02/2010]
1. [1ST] First time checkin.


*/
