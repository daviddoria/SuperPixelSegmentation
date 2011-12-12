#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include "itkSLICSegmentation.h"

//typedef itk::Image<itk::CovariantVector<float, 3>, 2> ImageType;
typedef itk::VectorImage<float, 2> ImageType;
typedef itk::Image<int, 2> LabelImageType;

int main(int argc, char* argv[])
{
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  typedef itk::SLICSegmentation<ImageType, LabelImageType> SLICSegmentationType;
  SLICSegmentationType::Pointer slicSegmentation = SLICSegmentationType::New();
  slicSegmentation->SetNumberOfSuperPixels(200);
  slicSegmentation->SetSpatialDistanceWeight(5.0);
  slicSegmentation->SetInput(reader->GetOutput());
  slicSegmentation->Update();
  
  typedef itk::ImageFileWriter<LabelImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(slicSegmentation->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
