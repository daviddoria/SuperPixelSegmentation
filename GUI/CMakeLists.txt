FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

add_subdirectory(Widgets)

# Let Qt find it's MOCed files
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Widgets) # without this, LabeledSlider.h can't find ui_LabeledSlider.h
INCLUDE_DIRECTORIES(Widgets)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

QT4_WRAP_UI(SuperPixelSegmentationUISrcs SuperPixelSegmentationGUI.ui)
QT4_WRAP_CPP(SuperPixelSegmentationMOCSrcs SuperPixelSegmentationGUI.h SuperPixelSegmentationComputationThread.h ComputationThread.h)

ADD_EXECUTABLE(SuperPixelSegmentation SuperPixelSegmentation.cpp SuperPixelSegmentationGUI.cpp
                ComputationThread.cpp SuperPixelSegmentationComputationThread.cpp SuperPixelSegmentationComputationObject.cpp
                HelpersQt.cpp ${SuperPixelSegmentationUISrcs} ${SuperPixelSegmentationMOCSrcs})
TARGET_LINK_LIBRARIES(SuperPixelSegmentation ${ITK_LIBRARIES} umfpack ${QT_LIBRARIES} libSuperPixelSegmentation libCustomWidgets)
#set_target_properties(PoissonCloning PROPERTIES COMPILE_DEFINITIONS "${DEFINITIONS}")
INSTALL( TARGETS SuperPixelSegmentation RUNTIME DESTINATION ${INSTALL_DIR} )
