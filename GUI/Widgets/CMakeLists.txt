include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

QT4_WRAP_UI(CustomWidgetsUISrcs LabeledSlider.ui)
QT4_WRAP_CPP(CustomWidgetsMOCSrcs LabeledSlider.h FloatSlider.h)

add_library(libCustomWidgets FloatSlider.cpp LabeledSlider.cpp ${CustomWidgetsUISrcs} ${CustomWidgetsMOCSrcs})
