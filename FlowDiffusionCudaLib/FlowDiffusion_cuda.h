#ifndef __FLOW_DIFFUSION_CUDA_H__
#define __FLOW_DIFFUSION_CUDA_H__

	// ADD-BY-LEETEN 12/07/2009-BEGIN
	#define UPDATE_HISTOGRAM_ON_GPU				0
	#define COMPUTE_ENTROPY_ON_GPU				0
	#define	_ComputeEntropyVolume_PRINT_TIMING	1
	// ADD-BY-LEETEN 12/07/2009-END

	// ADD-BY-LEETEN 2009/11/25-BEGIN
	// MOD-BY-LEETEN 2009/12/17-FROM:
		// #define USE_CUDPP	0
	// TO:
	#define CHECK_ERROR_CONVERGENCE_BY_CUDPP	0
	// MOD-BY-LEETEN 2009/12/17-END

	// DEL-BY-LEETEN 2009/12/17-BEGIN
	/*
	#if	CHECK_ERROR_CONVERGENCE_BY_CUDPP	
		#include "cudpp/cudpp.h"
		#pragma comment (lib, "cudpp32.lib")
	#endif
	*/
	// DEL-BY-LEETEN 2009/12/17-END

	// ADD-BY-LEETEN 2009/11/25-END

	#include "cuda_macro.h"

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	#include "liblog.h"
	using namespace std;

	// DEL-BY-LEETEN 2009/12/17-BEGIN
	// 	#define	M_PI	3.1415926535897932384626433832795f
	// DEL-BY-LEETEN 2009/12/17-END

	// ADD-BY-LEETEN 12/14/2009-END

	#include "libbuf.h"

////////////////////////////////////////////
	#define PRINT_FLOW_FUSION_TIMING	1
	#define USE_SHARED_MEMORY			0

	// ADD-BY-LEETEN 12/14/2009-BEGIN
	#define SHOW_COMPUTE_SRC_BIN_VOLUME_TIMING	1	
	// ADD-BY-LEETEN 12/14/2009-END

	// ADD-BY-LEETEN 11/04/2009-BEGIN
						// if this preprocessor is non zero, the volume is scanned via a for loop on the host
	#define DIFFUSION_BY_FOR_LOOP_ON_HOST		0
	// ADD-BY-LEETEN 11/04/2009-END

	// ADD-BY-LEETEN 10/02/2009-BEGIN
	#define BLOCK_DIM_X	16
	#define BLOCK_DIM_Y	12
	// ADD-BY-LEETEN 10/02/2009-END

#endif	// __FLOW_DIFFUSION_CUDA_H__

/*

$Log: not supported by cvs2svn $

*/
