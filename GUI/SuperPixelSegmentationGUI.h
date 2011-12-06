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

#ifndef SuperPixelSegmentation_H
#define SuperPixelSegmentation_H

#include "ui_SuperPixelSegmentationGUI.h"

// ITK
#include "itkImage.h"
#include "itkVectorImage.h"

// Custom
#include "SuperPixelSegmentationComputationThread.h"

// Qt
#include <QMainWindow>
class QGraphicsPixmapItem;

class SuperPixelSegmentationGUI : public QMainWindow, public Ui::SuperPixelSegmentationGUI
{
  Q_OBJECT
public:
  void DefaultConstructor();
  SuperPixelSegmentationGUI();
  SuperPixelSegmentationGUI(const std::string& imageFileName);

  //typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2> ImageType;
  typedef itk::VectorImage<unsigned char, 2> ImageType;
  typedef itk::Image<int, 2> LabelImageType;

public slots:

  void on_actionOpenImage_activated();
  void on_actionSaveResult_activated();
  
  void on_btnSegment_clicked();
  
  void on_chkShowInput_clicked();
  void on_chkShowSegments_clicked();

  void slot_StartProgressBar();
  void slot_StopProgressBar();
  void slot_IterationComplete(unsigned int);

protected:

  void Refresh();
  
  void showEvent ( QShowEvent * event );
  void resizeEvent ( QResizeEvent * event );
  
  void OpenImage(const std::string& imageFileName);

  LabelImageType::Pointer LabelImage;
  ImageType::Pointer Image;

  QGraphicsPixmapItem* ImagePixmapItem;
  QGraphicsPixmapItem* LabelImagePixmapItem;
  
  QGraphicsScene* Scene;

  std::string SourceImageFileName;


  SuperPixelSegmentationComputationThread* ComputationThread;

private:
  int MinSizeMin;
  int MinSizeMax;

  float KMin;
  float KMax;

  float SigmaMin;
  float SigmaMax;

};

#endif // SuperPixelSegmentationGUI_H
