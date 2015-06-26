# Reference:

#The name of the project
PROJECT(FlowDiffusionCudaLib)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

find_package(CUDA)
find_package(ThirdPartyLib)
find_package(mylib)

list(APPEND INC_DIRS
	${CUDA_TOOLKIT_ROOT_DIR}/include
	${CUDA_SDK_ROOT_DIR}/common/inc
)

CUDA_INCLUDE_DIRECTORIES(
	${INC_DIRS}
)

INCLUDE_DIRECTORIES(
	${INC_DIRS}
)

link_directories(
	${CUDA_TOOLKIT_ROOT_DIR}/lib/Win32
	${CUDA_SDK_ROOT_DIR}/common/lib/Win32
)

CUDA_ADD_LIBRARY(${PROJECT_NAME}
	FlowDiffusion.h
	FlowDiffusion_cuda.h
	
	FlowDiffusion.cu
	EntropyField_cuda.cu
	EntropyField_host.cu
	EntropyFieldOnSparseHistogram_cuda.cu
	EntropyFieldPerScanline_cuda.cu
	EntropyFieldSortPerVoxel_cuda.cu
	EntropyFieldWithMarginalHistogram_cuda.cu
	EntropyFieldWithSorting_cuda.cu
	EntropyFieldWithSortingInVectors_cuda.cu
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	DEBUG_OUTPUT_NAME "${PROJECT_NAME}_d"
	RELEASE_OUTPUT_NAME "${PROJECT_NAME}_r"
	)

# # TEST-DEL-BEGIN
# if( WITH_CUDPP )
    # if( IS_X64 )
        # target_link_libraries(${PROJECT_NAME}
            # cudpp64
        # )
    # else()
        # target_link_libraries(${PROJECT_NAME}
            # cudpp32
        # )
    # endif()
# endif()
# # TEST-DEL-END
	
install(
	TARGETS ${PROJECT_NAME}
	DESTINATION lib	)	

file(GLOB HEADER_FILES "*.h" )
install(
	FILES ${HEADER_FILES}
	DESTINATION "include/")
