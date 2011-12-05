#ifndef LabeledSlider_H
#define LabeledSlider_H

#include <QSlider>
#include <QtDesigner/QDesignerExportWidget>

#include "ui_LabeledSlider.h"

class QIntValidator;

class QDESIGNER_WIDGET_EXPORT LabeledSlider : public QWidget, public Ui::LabeledSlider
{
Q_OBJECT

signals:
  void valueChanged(int);
  
public slots:
  virtual void slot_horizontalSlider_valueChanged(int); // We can't use the named slot autoconnect here because we want to connect to a different slot in a subclass
  virtual void on_txtMin_textEdited( const QString & text );
  virtual void on_txtMax_textEdited( const QString & text );
  
public:
  LabeledSlider(QWidget *parent = 0);

  unsigned int value();

  void setMinimum(const unsigned int);
  void setMaximum(const unsigned int);
protected:
  QIntValidator* Validator;

};

#endif
