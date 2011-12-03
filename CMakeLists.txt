cmake_minimum_required(VERSION 2.6)

PROJECT(SuperPixelSegmentation)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Where to copy executables when 'make install' is run
SET( INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )

add_library(libSuperPixelSegmentation
disjoint-set.cxx imconv.cxx filter.cxx segment-graph.cxx segment-image.cxx)

SET(BuildVTK OFF CACHE BOOL "Build VTK segmentation?")
if(BuildVTK)
  FIND_PACKAGE(VTK REQUIRED)
  INCLUDE(${VTK_USE_FILE})

  ADD_EXECUTABLE(vtkSuperPixelSegmentation VTKSegmentationExample.cpp vtkSuperPixelSegmentation.cxx)
  TARGET_LINK_LIBRARIES(vtkSuperPixelSegmentation ${VTK_LIBRARIES} libSuperPixelSegmentation)
endif()

SET(BuildITK OFF CACHE BOOL "Build ITK segmentation?")
if(BuildITK)
  FIND_PACKAGE(ITK REQUIRED)
  INCLUDE(${ITK_USE_FILE})

  ADD_EXECUTABLE(itkSuperPixelSegmentation ITKSegmentationExample.cpp)
  TARGET_LINK_LIBRARIES(itkSuperPixelSegmentation ${ITK_LIBRARIES} libSuperPixelSegmentation)
endif()

SET(BuildTesting OFF CACHE BOOL "Build testing?")
if(BuildTesting)
  add_subdirectory(Testing)
endif()

SET(BuildGUI OFF CACHE BOOL "Build GUI?")
if(BuildGUI)
  add_subdirectory(GUI)
endif()
