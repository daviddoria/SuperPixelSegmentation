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
  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkImageData* input = vtkImageData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkImageData* output = vtkImageData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Convert input to the image class for this algorithm
  int* dims = input->GetDimensions();
  int cols = dims[0];
  int rows = dims[1];

  std::cout << "rows: " << rows << std::endl;
  std::cout << "cols: " << cols << std::endl;

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

  std::cout << "There were " << numberOfSegments << " segments." << std::endl;

  // Convert to the image class for this algorithm to a normal output type
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

