#include "itkSuperPixelSegmentation.h"

#include "Helpers.h"

#include <iostream>

template<typename TImage, typename TLabelImage>
void SuperPixelSegmentationComputationObject<TImage, TLabelImage>::Compute()
{
  typedef itk::SuperPixelSegmentation<TImage, TLabelImage> SuperPixelSegmentationType;
  typename SuperPixelSegmentationType::Pointer superPixelSegmentation = SuperPixelSegmentationType::New();
  superPixelSegmentation->SetSigma(.5);
  superPixelSegmentation->SetK(500);
  superPixelSegmentation->SetMinSize(50);
  superPixelSegmentation->SetInput(this->Image);
  superPixelSegmentation->Update();

  Helpers::DeepCopy<TLabelImage>(superPixelSegmentation->GetOutput(), this->LabelImage);
}
