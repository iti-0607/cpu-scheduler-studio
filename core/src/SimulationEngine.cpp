#include "SimulationEngine.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

SimulationResult SimulationEngine::run(
    const Scheduler& scheduler,
    const std::vector<Process>& processes
) const {

    SimulationResult result;

    // 1. Generate execution timeline.
    result.timeline = scheduler.schedule(processes);

    // 2. Calculate process metrics.
    result.processMetrics =
        MetricsCalculator::calculateProcessMetrics(
            processes,
            result.timeline
        );

    // 3. Calculate overall metrics.
    result.overallMetrics =
        MetricsCalculator::calculateOverallMetrics(
            result.processMetrics,
            result.timeline
        );

    // 4. Generate process state transitions.
    for (const Process& process : processes) {

        std::vector<ExecutionSegment> processSegments;

        for (const ExecutionSegment& segment : result.timeline) {
            if (!segment.isIdle &&
                segment.pid == process.getPID()) {
                processSegments.push_back(segment);
            }
        }

        // A process must have at least one execution segment.
        if (processSegments.empty()) {
            continue;
        }

        // Sort this process's segments by start time.
        std::sort(
            processSegments.begin(),
            processSegments.end(),
            [](const ExecutionSegment& a,
               const ExecutionSegment& b) {
                return a.startTime < b.startTime;
            }
        );

        // NEW -> READY
        result.stateHistory.push_back({
            process.getPID(),
            process.getArrivalTime(),
            ProcessState::NEW,
            ProcessState::READY
        });

        // READY -> RUNNING
        result.stateHistory.push_back({
            process.getPID(),
            processSegments[0].startTime,
            ProcessState::READY,
            ProcessState::RUNNING
        });

        // Handle preemption and resumption.
        for (std::size_t i = 1;
             i < processSegments.size();
             i++) {

            const auto& previous = processSegments[i - 1];
            const auto& current = processSegments[i];

            // No transition if execution is continuous.
            if (previous.endTime == current.startTime) {
                continue;
            }

            // RUNNING -> READY (preemption)
            result.stateHistory.push_back({
                process.getPID(),
                previous.endTime,
                ProcessState::RUNNING,
                ProcessState::READY
            });

            // READY -> RUNNING (resumption)
            result.stateHistory.push_back({
                process.getPID(),
                current.startTime,
                ProcessState::READY,
                ProcessState::RUNNING
            });
        }

        // RUNNING -> TERMINATED
        result.stateHistory.push_back({
            process.getPID(),
            processSegments.back().endTime,
            ProcessState::RUNNING,
            ProcessState::TERMINATED
        });
    }

    // Keep all events in chronological order.
    std::stable_sort(
        result.stateHistory.begin(),
        result.stateHistory.end(),
        [](const ProcessStateEvent& a,
           const ProcessStateEvent& b) {
            return a.time < b.time;
        }
    );

    return result;
}