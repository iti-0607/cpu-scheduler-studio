#pragma once

#include "Scheduler.h"
#include "MetricsCalculator.h"

#include <vector>

struct ProcessStateEvent {
    int pid;
    int time;
    ProcessState fromState;
    ProcessState toState;
};

struct SimulationResult {
    std::vector<ExecutionSegment> timeline;

    std::vector<ProcessMetrics> processMetrics;

    OverallMetrics overallMetrics;

    std::vector<ProcessStateEvent> stateHistory;
};