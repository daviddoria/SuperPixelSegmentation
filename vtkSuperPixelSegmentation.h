#ifndef __vtkSuperPixelSegmentation_h
#define __vtkSuperPixelSegmentation_h

#include "vtkImageAlgorithm.h"

class vtkSuperPixelSegmentation : public vtkImageAlgorithm 
{
public:
  vtkTypeMacro(vtkSuperPixelSegmentation,vtkImageAlgorithm);
  static vtkSuperPixelSegmentation *New();
	  
  vtkSetMacro(Sigma, float);
  vtkGetMacro(Sigma, float);
  
  vtkSetMacro(K, float);
  vtkGetMacro(K, float);
  
  vtkSetMacro(MinSize, int);
  vtkGetMacro(MinSize, int);
  
protected:
  vtkSuperPixelSegmentation();
  
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkSuperPixelSegmentation(const vtkSuperPixelSegmentation&);  // Not implemented.
  void operator=(const vtkSuperPixelSegmentation&);  // Not implemented.

  float Sigma;
  float K;
  int MinSize;
	
};

#endif
