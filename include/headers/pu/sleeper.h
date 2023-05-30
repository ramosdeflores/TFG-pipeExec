/* date = May 30th 2023 0:30 pm */

// TODO(lucashdez): Unidad de procesado para sleeps

#ifndef SLEEPER_H
#define SLEEPER_H

#include "../../headers/processing_unit_interface.h"

class Sleeper : public ProcessingUnitInterface {
  public:
    Sleeper();
    ~Sleeper();
    void Start();
    void Run(void *);
    void Delete();
    ProcessingUnitInterface *Clone();

  private:
};

#endif // SLEEPER_H
