#include <itkSLICSegmentation.h>

#include "Helpers.h"

#include <iostream>

template<typename TImage, typename TLabelImage>
void SLICSegmentationComputationObject<TImage, TLabelImage>::Compute()
{
  typedef itk::SLICSegmentation<TImage, TLabelImage> SLICSegmentationType;
  typename SLICSegmentationType::Pointer slicSegmentation = SLICSegmentationType::New();
  slicSegmentation->SetNumberOfSuperPixels(this->NumberOfSuperPixels);
  slicSegmentation->SetSpatialDistanceWeight(this->SpatialDistanceWeight);
  slicSegmentation->SetInput(this->Image);
  slicSegmentation->Update();

  Helpers::DeepCopy<TLabelImage>(slicSegmentation->GetOutput(), this->LabelImage);
}
