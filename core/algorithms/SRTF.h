#pragma once

#include "Scheduler.h"

class SRTFScheduler : public Scheduler {
public:
    std::vector<ExecutionSegment> schedule(
        const std::vector<Process>& processes
    ) const override;
};