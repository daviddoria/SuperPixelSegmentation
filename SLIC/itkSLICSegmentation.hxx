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
typename TInputImage::Pointer SLICSegmentation< TInputImage, TOutputLabelImage>
::GetContourImage()
{
  return this->ContourImage;
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

  this->Labels = new int[numberOfPixels];
  int numlabels(0);
  SLIC slic;
  
  slic.DoSuperpixelSegmentation_ForGivenK(image->GetBufferPointer(), width, height, this->Labels, numlabels, m_NumberOfSuperPixels, m_SpatialDistanceWeight);
  
  UnsignedIntImageType::Pointer labelImage = UnsignedIntImageType::New();
  labelImage->SetRegions(image->GetLargestPossibleRegion());
  labelImage->Allocate();
  
  itk::ImageRegionIterator<UnsignedIntImageType> labelIterator(labelImage, labelImage->GetLargestPossibleRegion());
 
  unsigned int labelId = 0;
  while(!labelIterator.IsAtEnd())
    {
    labelIterator.Set(this->Labels[labelId]);
 
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
  
  Helpers::DeepCopy<TInputImage>(input, this->ContourImage);
  
  DrawContoursAroundSegments(contourColor);
//   WriteImage<UnsignedIntImageType>(labelImage, "contours.mha");
  
}

template< typename TInputImage, typename TOutputLabelImage>
void SLICSegmentation< TInputImage, TOutputLabelImage>
::DrawContoursAroundSegments(const typename TInputImage::PixelType color)
{
        const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
        const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

        unsigned int width = this->ContourImage->GetLargestPossibleRegion().GetSize()[0];
        unsigned int height = this->ContourImage->GetLargestPossibleRegion().GetSize()[1];
        unsigned int sz = width*height;

        std::vector<bool> istaken(sz, false);

        int mainindex(0);
        for( int j = 0; j < height; j++ )
        {
                for( int k = 0; k < width; k++ )
                {
                        int np(0);
                        for( int i = 0; i < 8; i++ )
                        {
                                int x = k + dx8[i];
                                int y = j + dy8[i];

                                if( (x >= 0 && x < width) && (y >= 0 && y < height) )
                                {
                                        int index = y*width + x;

                                        if( false == istaken[index] )//comment this to obtain internal contours
                                        {
                                                if( this->Labels[mainindex] != this->Labels[index] ) np++;
                                        }
                                }
                        }
                        if( np > 1 )//change to 2 or 3 for thinner lines
                        {
                          itk::Index<2> itkIndex;
                          itkIndex[0] = k;
                          itkIndex[1] = j;

                          this->ContourImage->SetPixel(itkIndex, color);
                          istaken[mainindex] = true;
                        }
                        mainindex++;
                }
        }
}


}// end namespace

#endif
