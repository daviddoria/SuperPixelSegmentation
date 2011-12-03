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

#ifndef COMPUTATIONTHREAD_H
#define COMPUTATIONTHREAD_H

#include <QThread>

#include "ComputationObject.h"

// This class is named 'ComputationThreadClass' instead of just 'ComputationThread'
// because we often want to name a member variable 'ComputationThread'
class ComputationThreadClass : public QThread
{
Q_OBJECT

signals:
  // This signal is emitted to start the progress bar
  void StartProgressBarSignal();

  // This signal is emitted to stop the progress bar
  void StopProgressBarSignal();

  void RefreshSignal();
  
  void IterationCompleteSignal();
  void StepCompleteSignal();
  
public:
  ComputationThreadClass();

  // Store the type of operation to perform.
  enum OPERATION {ALLSTEPS, SINGLESTEP};
  OPERATION Operation;

  // This function is called when the thread is started.
  void run();

  virtual void AllSteps() = 0;
  virtual void SingleStep() = 0;

  // This function is called when the thread is stopped.
  void exit();

  void StopComputation();

  // Provide the object with which to do the computation.
  void SetObject(ComputationObject*);
  
protected:
  // We need a pointer to this object so we can perform the computations in this thread
  ComputationObject* Object;
  
  // This flag can be set from another thread (by calling StopInpainting()) to indicate that we want to stop the computation at the next possible opportunity.
  bool Stop;
};

#endif