#ifndef __itkQuickShiftSegmentation_h
#define __itkQuickShiftSegmentation_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputImage, typename TOutputLabelImage>
class QuickShiftSegmentation : public ImageToImageFilter<TInputImage, TOutputLabelImage>
{
public:
  /** Standard class typedefs. */
  typedef QuickShiftSegmentation Self;
  typedef ImageToImageFilter<TInputImage, TOutputLabelImage> Superclass;
  typedef SmartPointer< Self >        Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QuickShiftSegmentation, ImageToImageFilter);

  // The approximate number of superpixels in the result.
  itkSetMacro( KernelSize, float);
  itkGetMacro( KernelSize, float);

  // Maximum distance between two pixels in the same label.
  itkSetMacro( MaxDist, float );
  itkGetMacro( MaxDist, float);
  
  // Tradeoff between color and distance. Larger values give more importance to color.
  itkSetMacro( Ratio, float );
  itkGetMacro( Ratio, float);

  TOutputLabelImage* GetLabelImage();
  TInputImage* GetColoredImage();
  
protected:
  QuickShiftSegmentation();
  ~QuickShiftSegmentation(){}

  /** Does the real work. */
  virtual void GenerateData();

  DataObject::Pointer MakeOutput(unsigned int idx);

private:

  template<typename T>
  std::vector<T> GetVectorFromArray(const T* array, const unsigned int size);
  
  std::vector<int> GetLabelsFromParents(const std::vector<int>& parents);
  bool VectorsIdential(const std::vector<int>& v1, const std::vector<int>& v2);
  std::vector<int> SequentialLabels(const std::vector<int>& v);
  
  unsigned int ComputeLinearValueIndex(const unsigned int row, const unsigned int col, const unsigned int width, const unsigned int height, const unsigned int channel)
  {
    return channel*width*height + row + height * col;
  }
  
  unsigned int ComputeLinearPixelIndex(const unsigned int row, const unsigned int col, const unsigned int height)
  {
    return row + height * col;
  }

  QuickShiftSegmentation(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

  float m_KernelSize;
  float m_MaxDist;
  float m_Ratio;
};
} //namespace ITK

#include "itkQuickShiftSegmentation.hxx"

#endif
