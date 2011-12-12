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

#ifndef HELPERS_H
#define HELPERS_H

// ITK
#include "itkVectorImage.h"

namespace Helpers
{

template <typename TImage, typename TLabelImage>
void ColorLabelsByAverageColor(const TImage* image, const TLabelImage* labelImage, TImage* output);

template <typename TImage>
typename TImage::PixelType MaxValue(const TImage* image);

template<typename TImage>
unsigned int CountPixelsWithValue(const TImage* image, typename TImage::PixelType value);

template<typename TImage>
void WriteRGBImage(const TImage* input, const std::string& filename);

template <class TImage>
void WriteScaledScalarImage(const TImage* image, std::string filename);

template<typename TImage>
void WriteImage(const TImage* image, const std::string& filename);

template<typename TImage>
void WriteRegion(const TImage* image, const itk::ImageRegion<2>& region, const std::string& filename);

template<typename TImage>
void DeepCopy(const TImage* input, TImage* output);

template<typename TImage>
void DeepCopyInRegion(const TImage* input, const itk::ImageRegion<2>& region, TImage* output);

template<>
void DeepCopy<itk::VectorImage<unsigned char, 2> >(const itk::VectorImage<unsigned char, 2>* input, itk::VectorImage<unsigned char, 2>* output);

template<typename TImage>
void RelabelSequential(typename TImage::Pointer input, typename TImage::Pointer output);

template<typename TVectorImage>
void BilateralFilterAllChannels(const TVectorImage* image, TVectorImage* output, const float domainSigma, const float rangeSigma);

} // end namespace

#include "Helpers.hxx"

#endif
