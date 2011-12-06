#ifndef __itkImageFilter_txx
#define __itkImageFilter_txx

#include "itkSuperPixelSegmentation.h"

// Custom
#include "Helpers.h"

// ITK
#include "itkBilateralImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"

// Segmentation
#include "image.h" // The image class for the segmentation algorithm
#include "misc.h" // Defines the 'rgb' type.
#include "segment-image.h"

namespace itk
{

template< typename TInputImage, typename TOutputLabelImage>
SuperPixelSegmentation< TInputImage, TOutputLabelImage>
::SuperPixelSegmentation() : m_MinSize(20), m_K(500), m_Sigma(2.0)
{

}

template< typename TInputImage, typename TOutputLabelImage>
void SuperPixelSegmentation< TInputImage, TOutputLabelImage>
::GenerateData()
{
  typename TInputImage::ConstPointer filterInput = this->GetInput();
  
  typename TInputImage::Pointer input;
  if(!this->m_BlurFirst)
    {
    input = const_cast<TInputImage*>(filterInput.GetPointer());
    }
  else
    {
    // Optionally blur
//     typedef itk::BilateralImageFilter<TInputImage, TInputImage>  BilateralImageFilterType;
//     BilateralImageFilterType::Pointer bilateralImageFilter = BilateralImageFilterType::New();
//     bilateralImageFilter->SetInput(filterInput);
//     bilateralImageFilter->SetDomainSigma(domainSigma);
//     bilateralImageFilter->SetRangeSigma(rangeSigma);
//     bilateralImageFilter->Update();
//    input = bilateralImageFilter->GetOutput();
    float domainSigma = 3.0f;
    float rangeSigma = 10.0f;
    //Helpers::BilateralAllChannels<TInputImage>(filterInput, input, domainSigma, rangeSigma);
    Helpers::BilateralAllChannels<TInputImage>(const_cast<TInputImage*>(filterInput.GetPointer()), input, domainSigma, rangeSigma);

    }

  Helpers::WriteImage<TInputImage>(input, "finalInput.mha");
  
  typename TOutputLabelImage::Pointer outputLabelImage = this->GetOutput();
  outputLabelImage->SetRegions(input->GetLargestPossibleRegion());
  outputLabelImage->Allocate();

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
  image<int> *segmentImage = segment_image(im, this->m_K, this->m_MinSize, &numberOfSegments);

  std::cout << "There were " << numberOfSegments << " segments." << std::endl;
  this->FinalNumberOfSegments = numberOfSegments;
  
  itk::ImageRegionIterator<TOutputLabelImage> outputIterator(outputLabelImage, outputLabelImage->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    itk::Index<2> index = outputIterator.GetIndex();

    int segmentId = segmentImage->access[index[1]][index[0]];
    outputIterator.Set(segmentId);
    ++outputIterator;
    }

  Helpers::RelabelSequential<TOutputLabelImage>(outputLabelImage, outputLabelImage);
    
}

}// end namespace


#endif
