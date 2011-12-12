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
  
  typedef itk::GraphCutSegmentation<ImageType, LabelImageType> GraphCutSegmentationType;
  GraphCutSegmentationType::Pointer graphCutSegmentation = GraphCutSegmentationType::New();
  graphCutSegmentation->SetSigma(.5);
  graphCutSegmentation->SetK(500);
  graphCutSegmentation->SetMinSize(50);
  graphCutSegmentation->SetInput(reader->GetOutput());
  graphCutSegmentation->Update();
  
  typedef itk::ImageFileWriter<LabelImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(graphCutSegmentation->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
