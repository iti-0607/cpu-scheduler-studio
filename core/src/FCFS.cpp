
#include "FCFS.h"

#include <algorithm>

std::vector<ExecutionSegment> FCFSScheduler::schedule(
    const std::vector<Process>& processes
) const {

    // Copy processes so the original collection is unchanged
    std::vector<Process> sortedProcesses = processes;

    // Sort by arrival time, then PID for deterministic ties
    std::sort(
        sortedProcesses.begin(),
        sortedProcesses.end(),
        [](const Process& a, const Process& b) {

            if (a.getArrivalTime() != b.getArrivalTime()) {
                return a.getArrivalTime() < b.getArrivalTime();
            }

            return a.getPID() < b.getPID();
        }
    );

    std::vector<ExecutionSegment> timeline;

    int currentTime = 0;

    for (const Process& process : sortedProcesses) {

        int arrival = process.getArrivalTime();
        int burst = process.getBurstTime();

        // CPU remains idle if no process has arrived
        if (currentTime < arrival) {

            timeline.push_back({
                -1,
                currentTime,
                arrival,
                true
            });

            currentTime = arrival;
        }

        // Execute the process until completion
        int startTime = currentTime;
        int endTime = startTime + burst;

        timeline.push_back({
            process.getPID(),
            startTime,
            endTime,
            false
        });

        currentTime = endTime;
    }

    return timeline;
}
