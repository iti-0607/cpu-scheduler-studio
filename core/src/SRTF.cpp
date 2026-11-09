
#include "SRTF.h"

#include <limits>
#include <vector>

std::vector<ExecutionSegment> SRTFScheduler::schedule(
    const std::vector<Process>& processes
) const {

    std::vector<ExecutionSegment> timeline;

    const int n = static_cast<int>(processes.size());

    if (n == 0) {
        return timeline;
    }

    std::vector<int> remainingTime;

    for (const Process& process : processes) {
        remainingTime.push_back(process.getBurstTime());
    }

    int currentTime = 0;
    int completedCount = 0;

    while (completedCount < n) {

        int selectedIndex = -1;
        int shortestRemaining =
            std::numeric_limits<int>::max();

        // Find the process with the shortest remaining time.
        for (int i = 0; i < n; i++) {

            if (remainingTime[i] <= 0) {
                continue;
            }

            if (processes[i].getArrivalTime() > currentTime) {
                continue;
            }

            if (remainingTime[i] < shortestRemaining) {
                shortestRemaining = remainingTime[i];
                selectedIndex = i;
            }
            else if (remainingTime[i] == shortestRemaining) {

                // Tie-break using smaller PID.
                if (selectedIndex == -1 ||
                    processes[i].getPID() <
                    processes[selectedIndex].getPID()) {

                    selectedIndex = i;
                }
            }
        }

        // No process is ready: CPU is idle.
        if (selectedIndex == -1) {

            // Merge consecutive idle time into one segment.
            if (!timeline.empty() && timeline.back().isIdle) {
                timeline.back().endTime++;
            } else {
                timeline.push_back({
                    -1,
                    currentTime,
                    currentTime + 1,
                    true
                });
            }

            currentTime++;
            continue;
        }

        int pid = processes[selectedIndex].getPID();

        // Execute for exactly one time unit.
        if (!timeline.empty() &&
            !timeline.back().isIdle &&
            timeline.back().pid == pid &&
            timeline.back().endTime == currentTime) {

            // Same process continues: extend its segment.
            timeline.back().endTime++;

        } else {

            // New process or preemption: create a segment.
            timeline.push_back({
                pid,
                currentTime,
                currentTime + 1,
                false
            });
        }

        remainingTime[selectedIndex]--;
        currentTime++;

        if (remainingTime[selectedIndex] == 0) {
            completedCount++;
        }
    }

    return timeline;
}
