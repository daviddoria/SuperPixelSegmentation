FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Let Qt find it's MOCed files
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_UI(SuperPixelSegmentationUISrcs SuperPixelSegmentationGUI.ui)
QT4_WRAP_CPP(SuperPixelSegmentationMOCSrcs SuperPixelSegmentationGUI.h SuperPixelSegmentationComputationThread.h ComputationThread.h)

ADD_EXECUTABLE(SuperPixelSegmentation SuperPixelSegmentation.cpp SuperPixelSegmentationGUI.cpp
                ComputationThread.cpp SuperPixelSegmentationComputationThread.cpp SuperPixelSegmentationComputationObject.cpp
                HelpersQt.cpp ${SuperPixelSegmentationUISrcs} ${SuperPixelSegmentationMOCSrcs})
TARGET_LINK_LIBRARIES(SuperPixelSegmentation ${ITK_LIBRARIES} umfpack ${QT_LIBRARIES})
#set_target_properties(PoissonCloning PROPERTIES COMPILE_DEFINITIONS "${DEFINITIONS}")
INSTALL( TARGETS SuperPixelSegmentation RUNTIME DESTINATION ${INSTALL_DIR} )
