#include "Helpers.h"

namespace Helpers
{
  
// This is a specialization that ensures that the number of pixels per component also matches.
template<>
void DeepCopy<itk::VectorImage<unsigned char, 2> >(const itk::VectorImage<unsigned char, 2>* input, itk::VectorImage<unsigned char, 2>* output)
{
  //std::cout << "DeepCopy<FloatVectorImageType>()" << std::endl;
  bool changed = false;
  if(input->GetNumberOfComponentsPerPixel() != output->GetNumberOfComponentsPerPixel())
    {
    output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
    //std::cout << "Set output NumberOfComponentsPerPixel to " << input->GetNumberOfComponentsPerPixel() << std::endl;
    changed = true;
    }
    
  if(input->GetLargestPossibleRegion() != output->GetLargestPossibleRegion())
    {
    output->SetRegions(input->GetLargestPossibleRegion());
    changed = true;
    }
  if(changed)
    {
    output->Allocate();
    }

  DeepCopyInRegion<itk::VectorImage<unsigned char, 2> >(input, input->GetLargestPossibleRegion(), output);
    
}

} // end namespace
