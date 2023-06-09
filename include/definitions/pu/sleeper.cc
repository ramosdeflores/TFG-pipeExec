#include "../../headers/pu/sleeper.h"

Sleeper::Sleeper() {}
Sleeper::~Sleeper() {}

void
Sleeper::Start(void* pre_process_args) {
  seconds_to_sleep = 0;
}

void
Sleeper::Run(void *) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds_to_sleep));
}

ProcessingUnitInterface *
Sleeper::Clone() {
  return new Sleeper;
}

void
Sleeper::Delete() {}

int
Sleeper::start_arguments() {
  return 1;
}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */