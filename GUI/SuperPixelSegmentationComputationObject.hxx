#include "itkSuperPixelSegmentation.h"

#include "Helpers.h"

#include <iostream>

template<typename TImage, typename TLabelImage>
void SuperPixelSegmentationComputationObject<TImage, TLabelImage>::Compute()
{
  typedef itk::SuperPixelSegmentation<TImage, TLabelImage> SuperPixelSegmentationType;
  typename SuperPixelSegmentationType::Pointer superPixelSegmentation = SuperPixelSegmentationType::New();
  superPixelSegmentation->SetSigma(this->Sigma);
  superPixelSegmentation->SetK(this->K);
  superPixelSegmentation->SetMinSize(this->MinSize);
  superPixelSegmentation->SetInput(this->Image);
  superPixelSegmentation->Update();

  this->FinalNumberOfSegments = superPixelSegmentation->FinalNumberOfSegments;
  
  Helpers::DeepCopy<TLabelImage>(superPixelSegmentation->GetOutput(), this->LabelImage);
}
