INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

add_library(libGraphCut disjoint-set.cxx imconv.cxx filter.cxx segment-graph.cxx segment-image.cxx)

ADD_EXECUTABLE(GraphCutSegmentationExample GraphCutSegmentationExample.cpp)
TARGET_LINK_LIBRARIES(GraphCutSegmentationExample ${ITK_LIBRARIES} libGraphCut)
