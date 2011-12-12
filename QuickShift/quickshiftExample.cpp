#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"

#include "itkQuickShiftSegmentation.h"

typedef itk::VectorImage<unsigned char, 2> ImageType;
typedef itk::Image<unsigned int, 2> LabelImageType;

template<typename TImage>
static void CreateImage(TImage* image);

template<typename TImage>
static void WriteImage(TImage* image, const std::string& filename);

int main()
{
  ImageType::Pointer image = ImageType::New();
  CreateImage<ImageType>(image);

  typedef itk::QuickShiftSegmentation<ImageType, LabelImageType> QuickShiftSegmentationType;
  QuickShiftSegmentationType::Pointer quickShiftSegmentation = QuickShiftSegmentationType::New();

  return 0;
}


template<typename TImage>
void CreateImage(TImage* image)
{
  itk::Index<2> start;
  start.Fill(0);
 
  itk::Size<2> size;
  size.Fill(5);
 
  itk::ImageRegion<2> region(start,size);
 
  image->SetRegions(region);
  image->Allocate();
 
  itk::ImageRegionIterator<TImage> imageIterator(image, region);
 
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] < size[0]/2)
      {
      imageIterator.Set(255);
      }
    else
      {
      imageIterator.Set(0);
      }
 
    ++imageIterator;
    }
}

template<typename TImage>
void WriteImage(TImage* image, const std::string& filename)
{
  typedef  itk::ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(image);
  writer->Update();  
}
