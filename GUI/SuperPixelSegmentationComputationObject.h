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

#ifndef SUPERPIXELSEGMENTATIONCOMPUTATIONOBJECT_H
#define SUPERPIXELSEGMENTATIONCOMPUTATIONOBJECT_H

#include "ComputationObject.h"

template<typename TImage, typename TLabelImage>
class SuperPixelSegmentationComputationObject : public ComputationObject
{
public:
  void Compute();
  TImage* Image;
  TLabelImage* LabelImage;

  float Sigma;
  float K;
  unsigned int MinSize;
};

#include "SuperPixelSegmentationComputationObject.hxx"

#endif
