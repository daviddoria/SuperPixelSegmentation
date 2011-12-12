include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVL_DISABLE_SSE2")

add_library(libQuickShift generic.c host.c quickshift.c random.c)
#target_link_libraries(libQuickShift m pthread) # library 'm' (-lm) is for math.h functions (log, sin, etc).

add_executable(quickshiftExample quickshiftExample.cpp)
target_link_libraries(quickshiftExample m pthread libQuickShift ${ITK_LIBRARIES}) # library 'm' (-lm) is for math.h functions (log, sin, etc).
