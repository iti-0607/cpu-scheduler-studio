#pragma once

#include "Scheduler.h"

class SJFScheduler : public Scheduler {
public:
    std::vector<ExecutionSegment> schedule(
        const std::vector<Process>& processes
    ) const override;
};