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

#include "SuperPixelSegmentationGUI.h"

// Custom
#include "Helpers.h"
#include "HelpersQt.h"
#include "GraphCutSegmentationComputationObject.h"
#include "SLICSegmentationComputationObject.h"

// ITK
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"
#include "itkScalarToRGBColormapImageFilter.h"

// Qt
#include <QIcon>
#include <QFileDialog>
#include <QGraphicsPixmapItem>

// Called by all constructors
void SuperPixelSegmentationGUI::DefaultConstructor()
{
  this->setupUi(this);

  this->MinSizeMin = 0;
  this->MinSizeMax = 200;
  this->sldGraphCutMinSize->setMinimum(this->MinSizeMin);
  this->sldGraphCutMinSize->setMaximum(this->MinSizeMax);
  
  this->KMin = 0.0f;
  this->KMax = 1000.0f;
  this->sldGraphCutK->setMinimum(this->KMin);
  this->sldGraphCutK->setMaximum(this->KMax);

  this->SigmaMin = 0.0f;
  this->SigmaMax = 2.0f;
  this->sldGraphCutSigma->setMinimum(this->SigmaMin);
  this->sldGraphCutSigma->setMaximum(this->SigmaMax);
  
  this->progressBar->setMinimum(0);
  this->progressBar->setMaximum(0);
  this->progressBar->hide();

  this->ComputationThread = new GraphCutSegmentationComputationThread;
  connect(this->ComputationThread, SIGNAL(StartProgressBarSignal()), this, SLOT(slot_StartProgressBar()));
  connect(this->ComputationThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_StopProgressBar()));
  //connect(this->ComputationThread, SIGNAL(IterationCompleteSignal()), this, SLOT(slot_IterationComplete()));
  connect(this->ComputationThread, SIGNAL(IterationCompleteSignal(unsigned int)), this, SLOT(slot_IterationComplete(unsigned int)));

  this->Image = ImageType::New();
  this->LabelImage = LabelImageType::New();

  this->Scene = new QGraphicsScene;
  this->graphicsView->setScene(this->Scene);

  this->ImagePixmapItem = NULL;
  this->LabelImagePixmapItem = NULL;
}

// Default constructor
SuperPixelSegmentationGUI::SuperPixelSegmentationGUI()
{
  DefaultConstructor();
};

SuperPixelSegmentationGUI::SuperPixelSegmentationGUI(const std::string& imageFileName)
{
  DefaultConstructor();
  this->SourceImageFileName = imageFileName;
  
  OpenImage(this->SourceImageFileName);
}

void SuperPixelSegmentationGUI::showEvent ( QShowEvent * event )
{
  if(this->ImagePixmapItem)
    {
    this->graphicsView->fitInView(this->ImagePixmapItem, Qt::KeepAspectRatio);
    }
}

void SuperPixelSegmentationGUI::resizeEvent ( QResizeEvent * event )
{
  if(this->ImagePixmapItem)
    {
    this->graphicsView->fitInView(this->ImagePixmapItem, Qt::KeepAspectRatio);
    }
}

void SuperPixelSegmentationGUI::on_btnSegmentGraphCut_clicked()
{
  GraphCutSegmentationComputationObject<ImageType, LabelImageType>* computationObject =
    new GraphCutSegmentationComputationObject<ImageType, LabelImageType>;
  computationObject->K = this->sldGraphCutK->GetValue();
  computationObject->Sigma = this->sldGraphCutSigma->GetValue();
  computationObject->MinSize = this->sldGraphCutMinSize->value();
  computationObject->Image = this->Image;
  computationObject->LabelImage = this->LabelImage;
  ComputationThread->SetObject(computationObject);
  ComputationThread->start();
}

void SuperPixelSegmentationGUI::on_btnSegmentSLIC_clicked()
{
  SLICSegmentationComputationObject<ImageType, LabelImageType>* computationObject =
    new SLICSegmentationComputationObject<ImageType, LabelImageType>;
  computationObject->SpatialDistanceWeight = this->sldSLICSpatialDistanceWeight->GetValue();
  computationObject->NumberOfSuperPixels = this->sldSLICNumberOfSuperPixels->value();
  computationObject->Image = this->Image;
  computationObject->LabelImage = this->LabelImage;
  ComputationThread->SetObject(computationObject);
  ComputationThread->start();
}

void SuperPixelSegmentationGUI::on_actionSaveResult_activated()
{
  // Get a filename to save
  QString fileName = QFileDialog::getSaveFileName(this, "Save File", ".", "Image Files (*.jpg *.jpeg *.bmp *.png *.mha)");

  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty." << std::endl;
    return;
    }

  Helpers::WriteImage<LabelImageType>(this->LabelImage, fileName.toStdString());
  this->statusBar()->showMessage("Saved result.");
}

void SuperPixelSegmentationGUI::OpenImage(const std::string& imageFileName)
{
  // Load and display image
  typedef itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(imageFileName);
  imageReader->Update();

  Helpers::DeepCopy<ImageType>(imageReader->GetOutput(), this->Image);

  QImage qimageImage = HelpersQt::GetQImageRGBA<ImageType>(this->Image);
  this->ImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimageImage));
  this->graphicsView->fitInView(this->ImagePixmapItem);
  Refresh();
}

void SuperPixelSegmentationGUI::on_actionOpenImage_activated()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                    "OpenFile", ".", "All Files (*.*)");

  if(!fileName.isEmpty())
    {
    OpenImage(fileName.toStdString());
    }
}

void SuperPixelSegmentationGUI::on_chkShowInput_clicked()
{
  Refresh();
}

void SuperPixelSegmentationGUI::on_chkShowSegments_clicked()
{
  Refresh();
}

void SuperPixelSegmentationGUI::slot_StartProgressBar()
{
  this->progressBar->show();
}

void SuperPixelSegmentationGUI::slot_StopProgressBar()
{
  this->progressBar->hide();
}


void SuperPixelSegmentationGUI::slot_IterationComplete(unsigned int numberOfSegments)
{
  std::stringstream ss;
  ss << "Computed " << numberOfSegments << " segments." << std::endl;
  this->statusBar()->showMessage(ss.str().c_str());

  ImageType::Pointer coloredLabelImage = ImageType::New();
  Helpers::ColorLabelsByAverageColor<ImageType, LabelImageType>(this->Image, this->LabelImage, coloredLabelImage);

  QImage qimage = HelpersQt::GetQImageRGB<ImageType>(coloredLabelImage);
  if(this->LabelImagePixmapItem)
    {
    this->Scene->removeItem(this->LabelImagePixmapItem);
    }
  this->LabelImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimage));

  Refresh();
}

/*
void SuperPixelSegmentationGUI::slot_IterationComplete()
{
  //Helpers::WriteImage<LabelImageType>(this->LabelImage, "LabelImage.mha");
  //QImage qimage = HelpersQt::GetQImageScalar<LabelImageType>(this->LabelImage);

  
  typedef itk::Image<itk::RGBPixel<unsigned char>, 2> RGBImageType;
  typedef itk::ScalarToRGBColormapImageFilter<LabelImageType, RGBImageType> ColorMapFilterType;
  ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
  colorMapFilter->SetInput(this->LabelImage);
  colorMapFilter->SetColormap( ColorMapFilterType::Hot );
  colorMapFilter->Update();

  QImage qimage = HelpersQt::GetQImageRGB<RGBImageType>(colorMapFilter->GetOutput());
  if(this->LabelImagePixmapItem)
    {
    this->Scene->removeItem(this->LabelImagePixmapItem);
    }
  this->LabelImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimage));

  
  Refresh();
}
*/

void SuperPixelSegmentationGUI::Refresh()
{
  if(this->LabelImagePixmapItem)
    {
    this->LabelImagePixmapItem->setVisible(this->chkShowSegments->isChecked());
    }
  if(this->ImagePixmapItem)
    {
    this->ImagePixmapItem->setVisible(this->chkShowInput->isChecked());
    }
}
