#ifndef __itkQuickShiftSegmentation_txx
#define __itkQuickShiftSegmentation_txx

#include "itkQuickShiftSegmentation.h"

// Custom
#include "Helpers.h"

// ITK
#include "itkBilateralImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"

// Segmentation
#include "quickshift.h"

namespace itk
{

template< typename TInputImage, typename TOutputLabelImage>
QuickShiftSegmentation< TInputImage, TOutputLabelImage>
::QuickShiftSegmentation() : m_KernelSize(5), m_MaxDist(10.0)
{
  this->SetNumberOfRequiredOutputs(2);

  this->SetNthOutput( 0, this->MakeOutput(0) );
  this->SetNthOutput( 1, this->MakeOutput(1) );
}

template< typename TInputImage, typename TOutputLabelImage>
DataObject::Pointer QuickShiftSegmentation<TInputImage, TOutputLabelImage>::MakeOutput(unsigned int idx)
{
  DataObject::Pointer output;

  switch ( idx )
    {
    case 0:
      output = ( TOutputLabelImage::New() ).GetPointer(); // The output for the label image
      break;
    case 1:
      output = ( TInputImage::New() ).GetPointer(); // The output for the image colored by the average color in each region
      break;
    default:
      std::cerr << "No output " << idx << std::endl;
      output = NULL;
      break;
    }
  return output.GetPointer();
}

template< typename TInputImage, typename TOutputLabelImage>
TOutputLabelImage* QuickShiftSegmentation<TInputImage, TOutputLabelImage>::GetLabelImage()
{
  return dynamic_cast< TOutputLabelImage * >(this->ProcessObject::GetOutput(0) );
}

template< typename TInputImage, typename TOutputLabelImage>
TInputImage* QuickShiftSegmentation<TInputImage, TOutputLabelImage>::GetColoredImage()
{
  return dynamic_cast< TInputImage * >(this->ProcessObject::GetOutput(1) );
}

template< typename TInputImage, typename TOutputLabelImage>
void QuickShiftSegmentation< TInputImage, TOutputLabelImage>
::GenerateData()
{
  TInputImage* input = const_cast<TInputImage*>(this->GetInput());

  unsigned int width = input->GetLargestPossibleRegion().GetSize()[0];
  unsigned int height = input->GetLargestPossibleRegion().GetSize()[1];
  unsigned int channels = input->GetNumberOfComponentsPerPixel();
  
  unsigned int totalPixels = width*height;
  
  vl_qs_type* image = new vl_qs_type[totalPixels*channels];
  
  vl_qs_type ratio = 10.0f;
  for(unsigned int channel = 0; channel < channels; ++channel)
    {
    for (unsigned int y = 0 ; y < height ; ++y)
      {
      for (unsigned int x = 0  ; x < width ; ++x) 
        {
        unsigned int linearIndex = ComputeLinearValueIndex(y, x, width, height, channel);
        std::cout << "linearIndex: " << linearIndex << std::endl;
        float noise = drand48()/10.0f;
        if(x <= 1)
          {
          //image[linearIndex] = 255;
          image[linearIndex] = 1.0 + noise;
          image[linearIndex] *= ratio;
          }
        else
          {
          image[linearIndex] = 0 + noise;
          }
        }
      }
    }

  // Create a new quick shift object
  VlQS* quickshift = vl_quickshift_new(image, height, width, channels);

  // Configure quick shift by setting the kernel size (vl_quickshift_set_kernel_size)
  // and the maximum gap (vl_quickshift_set_max_dist).
  // The latter is in principle not necessary, but useful to speedup processing.

  vl_quickshift_set_kernel_size(quickshift, this->m_KernelSize);

  vl_quickshift_set_max_dist(quickshift, this->m_MaxDist);

  vl_quickshift_process(quickshift);

  // Retrieve the parents (These can be used to segment the image in superpixels.)
  int* parents = vl_quickshift_get_parents(quickshift);

  std::vector<int> parentsVector = GetVectorFromArray(parents, totalPixels);

  std::vector<int> labels = GetLabelsFromParents(parentsVector);
  
  std::cout << "Labels vector:" << std::endl;
  
  std::vector<int> sequentialLabels = SequentialLabels(labels);
  
  //Retrieve the distances (These can be used to segment the image in superpixels.)
  vl_qs_type* distances = vl_quickshift_get_dists(quickshift);
  // Pixels that are roots have distance=inf
  
  typename TOutputLabelImage::Pointer outputLabelImage = this->GetLabelImage(); // One of the output ports
  outputLabelImage->SetRegions(input->GetLargestPossibleRegion());
  outputLabelImage->Allocate();
  
  itk::ImageRegionIterator<TOutputLabelImage> labelIterator(outputLabelImage, outputLabelImage->GetLargestPossibleRegion());
 
  unsigned int labelId = 0;
  while(!labelIterator.IsAtEnd())
    {
    unsigned int linearIndex = ComputeLinearPixelIndex(labelIterator.GetIndex()[1], labelIterator.GetIndex()[0], height);
    labelIterator.Set(sequentialLabels[linearIndex]);
 
    ++labelIterator;
    labelId++;
    }

  // Delete the quick shift object
  vl_quickshift_delete(quickshift);
  
  Helpers::RelabelSequential<TOutputLabelImage>(outputLabelImage, outputLabelImage); // This is the 0th output port of the filter
  
  Helpers::WriteImage<TOutputLabelImage>(outputLabelImage, "SLIC_LabelImage.mha");
  
  Helpers::ColorLabelsByAverageColor<TInputImage, TOutputLabelImage>(input, this->GetLabelImage(), this->GetColoredImage());
  Helpers::WriteImage<TInputImage>(this->GetColoredImage(), "SLIC_ColoredImage.mha");
}

template< typename TInputImage, typename TOutputLabelImage>
template<typename T>
std::vector<T> QuickShiftSegmentation< TInputImage, TOutputLabelImage>
::GetVectorFromArray(const T* array, const unsigned int size)
{
  std::vector<T> myVector(size);
  for(unsigned int i = 0; i < size; ++i)
    {
    myVector[i] = array[i];
    }
  return myVector;
}

template< typename TInputImage, typename TOutputLabelImage>
std::vector<int> QuickShiftSegmentation< TInputImage, TOutputLabelImage>
::GetLabelsFromParents(const std::vector<int>& parents)
{
  // The values of the 'parents' array indicate the linear index of the pixel that is the parent of each pixel.
  // Compare to the LABELS output of [LABELS CLUSTERS] = VL_FLATMAP(MAP) in Matlab.

  std::vector<int> labels(parents.size(), 0);

  // Initialize by copying
  for(unsigned int i = 0; i < parents.size(); ++i)
    {
    labels[i] = parents[i];
    }
  
  std::vector<int> tempLabels(parents.size(), 0);
  std::vector<int> oldLabels(parents.size(), 0);
  
  while(!VectorsIdential(oldLabels, labels))
    {
    oldLabels = labels;
    for(unsigned int i = 0; i < parents.size(); ++i)
      {
      //std::cout << "Replacing " << labels[i] << " with " << labels[labels[i]] << std::endl;
      tempLabels[i] = labels[labels[i]]; // Replace my label with my parents label.
      }
    labels = tempLabels;
    }

  return labels;
}

template< typename TInputImage, typename TOutputLabelImage>
bool QuickShiftSegmentation< TInputImage, TOutputLabelImage>
::VectorsIdential(const std::vector<int>& v1, const std::vector<int>& v2)
{
  if(v1.size() != v2.size())
    {
    std::cerr << "Vectors must be the same size!" << std::endl;
    exit(-1);
    }
  for(unsigned int i = 0; i < v1.size(); ++i)
    {
    if(v1[i] != v2[i])
      {
      return false;
      }
    }
  return true;
}

template< typename TInputImage, typename TOutputLabelImage>
std::vector<int> QuickShiftSegmentation< TInputImage, TOutputLabelImage>
::SequentialLabels(const std::vector<int>& v)
{
  std::vector<int> sequentialLabels(v.size());
  std::set<int> uniqueLabels;
  for(unsigned int i = 0; i < v.size(); ++i)
    {
    uniqueLabels.insert(v[i]);
    }

  // Set old values to new sequential labels
  unsigned int sequentialLabelId = 0;
  for(typename std::set<int>::iterator it1 = uniqueLabels.begin(); it1 != uniqueLabels.end(); it1++)
    {
    
    for(unsigned int i = 0; i < v.size(); ++i)
      {
      // We check the input image because if we change pixels in the output image and then search it later, we could accidentially write incorrect values.
      if(v[i] == *it1)
        {
        sequentialLabels[i] = sequentialLabelId;
        }
      }
    sequentialLabelId++;
    }
  return sequentialLabels;
}


}// end namespace

#endif
