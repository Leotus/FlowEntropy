cmake_minimum_required (VERSION 2.8)
project (OSUFlowExe) 

find_package(OpenGL)

find_package(ThirdPartyLib)
find_package(mylib)

# grab all the header files
file(GLOB HEADER_FILES "*.h" )

file(GLOB SRC_FILES "*.C" )

# list(APPEND SRC_FILES
	# Grid.C       
	# polynomials.C  
	# TimeVaryingFieldLine.C 
	# eigenvals.C  
	# Interpolator.C  
	# Rake.C	    
	# Topology.C 
	# eigenvecs.C  
	# IsoSurf.C
	# Solution.C
	# triangulator.C 
	# Element.C
	# StreakLine.C
	# VectorMatrix.C
	# Field.C 
	# PathLine.C
	# Streamline.C
	# FieldLine.C
	# Plot3DReader.C 
	# TimeLine.C
	# OSUFlow.C
	# FileReader.C
	# calc_subvolume.C 
	# FlashAMR.C
	# ParFlow.C
	# flashhdf5_float.C 
	# Repartition.C
	# ComputeFieldLines.C 
	# Partition.C
	# Lattice4D.C
	# LatticeAMR.C 
	# Plot3DReader.C
	# CurvilinearGrid.C
	# )

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

SET_SOURCE_FILES_PROPERTIES( ${SRC_FILES} PROPERTIES LANGUAGE CXX )

target_link_libraries(${PROJECT_NAME} 
    ${ThirdPartyLib_LIBRARIES}
    ${mylib_LIBRARIES}
    FlowDiffusionCudaLib
)

