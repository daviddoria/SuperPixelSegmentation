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

#include "Panel.h"

// Custom
#include "FileSelectionWidget.h"
#include "Helpers.h"
#include "HelpersQt.h"
#include "Mask.h"

// Qt
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QWidget>

// STL
#include <iostream>

// ITK
#include "itkImageFileReader.h"

Panel::Panel()
{
  this->SelectionWidget = new FileSelectionWidget;
  this->GraphicsScene = new QGraphicsScene;
  this->GraphicsView = new QGraphicsView;
  this->GraphicsView->setScene(this->GraphicsScene);
  this->Label = new QLabel;
  
  this->Layout = new QVBoxLayout;
  this->Layout->addWidget(this->Label);
  this->Layout->addWidget(this->SelectionWidget);
  this->Layout->addWidget(this->GraphicsView);

  connect(this->SelectionWidget, SIGNAL(selectionChanged()), this, SLOT(LoadAndDisplay()));
}


void Panel::LoadAndDisplay()
{
  // sender()
  //this->SelectionWidget->FileName = this->SelectionWidget->currentIndex().data(QFileSystemModel::FilePathRole).toString().toStdString();
  typedef itk::VectorImage<float, 2> FloatVectorImageType;
  typedef itk::ImageFileReader<FloatVectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(this->SelectionWidget->currentIndex().data(QFileSystemModel::FilePathRole).toString().toStdString());
  reader->Update();

  this->Image = reader->GetOutput();

  QImage image = HelpersQt::GetQImageRGB<FloatVectorImageType>(this->Image, this->Image->GetLargestPossibleRegion());
  image = HelpersQt::FitToGraphicsView(image, this->GraphicsView);

  this->GraphicsScene->clear();
  this->GraphicsScene->addPixmap(QPixmap::fromImage(image));

}
