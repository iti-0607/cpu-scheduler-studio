#pragma once

#include "Scheduler.h"
#include <vector>

struct ProcessMetrics {
    int pid;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int responseTime;
};

struct OverallMetrics {
    double averageWaitingTime;
    double averageTurnaroundTime;
    double averageResponseTime;
    double cpuUtilization;
    double throughput;
};

class MetricsCalculator {
public:
    static std::vector<ProcessMetrics> calculateProcessMetrics(
        const std::vector<Process>& processes,
        const std::vector<ExecutionSegment>& timeline
    );

    static OverallMetrics calculateOverallMetrics(
        const std::vector<ProcessMetrics>& metrics,
        const std::vector<ExecutionSegment>& timeline
    );
};