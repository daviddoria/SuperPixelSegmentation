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
#include "itkVectorImage.h"
#include "itkVectorMagnitudeImageFilter.h"

// Qt
#include <QColor>

namespace HelpersQt
{

template <typename TImage>
QImage GetQImageRGB(const typename TImage::Pointer image)
{
  return GetQImageRGB<TImage>(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageRGB(const typename TImage::Pointer image, const itk::ImageRegion<2>& region)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage > RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();
  
  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(), regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());
  
  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();
    int r = static_cast<int>(pixel[0]);
    int g = static_cast<int>(pixel[1]);
    int b = static_cast<int>(pixel[2]);
    QColor pixelColor(r,g,b);
    if(r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0)
      {
      std::cout << "Can't set r,g,b to " << r << " " << g << " " << b << std::endl;
      }
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}


template <typename TImage>
QImage GetQImageRGBA(const typename TImage::Pointer image)
{
  return GetQImageRGBA<TImage>(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageRGBA(const typename TImage::Pointer image, const itk::ImageRegion<2>& region)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_ARGB32);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage > RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();
  
  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(), regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());
  
  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();
    int r = static_cast<int>(pixel[0]);
    int g = static_cast<int>(pixel[1]);
    int b = static_cast<int>(pixel[2]);
    QColor pixelColor(r,g,b,255); // opaque
    if(r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0)
      {
      std::cout << "Can't set r,g,b to " << r << " " << g << " " << b << std::endl;
      }
    qimage.setPixel(index[0], index[1], pixelColor.rgba());

    ++imageIterator;
    }
  
  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

template <typename TImage>
QImage GetQImageMagnitude(const typename TImage::Pointer image)
{
  return GetQImageMagnitude<TImage>(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageMagnitude(const typename TImage::Pointer image, const itk::ImageRegion<2>& region)
{
  typedef itk::Image<typename TImage::InternalPixelType> ScalarImageType;
  
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::VectorMagnitudeImageFilter<TImage, ScalarImageType>  VectorMagnitudeFilterType;
  typename VectorMagnitudeFilterType::Pointer magnitudeFilter = VectorMagnitudeFilterType::New();
  magnitudeFilter->SetInput(image);
  magnitudeFilter->Update();

  typedef itk::VectorImage<unsigned char, 2> UnsignedCharVectorImageType;
  typedef itk::Image<unsigned char, 2> UnsignedCharScalarImageType;
  typedef itk::RescaleIntensityImageFilter<ScalarImageType, UnsignedCharScalarImageType> RescaleFilterType;
  typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->SetInput( magnitudeFilter->GetOutput() );
  rescaleFilter->Update();
  
  typedef itk::RegionOfInterestImageFilter< UnsignedCharScalarImageType, UnsignedCharScalarImageType> RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(rescaleFilter->GetOutput());
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(), regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    unsigned char pixelValue = imageIterator.Get();

    QColor pixelColor(static_cast<int>(pixelValue), static_cast<int>(pixelValue), static_cast<int>(pixelValue));

    itk::Index<2> index = imageIterator.GetIndex();
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}

template <typename TImage>
QImage GetQImageScalar(const typename TImage::Pointer image)
{
  return GetQImageScalar<TImage>(image, image->GetLargestPossibleRegion());
}

template <typename TImage>
QImage GetQImageScalar(const typename TImage::Pointer image, const itk::ImageRegion<2>& region)
{
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  typedef itk::RegionOfInterestImageFilter< TImage, TImage> RegionOfInterestImageFilterType;
  typename RegionOfInterestImageFilterType::Pointer regionOfInterestImageFilter = RegionOfInterestImageFilterType::New();
  regionOfInterestImageFilter->SetRegionOfInterest(region);
  regionOfInterestImageFilter->SetInput(image);
  regionOfInterestImageFilter->Update();

  itk::ImageRegionIterator<TImage> imageIterator(regionOfInterestImageFilter->GetOutput(), regionOfInterestImageFilter->GetOutput()->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixelValue = imageIterator.Get();

    QColor pixelColor(static_cast<int>(pixelValue), static_cast<int>(pixelValue), static_cast<int>(pixelValue));

    itk::Index<2> index = imageIterator.GetIndex();
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  //return qimage; // The actual image region
  return qimage.mirrored(false, true); // The flipped image region
}


} // end namespace
