#include "MetricsCalculator.h"
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

std::vector<ProcessMetrics>
MetricsCalculator::calculateProcessMetrics(
    const std::vector<Process>& processes,
    const std::vector<ExecutionSegment>& timeline
) {
    std::unordered_map<int, int> firstStart;
    std::unordered_map<int, int> completion;

    // Find first start time and completion time of each process.
    for (const ExecutionSegment& segment : timeline) {
        if (segment.isIdle) {
            continue;
        }

        if (segment.endTime < segment.startTime) {
            throw std::invalid_argument("Invalid execution segment");
        }

        int pid = segment.pid;

        if (firstStart.find(pid) == firstStart.end()) {
            firstStart[pid] = segment.startTime;
        } else {
            firstStart[pid] =
                std::min(firstStart[pid], segment.startTime);
        }

        completion[pid] = std::max(
            completion.count(pid) ? completion[pid] : segment.endTime,
            segment.endTime
        );
    }

    std::vector<ProcessMetrics> result;

    for (const Process& process : processes) {
        int pid = process.getPID();

        if (firstStart.find(pid) == firstStart.end() ||
            completion.find(pid) == completion.end()) {
            throw std::invalid_argument(
                "Process is missing from execution timeline"
            );
        }

        int ct = completion[pid];
        int tat = ct - process.getArrivalTime();
        int wt = tat - process.getBurstTime();
        int rt = firstStart[pid] - process.getArrivalTime();

        if (tat < 0 || wt < 0 || rt < 0) {
            throw std::invalid_argument(
                "Invalid scheduling timeline or process data"
            );
        }

        result.push_back({
            pid,
            ct,
            tat,
            wt,
            rt
        });
    }

    return result;
}

OverallMetrics MetricsCalculator::calculateOverallMetrics(
    const std::vector<ProcessMetrics>& metrics,
    const std::vector<ExecutionSegment>& timeline
) {
    if (metrics.empty() || timeline.empty()) {
        return {0, 0, 0, 0, 0};
    }

    double totalWT = 0;
    double totalTAT = 0;
    double totalRT = 0;
    double busyTime = 0;

    int totalTime = timeline.back().endTime;

    for (const ProcessMetrics& metric : metrics) {
        totalWT += metric.waitingTime;
        totalTAT += metric.turnaroundTime;
        totalRT += metric.responseTime;
    }

    for (const ExecutionSegment& segment : timeline) {
        if (!segment.isIdle) {
            busyTime += segment.endTime - segment.startTime;
        }
    }

    double processCount = metrics.size();

    OverallMetrics overall;

    overall.averageWaitingTime = totalWT / processCount;
    overall.averageTurnaroundTime = totalTAT / processCount;
    overall.averageResponseTime = totalRT / processCount;

    overall.cpuUtilization =
        totalTime > 0 ? (busyTime / totalTime) * 100.0 : 0;

    overall.throughput =
        totalTime > 0 ? processCount / totalTime : 0;

    return overall;
}