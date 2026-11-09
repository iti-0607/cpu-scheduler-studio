
#include "Priority.h"

#include <limits>
#include <vector>

std::vector<ExecutionSegment> PriorityScheduler::schedule(
    const std::vector<Process>& processes
) const {
    std::vector<ExecutionSegment> timeline;

    const int n = static_cast<int>(processes.size());

    std::vector<bool> completed(n, false);

    int completedCount = 0;
    int currentTime = 0;

    while (completedCount < n) {
        int selectedIndex = -1;
        int highestPriority = std::numeric_limits<int>::max();

        // Find the highest-priority process that has arrived.
        for (int i = 0; i < n; i++) {
            if (completed[i]) {
                continue;
            }

            if (processes[i].getArrivalTime() > currentTime) {
                continue;
            }

            int priority = processes[i].getPriority();

            if (priority < highestPriority) {
                highestPriority = priority;
                selectedIndex = i;
            }
            else if (priority == highestPriority) {
                // Tie-break using smaller PID.
                if (selectedIndex == -1 ||
                    processes[i].getPID() <
                    processes[selectedIndex].getPID()) {
                    selectedIndex = i;
                }
            }
        }

        // No process is ready: move time to the next arrival.
        if (selectedIndex == -1) {
            int nextArrival = std::numeric_limits<int>::max();

            for (int i = 0; i < n; i++) {
                if (!completed[i]) {
                    if (processes[i].getArrivalTime() < nextArrival) {
                        nextArrival = processes[i].getArrivalTime();
                    }
                }
            }

            timeline.push_back({
                -1,
                currentTime,
                nextArrival,
                true
            });

            currentTime = nextArrival;
            continue;
        }

        // Execute selected process completely.
        const Process& process = processes[selectedIndex];

        int startTime = currentTime;
        int endTime = currentTime + process.getBurstTime();

        timeline.push_back({
            process.getPID(),
            startTime,
            endTime,
            false
        });

        currentTime = endTime;

        completed[selectedIndex] = true;
        completedCount++;
    }

    return timeline;
}
