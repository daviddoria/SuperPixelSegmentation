#ifndef __itkGraphCutSegmentation_h
#define __itkGraphCutSegmentation_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputImage, typename TOutputLabelImage>
class GraphCutSegmentation : public ImageToImageFilter<TInputImage, TOutputLabelImage>
{
public:
  /** Standard class typedefs. */
  typedef GraphCutSegmentation Self;
  typedef ImageToImageFilter<TInputImage, TOutputLabelImage> Superclass;
  typedef SmartPointer< Self >        Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GraphCutSegmentation, ImageToImageFilter);

  // Minimum component size (enforced by post-processing stage).
  itkSetMacro( MinSize, int );
  itkGetMacro( MinSize, int);

  // Constant for threshold function. Larger K causes a preferences for larger components.
  itkSetMacro( K, float );
  itkGetMacro( K, float);

  // Variance of smoothing kernel.
  itkSetMacro( Sigma, float );
  itkGetMacro( Sigma, float);
  
  // Blur the image before computing the super pixels.
  itkSetMacro( BlurFirst, bool);
  itkGetMacro( BlurFirst, bool);
  
  TOutputLabelImage* GetLabelImage();
  TInputImage* GetColoredImage();
  
  unsigned int FinalNumberOfSegments;
  
protected:
  GraphCutSegmentation();
  ~GraphCutSegmentation(){}

  /** Does the real work. */
  virtual void GenerateData();
  
  DataObject::Pointer MakeOutput(unsigned int idx);

private:
  GraphCutSegmentation(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

  int m_MinSize;
  float m_K;
  float m_Sigma;
  
  bool m_BlurFirst;
};
} //namespace ITK

#include "itkGraphCutSegmentation.hxx"

#endif
