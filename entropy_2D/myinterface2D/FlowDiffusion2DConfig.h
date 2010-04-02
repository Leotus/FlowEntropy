#ifndef __FLOW_DIFFUSION_CUDA_H__
#define	__FLOW_DIFFUSION_CUDA_H__

#include "FlowDiffusionCudaLib/FlowDiffusion.h"

// ADD-BY-LEETEN 03/10/2010-BEGIN
#define ENTER_GLUT_LOOP				0
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

// ADD-BY-LEETEN 03/24/2010-BEGIN
#if	0	// MOD-BY-LEETEN 04/01/2010-FROM:
	#define	STREAMLINE_INIT_STEP_SIZE	0.2f
	#define	STREAMLINE_MAX_STEP_SIZE	1.0f
#else	// MOD-BY-LEETEN 04/01/2010-TO:
#define	STREAMLINE_INIT_STEP_SIZE	1.0f
#define	STREAMLINE_MAX_STEP_SIZE	2.0f
#endif	// MOD-BY-LEETEN 04/01/2010-END
// ADD-BY-LEETEN 03/24/2010-END

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
#define PRUNING_THRESHOLD	0.05
#define	NR_OF_SAMPLES		(max(grid_res[0], grid_res[1]))		// ((grid_res[0] * grid_res[1]) / 64)

// ADD-BY-LEETEN 03/15/2010-BEGIN
#define NR_OF_BINS		64	// 60		// 60
#define	KERNEL_SIZE		6	// TEST	// 8		// 8

#define	IMPORTANCE_SAMPLING_CHAIN_RULE			0x01
#define	IMPORTANCE_SAMPLING_REJECTION_METHOD	0x02

#define	IMPORTANCE_SAMPLING		IMPORTANCE_SAMPLING_CHAIN_RULE		// IMPORTANCE_SAMPLING_CHAIN_RULE	// IMPORTANCE_SAMPLING_REJECTION_METHOD
// ADD-BY-LEETEN 03/15/2010-END

// ADD-BY-LEETEN 03/16/2010-BEGIN
#define WRAP_MODE_CLAMP				0x01
#define WRAP_MODE_MIRROR			0x02
#define	WRAP_MODE_REPEAT			0x03
#define WRAP_MODE_CLAMP_TO_BORDER	0x04

#define WRAP_MODE					WRAP_MODE_MIRROR

// MOD-BY-LEETEN 03/18/2010-FROM:
	// #define	ENABLE_PRUNING				0	// 1 or 0
// TO:
#define PRUNING_MODE_NONE				0x00
#define	PRUNING_MODE_COND_ENTROPY		0x01
#define	PRUNING_MODE_KEEP_WHEN_DISTANT	0x02

#define PRUNING_MODE	PRUNING_MODE_KEEP_WHEN_DISTANT	//	PRUNING_MODE_KEEP_WHEN_DISTANT	// PRUNING_MODE_COND_ENTROPY
// MOD-BY-LEETEN 03/18/2010-END

// ADD-BY-LEETEN 03/19/2010-BEGIN

// MOD-BY-LEETEN 03/22/2010-FROM:
	// #define SEPARATION_DISTANCE			(ceilf(float(max(grid_res[0], grid_res[1])) * 0.02f))
// TO:
#define SEPARATION_DISTANCE			(ceilf(float(min(grid_res[0], grid_res[1])) * 0.02f))
// MOD-BY-LEETEN 03/22/2010-END

// ADD-BY-LEETEN 03/26/2010-BEGIN
#define ENTROPY_DEPENDENT_SEPARATION_DISTANCE						1	// 1 or 0
// ADD-BY-LEETEN 03/26/2010-END

// ADD-BY-LEETEN 04/01/2010-BEGIN
#define	MAX_NR_OF_ITERATIONS										1000	// 5
// ADD-BY-LEETEN 04/01/2010-END

#define SAMPLE_LOCAL_MAX_FIRST							1	// 1 or 0
#define	STOP_ADVENTION_WHEN_TOO_CLOSE_AND_LOW_ENTROPY	0	// 1 or 0

// ADD-BY-LEETEN 03/22/2010-BEGIN
#define	LOCAL_MAX_THRESHOLD		0.7f	// 0.7f

#define	KERNEL_RADIUS_AROUND_CRITICAL_POINT		1	// TEST 2
#define KERNEL_LENGTH_AROUND_CRITICAL_POINT		(2 * KERNEL_RADIUS_AROUND_CRITICAL_POINT	 + 1)

// DEL-BY-LEETEN 03/23/2010-BEGIN
	// #define KERNEL_SIZE_AROUND_CRITICAL_POINT		(KERNEL_LENGTH_AROUND_CRITICAL_POINT * KERNEL_LENGTH_AROUND_CRITICAL_POINT)
// DEL-BY-LEETEN 03/23/2010-END

// ADD-BY-LEETEN 03/23/2010-BEGIN
#define KERNEL_SHAPE_CIRCLE	0x01
#define KERNEL_SHAPE_SQUARE	0x02
// ADD-BY-LEETEN 03/25/2010-BEGIN
#define KERNEL_SHAPE_DIAMOND	0x03
// ADD-BY-LEETEN 03/25/2010-END

#define KERNEL_SHAPE	KERNEL_SHAPE_DIAMOND	// KERNEL_SHAPE_CIRCLE	

#if				KERNEL_SHAPE	== KERNEL_SHAPE_CIRCLE	
// MOD-BY-LEETEN 03/24/2010-FROM:
	// #define KERNEL_SIZE_AROUND_CRITICAL_POINT		(NR_OF_BINS)
// TO:
	// add one point at the center of the kernel
	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		(NR_OF_BINS + 1)

// ADD-BY-LEETEN 03/25/2010-BEGIN
#elif			KERNEL_SHAPE	== KERNEL_SHAPE_DIAMOND
	// MOD-BY-LEETEN 04/01/2010-FROM:
		//	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		(KERNEL_RADIUS_AROUND_CRITICAL_POINT * 9)
	//	TO:
	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		((KERNEL_RADIUS_AROUND_CRITICAL_POINT + 1) * 9)
	// MOD-BY-LEETEN 04/01/2010-END
// ADD-BY-LEETEN 03/25/2010-END
// MOD-BY-LEETEN 03/24/2010-END
#elif			KERNEL_SHAPE	== KERNEL_SHAPE_SQUARE	
	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		(KERNEL_LENGTH_AROUND_CRITICAL_POINT * KERNEL_LENGTH_AROUND_CRITICAL_POINT)
#endif	// #if	KERNEL_SHAPE	

// ADD-BY-LEETEN 03/23/2010-END


// ADD-BY-LEETEN 03/22/2010-END

// ADD-BY-LEETEN 03/19/2010-END

// ADD-BY-LEETEN 03/16/2010-END

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $
Revision 1.10  2010/03/26 14:57:05  leeten

[03/26/2010]
1. [MOD] Change the threshold LOCAL_MAX_THRESHOLD from 0.5 to 0.7.
2. [MOD] Change the readius of the kernel to 2.
3. [ADD] Define a new kernel shape KERNEL_SHAPE_DIAMOND.

Revision 1.9  2010/03/24 16:19:05  leeten

[03/23/2010]
1. [MOD] Change the #bins from 32 to 64.
2. [ADD] Specify the line integral step size by two preprocessors STREAMLINE_INIT_STEP_SIZE and STREAMLINE_MAX_STEP_SIZE.
3. [ADD]  Change the threshold for local max to 0.5.

Revision 1.8  2010/03/23 19:40:56  leeten

[03/23/2010]
1. [ADD] Define new preprocessors KERNEL_SHAPE, KERNEL_SHAPE_CIRCLE and KERNEL_SHAPE_SQUARE to control the kernel near the critical points.

Revision 1.7  2010/03/22 13:46:51  leeten

[03/22/2010]
1. [MOD] Change the separation distance SEPARATION_DISTANCE from 0.02 times max. dim. to min. dim.
2. [ADD] Define a preprocessor LOCAL_MAX_THRESHOLD to define the lower bound for local max.
3. [ADD] Define preprocessor KERNEL_RADIUS_AROUND_CRITICAL_POINT, KERNEL_LENGTH_AROUND_CRITICAL_POINT and KERNEL_SIZE_AROUND_CRITICAL_POINT as the kernel sizes.

Revision 1.6  2010/03/19 19:48:24  leeten

[03/19/2010]
1. [ADD] Define a new preprocessor SEPARATION_DISTANCE to specify the separation distance btw the streamlines.
2. [ADD] Define a new preprocessor SAMPLE_LOCAL_MAX_FIRST to decide whether the local max should be sampled first.
3. [ADD] Define a new preprocessor STOP_ADVENTION_WHEN_TOO_CLOSE_AND_LOW_ENTROPY to decide whether the line advention should stop when it is too closed to existing streamlines and the local entropy is lower than a threshold.

Revision 1.5  2010/03/18 16:15:42  leeten

[03/18/2010]
1. [ADD] Remvoe the preprocessor ENABLE_PRUNING and define new ones: PRUNING_MODE_NONE, PRUNING_MODE_COND_ENTROPY, PRUNING_MODE_KEEP_WHEN_DISTANT and PRUNING_MODE.

Revision 1.4  2010/03/16 15:39:46  leeten

[03/16/2010]
1. [ADD] Add new preprocessor to define the wrapping for field boundary.
2. [ADD] ADD a new preprocessor ENABLE_PRUNING to decide whether the pruning is on.

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
