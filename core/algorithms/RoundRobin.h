#pragma once

#include "Scheduler.h"

class RoundRobinScheduler : public Scheduler {
public:
    explicit RoundRobinScheduler(int timeQuantum);

    std::vector<ExecutionSegment> schedule(
        const std::vector<Process>& processes
    ) const override;

private:
    int timeQuantum;
};