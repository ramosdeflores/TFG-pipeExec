#include "../../headers/pu/sleeper.h"

Sleeper::Sleeper() {}
Sleeper::~Sleeper() {}

void
Sleeper::Start() {}

void
Sleeper::Run(void *) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

ProcessingUnitInterface *
Sleeper::Clone() {
    return new Sleeper;
}

void
Sleeper::Delete() {}
