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
  
  // Marquee mode
  this->progressBar->setMinimum(0);
  this->progressBar->setMaximum(0);
  this->progressBar->hide();

  this->GraphCutFilter = GraphCutFilterType::New();
  this->SLICFilter = SLICFilterType::New();
  
  this->GraphCutThread = new ITKComputationThread<GraphCutFilterType>;
  connect(this->GraphCutThread, SIGNAL(StartProgressBarSignal()), this, SLOT(slot_StartProgressBar()));
  connect(this->GraphCutThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_StopProgressBar()));
  connect(this->GraphCutThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_GraphCutComplete()));

  this->SLICThread = new ITKComputationThread<SLICFilterType>;
  connect(this->SLICThread, SIGNAL(StartProgressBarSignal()), this, SLOT(slot_StartProgressBar()));
  connect(this->SLICThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_StopProgressBar()));
  connect(this->SLICThread, SIGNAL(StopProgressBarSignal()), this, SLOT(slot_SLICComplete()));
  
  this->Image = ImageType::New();
  this->LabelImage = LabelImageType::New();

  this->Scene = new QGraphicsScene;
  this->graphicsView->setScene(this->Scene);

  this->InputImagePixmapItem = NULL;
  this->LabelImagePixmapItem = NULL;
  this->ColoredImagePixmapItem = NULL;
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
  if(this->InputImagePixmapItem)
    {
    this->graphicsView->fitInView(this->InputImagePixmapItem, Qt::KeepAspectRatio);
    }
}

void SuperPixelSegmentationGUI::resizeEvent ( QResizeEvent * event )
{
  if(this->InputImagePixmapItem)
    {
    this->graphicsView->fitInView(this->InputImagePixmapItem, Qt::KeepAspectRatio);
    }
}

void SuperPixelSegmentationGUI::on_btnSegmentGraphCut_clicked()
{
  this->GraphCutFilter->SetK(this->sldGraphCutK->GetValue());
  this->GraphCutFilter->SetSigma(this->sldGraphCutSigma->GetValue());
  this->GraphCutFilter->SetMinSize(this->sldGraphCutMinSize->value());
  this->GraphCutFilter->SetInput(this->Image);
  GraphCutThread->SetFilter(this->GraphCutFilter);
  std::cout << "Starting graph cut thread..." << std::endl;
  GraphCutThread->start();
}

void SuperPixelSegmentationGUI::on_btnSegmentSLIC_clicked()
{
  this->SLICFilter->SetSpatialDistanceWeight(this->sldSLICSpatialDistanceWeight->GetValue());
  this->SLICFilter->SetNumberOfSuperPixels(this->sldSLICNumberOfSuperPixels->value());
  this->SLICFilter->SetInput(this->Image);
  SLICThread->SetFilter(this->SLICFilter);
  std::cout << "Starting SLIC thread..." << std::endl;
  SLICThread->start();
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
  this->InputImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimageImage));
  this->graphicsView->fitInView(this->InputImagePixmapItem);
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

void SuperPixelSegmentationGUI::on_chkShowInputImage_clicked()
{
  Refresh();
}

void SuperPixelSegmentationGUI::on_chkShowLabelImage_clicked()
{
  Refresh();
}

void SuperPixelSegmentationGUI::on_chkShowColoredImage_clicked()
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


void SuperPixelSegmentationGUI::slot_GraphCutComplete()
{
//   std::stringstream ss;
//   ss << "Computed " << numberOfSegments << " segments." << std::endl;
//   this->statusBar()->showMessage(ss.str().c_str());

//   QImage qimage = HelpersQt::GetQImageScalar<LabelImageType>(this->GraphCutFilter->GetLabelImage());
//   if(this->LabelImagePixmapItem)
//     {
//     this->Scene->removeItem(this->LabelImagePixmapItem);
//     }
//   this->LabelImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimage));
  
  // Display label image
  typedef itk::ScalarToRGBColormapImageFilter<LabelImageType, ImageType> ColorMapFilterType;
  ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
  colorMapFilter->SetInput(this->GraphCutFilter->GetLabelImage());
  colorMapFilter->SetColormap( ColorMapFilterType::Hot );
  colorMapFilter->Update();
  
  QImage qimage = HelpersQt::GetQImageRGB<ImageType>(colorMapFilter->GetOutput());
  if(this->LabelImagePixmapItem)
    {
    this->Scene->removeItem(this->LabelImagePixmapItem);
    }
  this->LabelImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimage));

  // Display colored image
  QImage qColoredImage = HelpersQt::GetQImageRGB<ImageType>(this->GraphCutFilter->GetColoredImage());
  if(this->ColoredImagePixmapItem)
    {
    this->Scene->removeItem(this->ColoredImagePixmapItem);
    }
  this->ColoredImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qColoredImage));
  
  Refresh();
}

void SuperPixelSegmentationGUI::slot_SLICComplete()
{
//   QImage qLabelImage = HelpersQt::GetQImageScalar<LabelImageType>(this->SLICFilter->GetLabelImage());
//   if(this->LabelImagePixmapItem)
//     {
//     this->Scene->removeItem(this->LabelImagePixmapItem);
//     }
//   this->LabelImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qLabelImage));

  // Display label image
  typedef itk::ScalarToRGBColormapImageFilter<LabelImageType, ImageType> ColorMapFilterType;
  ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
  colorMapFilter->SetInput(this->SLICFilter->GetLabelImage());
  colorMapFilter->SetColormap( ColorMapFilterType::Hot );
  colorMapFilter->Update();
  
  QImage qimage = HelpersQt::GetQImageRGB<ImageType>(colorMapFilter->GetOutput());
  if(this->LabelImagePixmapItem)
    {
    this->Scene->removeItem(this->LabelImagePixmapItem);
    }
  this->LabelImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qimage));
  
  // Display colored image
  
  QImage qColoredImage = HelpersQt::GetQImageRGB<ImageType>(this->SLICFilter->GetColoredImage());
  if(this->ColoredImagePixmapItem)
    {
    this->Scene->removeItem(this->ColoredImagePixmapItem);
    }
  this->ColoredImagePixmapItem = this->Scene->addPixmap(QPixmap::fromImage(qColoredImage));

  Refresh();
}

void SuperPixelSegmentationGUI::Refresh()
{
  if(this->LabelImagePixmapItem)
    {
    this->LabelImagePixmapItem->setVisible(this->chkShowLabelImage->isChecked());
    }
  if(this->InputImagePixmapItem)
    {
    this->InputImagePixmapItem->setVisible(this->chkShowInputImage->isChecked());
    }
  if(this->ColoredImagePixmapItem)
    {
    this->ColoredImagePixmapItem->setVisible(this->chkShowColoredImage->isChecked());
    }
}
