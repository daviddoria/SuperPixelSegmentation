#ifndef __itkSLICSegmentation_h
#define __itkSLICSegmentation_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputImage, typename TOutputLabelImage>
class SLICSegmentation : public ImageToImageFilter<TInputImage, TOutputLabelImage>
{
public:
  /** Standard class typedefs. */
  typedef SLICSegmentation Self;
  typedef ImageToImageFilter<TInputImage, TOutputLabelImage> Superclass;
  typedef SmartPointer< Self >        Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SLICSegmentation, ImageToImageFilter);

  // The approximate number of superpixels in the result.
  itkSetMacro( NumberOfSuperPixels, int );
  itkGetMacro( NumberOfSuperPixels, int);

  // Spatial distance weight.
  itkSetMacro( SpatialDistanceWeight, float );
  itkGetMacro( SpatialDistanceWeight, float);

  TOutputLabelImage* GetLabelImage();
  TInputImage* GetContourImage();
  TInputImage* GetColoredImage();
  
protected:
  SLICSegmentation();
  ~SLICSegmentation(){}

  /** Does the real work. */
  virtual void GenerateData();

  DataObject::Pointer MakeOutput(unsigned int idx);

  void DrawContoursAroundSegments(const typename TInputImage::PixelType color);

private:
  // This function takes 4 unsigned chars and inserts them into an int (4x8bit = 32bit)
  // For an RGB pixel, use:
  // int intPixel = PACK(1, R, G, B);
  inline int PACK(const char c0, const char c1, const char c2, const char c3) 
  {
    return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
  }

  SLICSegmentation(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

  int m_NumberOfSuperPixels;
  float m_SpatialDistanceWeight;
  
  int* Labels;
};
} //namespace ITK

#include "itkSLICSegmentation.hxx"

#endif
