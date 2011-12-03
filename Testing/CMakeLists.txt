ENABLE_TESTING()

if(BuildVTK)
  ADD_EXECUTABLE(TestVTKSegmentation TestVTKSegmentation.cpp vtkSuperPixelSegmentation.cxx)
  TARGET_LINK_LIBRARIES(TestVTKSegmentation ${VTK_LIBRARIES} libSuperPixelSegmentation)
  ADD_TEST(TestVTKSegmentation TestVTKSegmentation ../data/beach.jpg output.jpg)
endif()

if(BuildITK)
  ADD_EXECUTABLE(TestITKSegmentation TestITKSegmentation.cpp itkSuperPixelSegmentation.cxx)
  TARGET_LINK_LIBRARIES(TestITKSegmentation ${VTK_LIBRARIES} libSuperPixelSegmentation)
  ADD_TEST(TestITKSegmentation TestITKSegmentation ../data/beach.jpg output.jpg)
endif()

