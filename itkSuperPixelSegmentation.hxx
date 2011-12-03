#ifndef __itkImageFilter_txx
#define __itkImageFilter_txx

// Custom
#include "Helpers.h"

// ITK
#include "itkSuperPixelSegmentation.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

// Segmentation
#include "image.h" // The image class for the segmentation algorithm
#include "misc.h" // Defines the 'rgb' type.
#include "segment-image.h"

namespace itk
{

template< typename TInputImage, typename TOutputImage>
void SuperPixelSegmentation< TInputImage, TOutputImage>
::GenerateData()
{
  typename TInputImage::ConstPointer input = this->GetInput();
  typename TOutputImage::Pointer output = this->GetOutput();
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();

  itk::Size<2> size = input->GetLargestPossibleRegion().GetSize();
  //std::cout << "Size: " << size << std::endl;
  unsigned int width = size[0];
  unsigned int height = size[1];
  image<rgb> *im = new image<rgb>(width, height);

  itk::ImageRegionConstIterator<TInputImage> imageIterator(input, input->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    itk::Index<2> index = imageIterator.GetIndex();
  
    typename TInputImage::PixelType color = imageIterator.Get();
    rgb rgbColor;
    rgbColor.r = color[0];
    rgbColor.g = color[1];
    rgbColor.b = color[2];
    im->access[index[1]][index[0]] = rgbColor; // [r][c]

    ++imageIterator;
    }

  int numberOfSegments;
  image<int> *segmentImage = segment_image(im, this->m_Sigma, this->m_K, this->m_MinSize, &numberOfSegments);

  std::cout << "There were " << numberOfSegments << " segments." << std::endl;

  itk::ImageRegionIterator<TOutputImage> outputIterator(output, output->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    itk::Index<2> index = outputIterator.GetIndex();

    int segmentId = segmentImage->access[index[1]][index[0]];
    outputIterator.Set(segmentId);
    ++outputIterator;
    }

  Helpers::RelabelSequential<TOutputImage>(output, output);
    
}

}// end namespace


#endif
