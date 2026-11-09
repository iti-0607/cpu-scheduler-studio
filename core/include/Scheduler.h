
#pragma once

#include "Process.h"
#include <vector>

// One continuous CPU execution interval
struct ExecutionSegment {
    int pid;
    int startTime;
    int endTime;
    bool isIdle;
};

// Common interface for all scheduling algorithms
class Scheduler {
public:
    virtual std::vector<ExecutionSegment> schedule(
        const std::vector<Process>& processes
    ) const = 0;

    virtual ~Scheduler() = default;
};
