
#include "SJF.h"

#include <limits>
#include <vector>

std::vector<ExecutionSegment> SJFScheduler::schedule(
    const std::vector<Process>& processes
) const {
    std::vector<ExecutionSegment> timeline;

    const int n = static_cast<int>(processes.size());

    // Tracks which processes have already completed.
    std::vector<bool> completed(n, false);

    int completedCount = 0;
    int currentTime = 0;

    while (completedCount < n) {
        int selectedIndex = -1;
        int shortestBurst = std::numeric_limits<int>::max();

        // Find the shortest process among those that have arrived.
        for (int i = 0; i < n; i++) {
            if (completed[i]) {
                continue;
            }

            if (processes[i].getArrivalTime() > currentTime) {
                continue;
            }

            int burst = processes[i].getBurstTime();

            if (burst < shortestBurst) {
                shortestBurst = burst;
                selectedIndex = i;
            }
            else if (burst == shortestBurst) {
                // Tie-break using smaller PID.
                if (selectedIndex == -1 ||
                    processes[i].getPID() <
                    processes[selectedIndex].getPID()) {
                    selectedIndex = i;
                }
            }
        }

        // No process has arrived yet: CPU must remain idle.
        if (selectedIndex == -1) {
            int nextArrival = std::numeric_limits<int>::max();

            for (int i = 0; i < n; i++) {
                if (!completed[i]) {
                    nextArrival = std::min(
                        nextArrival,
                        processes[i].getArrivalTime()
                    );
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

        // Execute the selected process completely.
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
