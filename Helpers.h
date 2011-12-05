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

#ifndef HELPERS_OUTPUT_H
#define HELPERS_OUTPUT_H

namespace Helpers
{

template <class TImage>
float MaxValue(const typename TImage::Pointer image);

template<typename TImage>
unsigned int CountPixelsWithValue(const typename TImage::Pointer image, typename TImage::PixelType value);

template<typename T>
void WriteRGBImage(const typename T::Pointer input, const std::string& filename);

template <class T>
void WriteScaledScalarImage(const typename T::Pointer image, std::string filename);

template<typename T>
void WriteImage(const typename T::Pointer image, const std::string& filename);

template<typename TImage>
void WriteRegion(const typename TImage::Pointer image, const itk::ImageRegion<2>& region, const std::string& filename);

template<typename TImage>
void DeepCopy(typename TImage::Pointer input, typename TImage::Pointer output);

template<typename TImage>
void RelabelSequential(typename TImage::Pointer input, typename TImage::Pointer output);

} // end namespace

#include "Helpers.hxx"

#endif
