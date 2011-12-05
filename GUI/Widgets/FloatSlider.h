#ifndef FloatSlider_H
#define FloatSlider_H

#include <QtDesigner/QDesignerExportWidget>
class QDoubleValidator;

#include "LabeledSlider.h"

// This slider should always be set from min = 0 to max = 100.
// This then lets us interpret the difference between the min and max
// as a percentage.
class QDESIGNER_WIDGET_EXPORT FloatSlider : public LabeledSlider
{
Q_OBJECT

signals:
  void valueChanged(float);

public:
  FloatSlider(QWidget *parent = 0);

  float GetValue();
  void setMinimum(const float);
  void setMaximum(const float);

public slots:
  void slot_horizontalSlider_valueChanged(int value);
  virtual void on_txtMin_textEdited( const QString & text );
  virtual void on_txtMax_textEdited( const QString & text );
  
protected:
  float MinValue;
  float MaxValue;
  QDoubleValidator* Validator;

};

#endif
