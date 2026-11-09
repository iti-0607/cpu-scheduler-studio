
#include "RoundRobin.h"

#include <algorithm>
#include <queue>
#include <stdexcept>
#include <vector>

RoundRobinScheduler::RoundRobinScheduler(int timeQuantum)
    : timeQuantum(timeQuantum) {

    if (timeQuantum <= 0) {
        throw std::invalid_argument(
            "Time quantum must be positive"
        );
    }
}

std::vector<ExecutionSegment> RoundRobinScheduler::schedule(
    const std::vector<Process>& processes
) const {

    std::vector<ExecutionSegment> timeline;

    if (processes.empty()) {
        return timeline;
    }

    // Sort process indices by arrival time, then PID.
    std::vector<int> order;

    for (int i = 0; i < static_cast<int>(processes.size()); i++) {
        order.push_back(i);
    }

    std::sort(
        order.begin(),
        order.end(),
        [&processes](int a, int b) {
            if (processes[a].getArrivalTime() !=
                processes[b].getArrivalTime()) {

                return processes[a].getArrivalTime() <
                       processes[b].getArrivalTime();
            }

            return processes[a].getPID() <
                   processes[b].getPID();
        }
    );

    // Each process gets its own remaining burst time.
    std::vector<int> remainingTime;

    for (const Process& process : processes) {
        remainingTime.push_back(process.getBurstTime());
    }

    std::queue<int> readyQueue;

    int currentTime = 0;
    int nextProcess = 0;
    int completedCount = 0;

    const int n = static_cast<int>(processes.size());

    while (completedCount < n) {

        // Add all processes that have arrived.
        while (nextProcess < n &&
               processes[order[nextProcess]].getArrivalTime()
                   <= currentTime) {

            readyQueue.push(order[nextProcess]);
            nextProcess++;
        }

        // If nobody is ready, advance to the next arrival.
        if (readyQueue.empty()) {

            int nextArrival =
                processes[order[nextProcess]].getArrivalTime();

            timeline.push_back({
                -1,
                currentTime,
                nextArrival,
                true
            });

            currentTime = nextArrival;
            continue;
        }

        // Select the process at the front of the queue.
        int index = readyQueue.front();
        readyQueue.pop();

        int executionTime = std::min(
            timeQuantum,
            remainingTime[index]
        );

        int startTime = currentTime;
        int endTime = currentTime + executionTime;

        timeline.push_back({
            processes[index].getPID(),
            startTime,
            endTime,
            false
        });

        currentTime = endTime;
        remainingTime[index] -= executionTime;

        // First enqueue processes that arrived during this slice.
        // This includes arrivals exactly at the slice end.
        while (nextProcess < n &&
               processes[order[nextProcess]].getArrivalTime()
                   <= currentTime) {

            readyQueue.push(order[nextProcess]);
            nextProcess++;
        }

        // If the current process is unfinished, put it at the back.
        if (remainingTime[index] > 0) {
            readyQueue.push(index);
        } else {
            completedCount++;
        }
    }

    return timeline;
}
