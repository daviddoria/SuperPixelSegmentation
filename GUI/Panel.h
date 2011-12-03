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

#ifndef Panel_H
#define Panel_H

#include "ui_FileSelector.h"

// Custom
#include "FileSelectionWidget.h"

// Qt
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

// ITK
#include "itkVectorImage.h"

typedef itk::VectorImage<float, 2> ImageType;

class Panel : public QObject
{
 Q_OBJECT
public:
  Panel();
  
  FileSelectionWidget* SelectionWidget;

  QGraphicsScene* GraphicsScene;

  ImageType::Pointer Image;

  QGraphicsView* GraphicsView;
  QVBoxLayout* Layout;
  QLabel* Label;
  
public slots:
  void LoadAndDisplay();
};

#endif // Panel_H
