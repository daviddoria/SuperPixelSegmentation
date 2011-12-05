#ifndef COMPUTATIONOBJECT_H
#define COMPUTATIONOBJECT_H

class ComputationObject
{
public:
  virtual void Compute() = 0;
  
  unsigned int FinalNumberOfSegments;
};

#endif
