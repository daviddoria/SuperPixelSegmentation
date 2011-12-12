/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// ITK
#include "itkRegionOfInterestImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkComposeImageFilter.h"
#include "itkBilateralImageFilter.h"

// STL
#include <set>

namespace Helpers
{

template<typename TImage>
void DeepCopy(const TImage* input, TImage* output)
{
  DeepCopyInRegion<TImage>(input, input->GetLargestPossibleRegion(), output);
}

template<typename TImage>
void DeepCopyInRegion(const TImage* input, const itk::ImageRegion<2>& region, TImage* output)
{
  output->SetRegions(region);
  output->Allocate();

  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}


template <class T>
void WriteScaledScalarImage(const typename T::Pointer image, const std::string& filename)
{
  if(T::PixelType::Dimension > 1)
    {
    std::cerr << "Cannot write scaled scalar image with vector image input!" << std::endl;
    return;
    }
  typedef itk::Image<unsigned char, 2> UnsignedCharScalarImageType;
  typedef itk::RescaleIntensityImageFilter<T, UnsignedCharScalarImageType> RescaleFilterType; // expected ';' before rescaleFilter

  typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(image);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  typedef itk::ImageFileWriter<UnsignedCharScalarImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
}


template<typename TImage>
void WriteImage(const TImage* image, const std::string& filename)
{
  // This is a convenience function so that images can be written in 1 line instead of 4.
  typename itk::ImageFileWriter<TImage>::Pointer writer = itk::ImageFileWriter<TImage>::New();
  writer->SetFileName(filename);
  writer->SetInput(image);
  writer->Update();
}


template<typename TImage>
void WriteRGBImage(const TImage* input, const std::string& filename)
{
  typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2> RGBImageType;

  RGBImageType::Pointer output = RGBImageType::New();
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<RGBImageType> outputIterator(output, output->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    itk::CovariantVector<unsigned char, 3> pixel;
    for(unsigned int i = 0; i < 3; ++i)
      {
      pixel[i] = inputIterator.Get()[i];
      }
    outputIterator.Set(pixel);
    ++inputIterator;
    ++outputIterator;
    }

  typename itk::ImageFileWriter<RGBImageType>::Pointer writer = itk::ImageFileWriter<RGBImageType>::New();
  writer->SetFileName(filename);
  writer->SetInput(output);
  writer->Update();

}

template<typename TImage>
void WriteRegion(const typename TImage::Pointer image, const itk::ImageRegion<2>& region, const std::string& filename)
{
  //std::cout << "WriteRegion() " << filename << std::endl;
  //std::cout << "region " << region << std::endl;
  typedef itk::RegionOfInterestImageFilter<TImage, TImage> RegionOfInterestImageFilterType;

  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  //std::cout << "regionOfInterestImageFilter " << regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion() << std::endl;
  
  typename itk::ImageFileWriter<TImage>::Pointer writer = itk::ImageFileWriter<TImage>::New();
  writer->SetFileName(filename);
  writer->SetInput(regionOfInterestImageFilter->GetOutput());
  writer->Update();
}

template<typename TImage>
void RelabelSequential(typename TImage::Pointer input, typename TImage::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();

  // Keep only unique label ids
  std::set<typename TImage::PixelType> uniqueLabels;
  itk::ImageRegionConstIterator<TImage> imageIterator(input, input->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    uniqueLabels.insert(imageIterator.Get());
    ++imageIterator;
    }

  // Copy the set into a vector to induce an ordering
  std::vector<typename TImage::PixelType> sequentialLabels;
  for(typename std::set<typename TImage::PixelType>::iterator iterator = uniqueLabels.begin(); iterator != uniqueLabels.end(); iterator++)
    {
    sequentialLabels.push_back(*iterator);
    }

  std::map <typename TImage::PixelType, typename TImage::PixelType> labelMap;
  for(unsigned int i = 0; i < sequentialLabels.size(); ++i)
    {
    labelMap[sequentialLabels[i]] = i;
    }

  // Set old values to new sequential labels
  itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());

  while(!outputIterator.IsAtEnd())
    {
    // We check the input image because if we change pixels in the output image and then search it later, we could accidentially write incorrect values.
    outputIterator.Set(labelMap[input->GetPixel(outputIterator.GetIndex())]);

    ++outputIterator;
    }

}

template<typename TImage>
unsigned int CountPixelsWithValue(const TImage* image, typename TImage::PixelType value)
{
  itk::ImageRegionConstIterator<TImage> imageIterator(image, image->GetLargestPossibleRegion());

  unsigned int counter = 0;
  while(!imageIterator.IsAtEnd())
    {
    if(image->Get() == value)
      {
      counter++;
      }
    ++imageIterator;
    }
  return counter;
}

template <class TImage>
typename TImage::PixelType MaxValue(const TImage* image)
{
  typedef typename itk::MinimumMaximumImageCalculator<TImage>
          ImageCalculatorFilterType;

  typename ImageCalculatorFilterType::Pointer imageCalculatorFilter
          = ImageCalculatorFilterType::New ();
  imageCalculatorFilter->SetImage(image);
  imageCalculatorFilter->Compute();

  return imageCalculatorFilter->GetMaximum();
}


template<typename TVectorImage>
void BilateralAllChannels(const TVectorImage* image, TVectorImage* output, const float domainSigma, const float rangeSigma)
{
  typedef itk::Image<typename TVectorImage::InternalPixelType, 2> ScalarImageType;
  
  // Disassembler
  typedef itk::VectorIndexSelectionCastImageFilter<TVectorImage, ScalarImageType> IndexSelectionType;
  typename IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
  indexSelectionFilter->SetInput(image);
  
  // Reassembler
  typedef itk::ComposeImageFilter<ScalarImageType, TVectorImage> ImageToVectorImageFilterType;
  typename ImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ImageToVectorImageFilterType::New();
  
  std::vector< typename ScalarImageType::Pointer > filteredImages;
  
  for(unsigned int i = 0; i < image->GetNumberOfComponentsPerPixel(); ++i)
    {
    indexSelectionFilter->SetIndex(i);
    indexSelectionFilter->Update();
  
    typename ScalarImageType::Pointer imageChannel = ScalarImageType::New();
    DeepCopy<ScalarImageType>(indexSelectionFilter->GetOutput(), imageChannel);
  
    typedef itk::BilateralImageFilter<ScalarImageType, ScalarImageType>  BilateralFilterType;
    typename BilateralFilterType::Pointer bilateralFilter = BilateralFilterType::New();
    bilateralFilter->SetInput(imageChannel);
    bilateralFilter->SetDomainSigma(domainSigma);
    bilateralFilter->SetRangeSigma(rangeSigma);
    bilateralFilter->Update();
    
    typename ScalarImageType::Pointer blurred = ScalarImageType::New();
    DeepCopy<ScalarImageType>(bilateralFilter->GetOutput(), blurred);
    
    filteredImages.push_back(blurred);
    imageToVectorImageFilter->SetInput(i, filteredImages[i]);
    }

  imageToVectorImageFilter->Update();
 
  DeepCopy<TVectorImage>(imageToVectorImageFilter->GetOutput(), output);
}

template <typename TImage, typename TLabelImage>
void ColorLabelsByAverageColor(const TImage* image, const TLabelImage* labelImage, TImage* output)
{
  output->SetRegions(labelImage->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(image->GetNumberOfComponentsPerPixel());
  output->Allocate();

  // Determine how many labels there are
  unsigned int maxLabel = Helpers::MaxValue<TLabelImage>(labelImage);

//   typedef itk::Vector<float, 3> FloatPixelType; // This should really be based on the input image pixel dimension
//   FloatPixelType zeroFloatPixel;
//   //zeroFloatPixel.SetSize(3);
//   zeroFloatPixel.Fill(0);
  
  typedef itk::VariableLengthVector<float> FloatPixelType;
  FloatPixelType zeroFloatPixel;
  zeroFloatPixel.SetSize(image->GetNumberOfComponentsPerPixel());
  zeroFloatPixel.Fill(0);
  
  // We have to use float pixels or we would cause overflows while summing
  std::vector<FloatPixelType> segmentFloatColors(maxLabel + 1, zeroFloatPixel); // +1 because Labels start at 0
  std::vector<unsigned int> labelCount(maxLabel + 1, 0); // +1 because Labels start at 0
  
  //std::cout << "Coloring label " << labelId << std::endl;
  FloatPixelType floatColor;
  floatColor.SetSize(image->GetNumberOfComponentsPerPixel());
  floatColor.Fill(0);
  itk::ImageRegionConstIterator<TLabelImage> labelIterator(labelImage, labelImage->GetLargestPossibleRegion());
  
  while(!labelIterator.IsAtEnd())
    {
    labelCount[labelIterator.Get()]++;
    floatColor[0] = image->GetPixel(labelIterator.GetIndex())[0];
    floatColor[1] = image->GetPixel(labelIterator.GetIndex())[1];
    floatColor[2] = image->GetPixel(labelIterator.GetIndex())[2];
    
    segmentFloatColors[labelIterator.Get()] += floatColor;
    ++labelIterator;
    } // end while
    
  typename TImage::PixelType zeroPixel;
  zeroPixel.SetSize(image->GetNumberOfComponentsPerPixel());
  zeroPixel.Fill(0);
  std::vector<typename TImage::PixelType> segmentColors(maxLabel + 1, zeroPixel); // +1 because Labels start at 0
  
  typename TImage::PixelType colorPixel;
  colorPixel.SetSize(image->GetNumberOfComponentsPerPixel());
  colorPixel.Fill(0);
  
  for(unsigned int i = 0; i < segmentColors.size(); ++i)
    {
    colorPixel[0] = segmentFloatColors[i][0]/static_cast<float>(labelCount[i]);
    colorPixel[1] = segmentFloatColors[i][1]/static_cast<float>(labelCount[i]);
    colorPixel[2] = segmentFloatColors[i][2]/static_cast<float>(labelCount[i]);

    segmentColors[i] = colorPixel;
    }

  itk::ImageRegionConstIterator<TLabelImage> colorIterator(labelImage, labelImage->GetLargestPossibleRegion());
  
  while(!colorIterator.IsAtEnd())
    {
    output->SetPixel(colorIterator.GetIndex(), segmentColors[colorIterator.Get()]);

    ++colorIterator;
    } // end while
}

} // end namespace
