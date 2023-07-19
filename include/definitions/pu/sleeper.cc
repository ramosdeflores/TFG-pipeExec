#include "../../headers/pu/sleeper.h"

// TODO: The comment

/**
 * @brief Default constructor: does nothing
 */
Sleeper::Sleeper() {}

/**
 * @brief Default destructor: does nothing
 */
Sleeper::~Sleeper() {}

void Sleeper::Start(void **pre_process_args) {
  seconds_to_sleep_ = 0;
  if (pre_process_args != nullptr) {
    seconds_to_sleep_ = *(int *)pre_process_args[0];
  }
}

void Sleeper::Run(void *) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds_to_sleep_));
}

ProcessingUnitInterface *Sleeper::Clone() { return new Sleeper; }

void Sleeper::Delete() {}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
