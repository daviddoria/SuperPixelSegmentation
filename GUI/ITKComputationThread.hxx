#include <iostream>

template<typename TFilter>
ITKComputationThread<TFilter>::ITKComputationThread()
{
  this->Filter = NULL;
}

template<typename TFilter>
void ITKComputationThread<TFilter>::run()
{
  std::cout << "ProgressThread::run()" << std::endl;
  // When the thread is started, emit the signal to start the marquee progress bar
  emit StartProgressBarSignal();

  this->Filter->Update();

  // When the function is finished, end the thread
  exit();
}

template<typename TFilter>
void ITKComputationThread<TFilter>::exit()
{
  std::cout << "ProgressThread::exit()" << std::endl;
  
  // When the thread is stopped, emit the signal to stop the marquee progress bar
  emit StopProgressBarSignal();
}

template<typename TFilter>
void ITKComputationThread<TFilter>::SetFilter(TFilter* filter)
{
  this->Filter = filter;
}
