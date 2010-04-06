#ifndef __FLOW_DIFFUSION_CUDA_H__
#define	__FLOW_DIFFUSION_CUDA_H__

// ADD-BY-LEETEN 02/04/2010-BEGIN
#define	DIFFUSION_ON_GLSL	0
#include "FlowDiffusionGLSLLib/FlowDiffusionGLSLLib.h"
// ADD-BY-LEETEN 02/04/2010-END

#define CLOCK_reconstruct_field_GVF_3D	0

#define USE_CUDA			0
#define ENTER_GLUT_LOOP		0
#define BIN_LOOKUP			1


// ADD-BY-LEETEN 02/02/2010-BEGIN
#if	USE_CUDA
	#define DIFFUSION_ON_CUDA	1
	// MOD-BY-LEETEN 02/04/2010-FROM:
		// #define BIN_LOOKUP_ON_GPU	1
	// TO:
	#define BIN_LOOKUP_ON_CUDA	1
	// MOD-BY-LEETEN 02/04/2010-END

	#if		BIN_LOOKUP_ON_CUDA
		// MOD-BY-LEETEN 02/04/2010-FROM:
			// #define	ENTROPY_ON_GPU		0
		// TO:	
		#define	ENTROPY_ON_CUDA		0
		// MOD-BY-LEETEN 02/04/2010-END
	#endif	// #if	BIN_LOOKUP_ON_CUDA	
#endif	// #if	USE_CUDA
// ADD-BY-LEETEN 02/02/2010-END

// ADD-BY-LEETEN 02/04/2010-BEGIN
#if	DIFFUSION_ON_CUDA
	#define	DIFFUSION_ON_GLSL	0
#endif

// ADD-BY-LEETEN 02/04/2010-END

// ADD-BY-LEETEN 02/02/2010-BEGIN
#define KERNEL_HALF_WIDTH	6
#define KERNEL_HALF_HEIGHT	6
#define KERNEL_HALF_DEPTH	6
// ADD-BY-LEETEN 02/02/2010-END

// ADD-BY-LEETEN 12/14/2009-BEGIN
// DEL-BY-LEETEN 02/02/2010		#define BIN_LOOKUP_ON_CUDA	1
#define KEEP_BOUNDARY		0
// ADD-BY-LEETEN 12/14/2009-END

// ADD-BY-LEETEN 02/04/2010-BEGIN
#define PRUNING_THRESHOLD	0.000
// MOD-BY-LEETEN 03/18/2010-FROM:
	// #define	NR_OF_SAMPLES		((grid_res[0] * grid_res[1] * grid_res[2]) / 512)
// TO:
// MOD-BY-LEETEN 03/25/2010-FROM:
	// #define	NR_OF_SAMPLES		(max(max(grid_res[0], grid_res[1]), grid_res[2]))
// TO:
// MOD-BY-LEETEN 04/01/2010-FROM:
	// #define	NR_OF_SAMPLES		(int((pow(double(max(max(grid_res[0], grid_res[1]), grid_res[2])), 1.5))))
// TO:
#define	NR_OF_SAMPLES		(int(double(sqrt(double(grid_res[0] * grid_res[1] * grid_res[2])))))
// MOD-BY-LEETEN 04/01/2010-END
// MOD-BY-LEETEN 03/25/2010-END
// MOD-BY-LEETEN 03/18/2010-END
// ADD-BY-LEETEN 02/04/2010-END

// ADD-BY-LEETEN 03/18/2010-BEGIN
#define NR_OF_BINS		360	

//////////////////////////////////////////////////////
#define WRAP_MODE_CLAMP				0x01
#define WRAP_MODE_MIRROR			0x02
#define	WRAP_MODE_REPEAT			0x03
#define WRAP_MODE_CLAMP_TO_BORDER	0x04

#define WRAP_MODE					WRAP_MODE_MIRROR	// WRAP_MODE_MIRROR

//////////////////////////////////////////////////////
#define PRUNING_MODE_NONE				0x00
#define	PRUNING_MODE_COND_ENTROPY		0x01
#define	PRUNING_MODE_KEEP_WHEN_DISTANT	0x02

#define PRUNING_MODE					PRUNING_MODE_KEEP_WHEN_DISTANT	// PRUNING_MODE_COND_ENTROPY
// ADD-BY-LEETEN 03/18/2010-END

// ADD-BY-LEETEN 03/19/2010-BEGIN
#define SEPARATION_DISTANCE			(ceilf(float(min(min(grid_res[0], grid_res[1]), grid_res[2])) * 0.02f))
#define SAMPLE_LOCAL_MAX_FIRST							1	// 1 or 0
#define	STOP_ADVENTION_WHEN_TOO_CLOSE_AND_LOW_ENTROPY	0	// 1 or 0
// ADD-BY-LEETEN 03/19/2010-BEGIN

// ADD-BY-LEETEN 03/25/2010-BEGIN
// threshold of ratio to detect the salient local max.
#define	LOCAL_MAX_THRESHOLD		0.7f

#define	KERNEL_RADIUS_AROUND_CRITICAL_POINT		1
#define KERNEL_LENGTH_AROUND_CRITICAL_POINT		(2 * KERNEL_RADIUS_AROUND_CRITICAL_POINT	 + 1)

// not supported in 3D		#define KERNEL_SHAPE_CIRCLE		0x01
// not supported in 3D		#define KERNEL_SHAPE_SQUARE		0x02
#define KERNEL_SHAPE_DIAMOND	0x03
#define KERNEL_SHAPE_CUBE		0x04

#define KERNEL_SHAPE	KERNEL_SHAPE_DIAMOND	// KERNEL_SHAPE_CUBE	

#if	KERNEL_SHAPE	== KERNEL_SHAPE_DIAMOND

	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		(KERNEL_RADIUS_AROUND_CRITICAL_POINT * 27 + 1)

#elif	KERNEL_SHAPE	== KERNEL_SHAPE_CUBE		

	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		(KERNEL_LENGTH_AROUND_CRITICAL_POINT * KERNEL_LENGTH_AROUND_CRITICAL_POINT * KERNEL_LENGTH_AROUND_CRITICAL_POINT)

#else

	#define KERNEL_SIZE_AROUND_CRITICAL_POINT		1

#endif	// #if	KERNEL_SHAPE	
// ADD-BY-LEETEN 03/25/2010-END

// ADD-BY-LEETEN 04/01/2010-BEGIN
#define ENTROPY_DEPENDENT_SEPARATION_DISTANCE			1	// 1 or 0
// ADD-BY-LEETEN 04/01/2010-END

// max. #iterations
#define	MAX_NR_OF_ITERATIONS							1000	// 5
// #endif


#define NR_OF_STREAMLINES	1

#define SCALE_SIZE			3.0

#define SHOW_COMPUTE_STREAMLINE_TIMING	1
#define SHOW_SELECT_NEXT_SEED_3_TIMING	0

#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
#endif	// DEL-BY-LEETEN 12/07/2009-END
 
// #if	USE_CUDA
	#include "FlowDiffusionCudaLib/FlowDiffusion.h"
// #endif

#if	0	// DEL-BY-LEETEN 12/07/2009-BEGIN
#endif	// DEL-BY-LEETEN 12/07/2009-END

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $
Revision 1.6  2010/04/02 10:33:15  leeten

[04/02/2010]
1. [ADD] Add a new preprocessor MAX_NR_OF_ITERATIONS.
2. [MOD] Change #samples to square root of the #voxels.

Revision 1.5  2010/03/26 14:59:59  leeten

[03/26/2010]
1. [MOD] Define a new kernel size from 8 to 6.
2. [MOD] Change the #samples from min(dim) to min(dim) ^ 1.5.
3. [ADD] Specify the threshold LOCAL_MAX_THRESHOLD to 0.7.
4. [ADD] Define kernel shapes KERNEL_SHAPE_DIAMOND and KERNEL_SHAPE_CUBE.

Revision 1.4  2010/03/19 19:53:03  leeten

[03/19/2010]
1. [ADD] Define a new preprocessor SEPARATION_DISTANCE to specify the separation distance.
2. [ADD] Defien new preprocessors SAMPLE_LOCAL_MAX_SAMPLED_FIRSTand STOP_ADVENTION_WHEN_TOO_CLOSE_AND_LOW_ENTROPY. (They are not used now).

Revision 1.3  2010/03/18 16:26:29  leeten

[03/18/2010]
1. [MOD] Change NR_OF_SAMPLES to the max. dim. of the data.
2. [ADD] Define a new preprocessor NR_OF_BINS as #bins. of the histogram.
3. [ADD] Define new preprocessors WRAP_MODE_CLAMP, WRAP_MODE_MIRROR, WRAP_MODE_REPEAT and WRAP_MODE_CLAMP_TO_BORDER to control the wrapping (decided by WRAP_MODE).
4. [ADD] Define new preprocessors PRUNING_MODE_NONE, PRUNING_MODE_COND_ENTROPY, and PRUNING_MODE_KEEP_WHEN_DISTANT to control the wrapping (decided by PRUNING_MODE).
5. [DEL] Remove useless old code segments.

Revision 1.2  2010/02/05 01:29:30  leeten

[02/02/2010]
1. [ADD] Add a new preprocessor DIFFUSION_ON_GLSL and include a header FlowDiffusionGLSLLib.h
2. [MOD] Change the preprocessors BIN_LOOKUP_ON_GPU, DIFFUSION_ON_GPU, ENTROPY_ON_GPU to BIN_LOOKUP_ON_CUDA, DIFFUSION_ON_CUDA, ENTROPY_ON_CUDA, respectively.
3. [ADD] Add a new preprocessor PRUNING_THRESHOLD.
4. [ADD] Add a new preprocessor NR_OF_SAMPLES.

Revision 1.1  2010/02/03 00:40:28  leeten

[02/02/2010]
1. [1ST] First time checkin.

Revision 1.4  2010/01/06 17:28:36  leeten

[01/06/2010]
1. [MOD] Change the preprosessor KEEP_BOUNDARY to 0.
2. [MOD] Change the preprocessor USE_CUDA to 0.
3. [MOD] Change the preprocessor ENTER_GLUT_LOOP to 0.

Revision 1.3  2010/01/04 18:02:38  leeten

[01/04/2010]
1. [MOD] Enable the GPU-based comutation of bins.
2. [MOD] Reduce #streamlines to 1.

Revision 1.2  2009/12/15 20:08:17  leeten

[12/15/2009]
1. [ADD] Define a new preprocessor BIN_LOOKUP_ON_CUDA. If it is 1, the vector field is converted to the discrete bin via CUDA.
2. [ADD] Define a new preprocessor KEEP_BOUNDARY. If it is 1, the vector field along the boundary will be preserved.

Revision 1.1  2009/12/07 20:16:04  leeten

[12/07/2009]
1. [1ST] First time checkin.

Revision 1.1.1.1  2009/12/05 21:31:07  leeten

[12/04/2009]
1. [1ST] First time checkin.


*/
