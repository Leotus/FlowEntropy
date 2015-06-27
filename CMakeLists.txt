# The name of our project is "HELLO". CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (FlowEntropy) 

find_package(CUDA)
find_package(ThirdPartyLib)
find_package(mylib)

include_directories(.)
include_directories("${CUDA_INCLUDE_DIRS}")

include_directories(FlowDiffusionCudaLib)
add_subdirectory(FlowDiffusionCudaLib)

include_directories(entropy_final_3D/OSUFlow_sequential)
add_subdirectory(entropy_final_3D/OSUFlow_sequential)
add_dependencies(OSUFlowExe FlowDiffusionCudaLib)

# TEST-MOD: # add_subdirectory(FlowEntropyViewer)
add_subdirectory(FlowEntropyViewer)
# TEST-MOD-END
