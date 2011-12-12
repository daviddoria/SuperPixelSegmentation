#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include "itkGraphCutSegmentation.h"

//typedef itk::Image<itk::CovariantVector<float, 3>, 2> ImageType;
typedef itk::VectorImage<float, 2> ImageType;
typedef itk::Image<int, 2> LabelImageType;

int main(int argc, char* argv[])
{
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  typedef itk::SuperPixelSegmentation<ImageType, LabelImageType> SuperPixelSegmentationType;
  SuperPixelSegmentationType::Pointer superPixelSegmentation = SuperPixelSegmentationType::New();
  superPixelSegmentation->SetSigma(.5);
  superPixelSegmentation->SetK(500);
  superPixelSegmentation->SetMinSize(50);
  superPixelSegmentation->SetInput(reader->GetOutput());
  superPixelSegmentation->Update();
  
  typedef itk::ImageFileWriter<LabelImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(superPixelSegmentation->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
