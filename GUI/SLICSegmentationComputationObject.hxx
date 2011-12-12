#include <itkSLICSegmentation.h>

#include "Helpers.h"

#include <iostream>

template<typename TImage, typename TLabelImage>
void SLICSegmentationComputationObject<TImage, TLabelImage>::Compute()
{
  typedef itk::SLICSegmentation<TImage, TLabelImage> SLICSegmentationType;
  typename SLICSegmentationType::Pointer slicSegmentation = SLICSegmentationType::New();
  slicSegmentation->SetSigma(this->Sigma);
  slicSegmentation->SetK(this->K);
  slicSegmentation->SetMinSize(this->MinSize);
  slicSegmentation->SetInput(this->Image);
  slicSegmentation->Update();

  this->FinalNumberOfSegments = slicSegmentation->FinalNumberOfSegments;
  
  Helpers::DeepCopy<TLabelImage>(slicSegmentation->GetOutput(), this->LabelImage);
}
