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
#include "ITKComputationThread.h"
#include "itkSLICSegmentation.h"
#include "itkGraphCutSegmentation.h"
#include "itkQuickShiftSegmentation.h"

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
  
  void on_btnSegmentGraphCut_clicked();
  void on_btnSegmentSLIC_clicked();
  void on_btnSegmentQuickShift_clicked();
  
  void on_chkShowInputImage_clicked();
  void on_chkShowColoredImage_clicked();
  void on_chkShowLabelImage_clicked();

  void slot_StartProgressBar();
  void slot_StopProgressBar();

  void slot_GraphCutComplete();
  void slot_SLICComplete();
  void slot_QuickShiftComplete();

protected:

  void Refresh();
  
  void showEvent ( QShowEvent * event );
  void resizeEvent ( QResizeEvent * event );
  
  void OpenImage(const std::string& imageFileName);

  LabelImageType::Pointer LabelImage;
  ImageType::Pointer Image;

  QGraphicsPixmapItem* InputImagePixmapItem;
  QGraphicsPixmapItem* LabelImagePixmapItem;
  QGraphicsPixmapItem* ColoredImagePixmapItem;
  
  QGraphicsScene* Scene;

  std::string SourceImageFileName;

  typedef itk::GraphCutSegmentation<ImageType, LabelImageType> GraphCutFilterType;
  GraphCutFilterType::Pointer GraphCutFilter;
  ITKComputationThread<GraphCutFilterType>* GraphCutThread;
  
  typedef itk::SLICSegmentation<ImageType, LabelImageType> SLICFilterType;
  SLICFilterType::Pointer SLICFilter;
  ITKComputationThread<SLICFilterType>* SLICThread;
  
  typedef itk::QuickShiftSegmentation<ImageType, LabelImageType> QuickShiftFilterType;
  QuickShiftFilterType::Pointer QuickShiftFilter;
  ITKComputationThread<QuickShiftFilterType>* QuickShiftThread;

private:
  // GraphCut sliders
  int MinSizeMin;
  int MinSizeMax;

  float KMin;
  float KMax;

  float SigmaMin;
  float SigmaMax;
  
  // QuickShift sliders
  float MaxDistMin;
  float MaxDistMax;
  
  float KernelSizeMin;
  float KernelSizeMax;
  
  float RatioMin;
  float RatioMax;
  
  // SLIC sliders
  float NumberOfSuperPixelsMin;
  float NumberOfSuperPixelsMax;
  
  float SpatialDistanceWeightMin;
  float SpatialDistanceWeightMax;

};

#endif // SuperPixelSegmentationGUI_H
