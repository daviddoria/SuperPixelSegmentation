ENABLE_TESTING()

ADD_EXECUTABLE(TestVTKSegmentation Test.cxx vtkSuperPixelSegmentation.cxx)
TARGET_LINK_LIBRARIES(TestVTKSegmentation ${VTK_LIBRARIES} libSuperPixelSegmentation)
ADD_TEST(TestVTKSegmentation TestVTKSegmentation ../data/beach.jpg output.jpg)

