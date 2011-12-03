#include "vtkSuperPixelSegmentation.h"
#include "segment-image.h"
#include "image.h"
#include "misc.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"

vtkStandardNewMacro(vtkSuperPixelSegmentation);

vtkSuperPixelSegmentation::vtkSuperPixelSegmentation()
{
  this->Sigma = 0.5;
  this->K = 500;
  this->MinSize = 20;
	
}

int vtkSuperPixelSegmentation::RequestData(vtkInformation *vtkNotUsed(request),
                                             vtkInformationVector **inputVector,
                                             vtkInformationVector *outputVector)
{
  
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
    
  // get the input and ouptut
  vtkImageData* input = vtkImageData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  vtkImageData* output = vtkImageData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));
    
  //convert input to the image class for this algorithm
  int* dims = input->GetDimensions();
  int cols = dims[0];
  int rows = dims[1];
  
  cout << "rows: " << rows << endl;
  cout << "cols: " << cols << endl;
  
  image<rgb> *im = new image<rgb>(dims[0], dims[1]);
  
  for(unsigned int r = 0; r < rows; r++)
    {
    for(unsigned int c = 0; c < cols; c++)
      {
      unsigned char* color = static_cast<unsigned char*>(input->GetScalarPointer(c,r,0));
      rgb rgbColor;
      rgbColor.r = color[0];
      rgbColor.g = color[1];
      rgbColor.b = color[2];
      im->access[r][c] = rgbColor;
      }
    }
  
  int numberOfSegments; 
  image<rgb> *seg = segment_image(im, this->Sigma, this->K, this->MinSize, &numberOfSegments);
  
  cout << "There were " << numberOfSegments << " segments." << endl;
   
  //convert to the image class for this algorithm to a normal output type
  output->SetDimensions(dims[0], dims[1], 1);
  output->SetNumberOfScalarComponents(3);

  for(unsigned int r = 0; r < rows; r++)
    {
    for(unsigned int c = 0; c < cols; c++)
      {
      unsigned char* color = static_cast<unsigned char*>(output->GetScalarPointer(c,r,0));
      rgb rgbColor = seg->access[r][c];
      
      color[0] = rgbColor.r;
      color[1] = rgbColor.g;
      color[2] = rgbColor.b;
      
      }
    }
  
  return 1;
}

