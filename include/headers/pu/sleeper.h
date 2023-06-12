/* date = May 30th 2023 0:30 pm */

// TODO (lucashdez): Comentar SLEEPER_H
#ifndef SLEEPER_H
#define SLEEPER_H

#include "../../headers/processing_unit_interface.h"

class Sleeper : public ProcessingUnitInterface {
  public:
  Sleeper();
  ~Sleeper();
  void Start(void** = nullptr);
  void Run(void *);
  void Delete();
  int start_arguments();
  ProcessingUnitInterface *Clone();
  
  private:
  u32 seconds_to_sleep_;
};

#endif // SLEEPER_H
