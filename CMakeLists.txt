cmake_minimum_required(VERSION 2.6)

PROJECT(SuperPixelSegmentation)
ENABLE_TESTING()

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

add_library(libSuperPixelSegmentation
disjoint-set.cxx
imconv.cxx
filter.cxx
segment-graph.cxx
segment-image.cxx)

ADD_EXECUTABLE(vtkSuperPixelSegmentation Example.cxx vtkSuperPixelSegmentation.cxx)
TARGET_LINK_LIBRARIES(vtkSuperPixelSegmentation ${VTK_LIBRARIES} libSuperPixelSegmentation)

SET(BuildTesting OFF CACHE BOOL "Build testing?")
if(BuildTesting)
  add_subdirectory(Testing)
endif()