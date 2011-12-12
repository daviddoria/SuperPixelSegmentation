cmake_minimum_required(VERSION 2.6)

PROJECT(SuperPixelSegmentation)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/GraphCut)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/SLIC)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/QuickShift)

# Where to copy executables when 'make install' is run
SET( INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )

add_subdirectory(GraphCut)
add_subdirectory(SLIC)
add_subdirectory(QuickShift)

SET(BuildTesting OFF CACHE BOOL "Build testing?")
if(BuildTesting)
  add_subdirectory(Testing)
endif()

SET(BuildGUI OFF CACHE BOOL "Build GUI?")
if(BuildGUI)
  add_subdirectory(GUI)
endif()
