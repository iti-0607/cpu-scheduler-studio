
#pragma once

#include "Scheduler.h"

class FCFSScheduler : public Scheduler {
public:
    std::vector<ExecutionSegment> schedule(
        const std::vector<Process>& processes
    ) const override;
};
