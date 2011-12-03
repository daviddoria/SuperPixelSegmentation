#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkJPEGWriter.h>

#include "vtkSuperPixelSegmentation.h"

int main(int argc, char* argv[])
{
  vtkSmartPointer<vtkJPEGReader> reader = 
      vtkSmartPointer<vtkJPEGReader>::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  vtkSmartPointer<vtkSuperPixelSegmentation> superPixelSegmentation = 
      vtkSmartPointer<vtkSuperPixelSegmentation>::New();

  superPixelSegmentation->SetSigma(.5);
  superPixelSegmentation->SetK(500);
  superPixelSegmentation->SetMinSize(50);
    
  superPixelSegmentation->SetInputConnection(reader->GetOutputPort());
  superPixelSegmentation->Update();
  
  vtkSmartPointer<vtkJPEGWriter> writer = 
      vtkSmartPointer<vtkJPEGWriter>::New();
  writer->SetFileName(argv[2]);
  writer->SetInputConnection(superPixelSegmentation->GetOutputPort());
  writer->Write();
  
  return EXIT_SUCCESS;
}