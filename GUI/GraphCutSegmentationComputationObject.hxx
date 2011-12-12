#include <itkGraphCutSegmentation.h>

#include "Helpers.h"

#include <iostream>

template<typename TImage, typename TLabelImage>
void GraphCutSegmentationComputationObject<TImage, TLabelImage>::Compute()
{
  typedef itk::GraphCutSegmentation<TImage, TLabelImage> GraphCutSegmentationType;
  typename GraphCutSegmentationType::Pointer graphCutSegmentation = GraphCutSegmentationType::New();
  graphCutSegmentation->SetSigma(this->Sigma);
  graphCutSegmentation->SetK(this->K);
  graphCutSegmentation->SetMinSize(this->MinSize);
  graphCutSegmentation->SetInput(this->Image);
  graphCutSegmentation->Update();

  this->FinalNumberOfSegments = graphCutSegmentation->FinalNumberOfSegments;
  
  Helpers::DeepCopy<TLabelImage>(graphCutSegmentation->GetOutput(), this->LabelImage);
}
