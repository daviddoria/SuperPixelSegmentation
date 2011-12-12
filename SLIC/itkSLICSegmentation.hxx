#ifndef __itkSLICSegmentation_txx
#define __itkSLICSegmentation_txx

#include "itkSLICSegmentation.h"

// Custom
#include "Helpers.h"

// ITK
#include "itkBilateralImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"

// Segmentation
#include "SLIC.h"

namespace itk
{

template< typename TInputImage, typename TOutputLabelImage>
SLICSegmentation< TInputImage, TOutputLabelImage>
::SLICSegmentation() : m_NumberOfSuperPixels(200), m_SpatialDistanceWeight(5.0)
{

}

template< typename TInputImage, typename TOutputLabelImage>
void SLICSegmentation< TInputImage, TOutputLabelImage>
::GenerateData()
{
  typename TInputImage::ConstPointer filterInput = this->GetInput();
  
  typename TInputImage::Pointer input;

  typedef itk::Image<unsigned int, 2> UnsignedIntImageType;
  UnsignedIntImageType::Pointer image = UnsignedIntImageType::New();
  image->SetRegions(input->GetLargestPossibleRegion());
  image->Allocate();

  itk::ImageRegionIterator<TInputImage> imageIterator(input, input->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    int intPixel = PACK(1, imageIterator.Get()[0], imageIterator.Get()[1], imageIterator.Get()[2]);
    image->SetPixel(imageIterator.GetIndex(), intPixel);
 
    ++imageIterator;
    }

  //WriteImage<UnsignedIntImageType>(image, "input4channel.mha");
    
  SLIC mySLIC;
  
  unsigned int width = image->GetLargestPossibleRegion().GetSize()[0];
  unsigned int height = image->GetLargestPossibleRegion().GetSize()[1];

  int numberOfPixels = width*height;

  int* labels = new int[numberOfPixels];
  int numlabels(0);
  SLIC slic;
  
  slic.DoSuperpixelSegmentation_ForGivenK(image->GetBufferPointer(), width, height, labels, numlabels, m_NumberOfSuperPixels, m_SpatialDistanceWeight);
  
  UnsignedIntImageType::Pointer labelImage = UnsignedIntImageType::New();
  labelImage->SetRegions(image->GetLargestPossibleRegion());
  labelImage->Allocate();
  
  itk::ImageRegionIterator<UnsignedIntImageType> labelIterator(labelImage, labelImage->GetLargestPossibleRegion());
 
  unsigned int labelId = 0;
  while(!labelIterator.IsAtEnd())
    {
    labelIterator.Set(labels[labelId]);
 
    ++labelIterator;
    labelId++;
    }
    
  //WriteImage<UnsignedIntImageType>(labelImage, "output.mha");
  
  typename TInputImage::PixelType contourColor;
  contourColor[0] = 255;
  contourColor[1] = 255;
  contourColor[2] = 0;
  //DrawContoursAroundSegments<RGBImageType>(reader->GetOutput(), labels, width, height, contourColor);
  //WriteImage<RGBImageType>(reader->GetOutput(), "ImageWithContours.png");
  
//   DrawContoursAroundSegments(labelImage, labels, width, height);
//   WriteImage<UnsignedIntImageType>(labelImage, "contours.mha");
  
}

}// end namespace

#endif
