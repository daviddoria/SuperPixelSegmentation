#ifndef ITKComputationThread_H
#define ITKComputationThread_H

#include <QThread>

class FilterComputationThreadNonTemplated : public QThread
{
Q_OBJECT
signals:
  // This signal is emitted to start the progress bar
  void StartProgressBarSignal();

  // This signal is emitted to stop the progress bar
  void StopProgressBarSignal();

};

template <typename TFilter>
class ITKComputationThread : public FilterComputationThreadNonTemplated
{
  public:

  ITKComputationThread();

  // This function is called when the thread is started
  void run();

  // This function is called when the thread is stopped
  void exit();

  void SetFilter(TFilter* filter);

private:
  TFilter* Filter;

};

#include "ITKComputationThread.hxx"

#endif
