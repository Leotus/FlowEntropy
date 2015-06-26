# The name of our project is "HELLO". CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (FlowEntropyViewer) 

find_package(ThirdPartyLib)

find_package(CUDA)
include_directories(
	${CUDA_INCLUDE_DIRS}
	)

find_package(OpenCV)
include_directories(
	${OpenCV_INCLUDE_DIRS}
	)


find_package(mylib)
find_package(osuflow)

list(APPEND HEADER_FILES 
	FlowEntropyViewerWin.h 
	Streamline.h	
	)

list(APPEND SRC_FILES 
	FlowEntropyViewer_main.cpp 
	FlowEntropyViewerWin.cpp	
	Streamline.cpp
	)

list(APPEND SHADER_FILES 
	importance_culling.frag
	importance_filling.frag
	line_drawing.geom
	line_illumination.vert
	ray_integral.frag
	spherical_histogram.geom
	spherical_histogram.vert
	)

foreach(SHADER ${SHADER_FILES})
	list(APPEND SHADER_HEADERS
		${PROJECT_SOURCE_DIR}/${SHADER}.h
	)
	file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
	set(SHADER_OUTPUT_NATIVE_PATH ${SHADER_NATIVE_PATH}.h)
    
    # TEST-ADD-BEGIN
    # Force CMake to convert shader codes to headers first.
    execute_process(COMMAND 
        ${mylib_DIR}/bin/shader2string
        ${SHADER_NATIVE_PATH} 
        ${SHADER_OUTPUT_NATIVE_PATH}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    # TEST-ADD-END
    
	add_custom_command (
		OUTPUT ${SHADER}.h
		MAIN_DEPENDENCY ${SHADER}
		DEPENDS ${SHADER}
		# TEST-MOD: COMMAND ${MYLIB_DIR}/bin/shader2string "${SHADER_NATIVE_PATH}" "${SHADER_OUTPUT_NATIVE_PATH}"
        COMMAND ${mylib_DIR}/bin/shader2string "${SHADER_NATIVE_PATH}" "${SHADER_OUTPUT_NATIVE_PATH}"
        # TEST-MOD-END
	)
    
endforeach(SHADER)
	
# build the project as a library
add_executable(
	${PROJECT_NAME} 
	${SRC_FILES} 
	${HEADER_FILES}
	${SHADER_HEADERS} 
	)


target_link_libraries(
	${PROJECT_NAME}
	${CUDA_CUDART_LIBRARY}
	${OpenCV_LIBS}
	debug lib3ds-120sd	optimized	lib3ds-120s
	debug libopt_d		optimized	libopt_r
	debug libshader_d	optimized	libshader_r
	debug libgw_d		optimized 	libgw_r	
	debug libtfw_d		optimized 	libtfw_r		
	debug libdvr2_d		optimized	libdvr2_r		
	debug libclip_d		optimized	libclip_r
	)