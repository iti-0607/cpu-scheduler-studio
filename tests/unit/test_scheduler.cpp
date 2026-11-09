
#include "Process.h"
#include "ProcessManager.h"
#include "FCFS.h"
#include "MetricsCalculator.h"
#include "SJF.h"
#include "Priority.h"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        failed++;
    }
}

void checkEqual(
    int actual,
    int expected,
    const std::string& testName
) {
    check(actual == expected, testName);
}

void checkNear(
    double actual,
    double expected,
    const std::string& testName
) {
    check(std::abs(actual - expected) < 0.01, testName);
}

// TEST 1: Basic FCFS scheduling
void testBasicFCFS() {
    std::vector<Process> processes = {
        Process(1, 0, 5),
        Process(2, 1, 3),
        Process(3, 2, 8)
    };

    FCFSScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    checkEqual(static_cast<int>(timeline.size()), 3,
               "FCFS creates three execution segments");

    checkEqual(timeline[0].pid, 1, "P1 executes first");
    checkEqual(timeline[1].pid, 2, "P2 executes second");
    checkEqual(timeline[2].pid, 3, "P3 executes third");

    checkEqual(timeline[0].startTime, 0, "P1 starts at 0");
    checkEqual(timeline[0].endTime, 5, "P1 ends at 5");
    checkEqual(timeline[2].endTime, 16, "P3 ends at 16");
}

// TEST 2: CPU idle time
void testIdleTime() {
    std::vector<Process> processes = {
        Process(1, 3, 4),
        Process(2, 8, 2)
    };

    FCFSScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    checkEqual(static_cast<int>(timeline.size()), 4,
               "Idle-time test creates four segments");

    check(timeline[0].isIdle, "CPU is idle initially");
    checkEqual(timeline[0].startTime, 0, "Initial idle starts at 0");
    checkEqual(timeline[0].endTime, 3, "Initial idle ends at 3");

    checkEqual(timeline[1].pid, 1, "P1 executes after initial idle");

    check(timeline[2].isIdle, "CPU is idle between processes");
    checkEqual(timeline[2].startTime, 7, "Second idle starts at 7");
    checkEqual(timeline[2].endTime, 8, "Second idle ends at 8");
}

// TEST 3: Simultaneous arrivals
void testSimultaneousArrivals() {
    std::vector<Process> processes = {
        Process(3, 0, 2),
        Process(1, 0, 4),
        Process(2, 0, 3)
    };

    FCFSScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    // Our FCFS implementation breaks arrival-time ties by PID.
    checkEqual(timeline[0].pid, 1, "Lowest PID executes first");
    checkEqual(timeline[1].pid, 2, "Second-lowest PID executes next");
    checkEqual(timeline[2].pid, 3, "Highest PID executes last");
}

// TEST 4: Process performance metrics
void testProcessMetrics() {
    std::vector<Process> processes = {
        Process(1, 0, 5),
        Process(2, 1, 3),
        Process(3, 2, 8)
    };

    FCFSScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    auto metrics =
        MetricsCalculator::calculateProcessMetrics(
            processes, timeline
        );

    checkEqual(static_cast<int>(metrics.size()), 3,
               "Metrics generated for all processes");

    checkEqual(metrics[0].completionTime, 5, "P1 CT");
    checkEqual(metrics[0].turnaroundTime, 5, "P1 TAT");
    checkEqual(metrics[0].waitingTime, 0, "P1 WT");
    checkEqual(metrics[0].responseTime, 0, "P1 RT");

    checkEqual(metrics[1].completionTime, 8, "P2 CT");
    checkEqual(metrics[1].turnaroundTime, 7, "P2 TAT");
    checkEqual(metrics[1].waitingTime, 4, "P2 WT");
    checkEqual(metrics[1].responseTime, 4, "P2 RT");

    checkEqual(metrics[2].completionTime, 16, "P3 CT");
    checkEqual(metrics[2].turnaroundTime, 14, "P3 TAT");
    checkEqual(metrics[2].waitingTime, 6, "P3 WT");
    checkEqual(metrics[2].responseTime, 6, "P3 RT");
}

// TEST 5: Overall performance metrics
void testOverallMetrics() {
    std::vector<Process> processes = {
        Process(1, 0, 5),
        Process(2, 1, 3),
        Process(3, 2, 8)
    };

    FCFSScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    auto processMetrics =
        MetricsCalculator::calculateProcessMetrics(
            processes, timeline
        );

    auto overall =
        MetricsCalculator::calculateOverallMetrics(
            processMetrics, timeline
        );

    checkNear(overall.averageWaitingTime, 10.0 / 3.0,
              "Average waiting time");

    checkNear(overall.averageTurnaroundTime, 26.0 / 3.0,
              "Average turnaround time");

    checkNear(overall.averageResponseTime, 10.0 / 3.0,
              "Average response time");

    checkNear(overall.cpuUtilization, 100.0,
              "CPU utilization");

    checkNear(overall.throughput, 3.0 / 16.0,
              "Throughput");
}

// TEST 6: ProcessManager duplicate PID protection
void testProcessManager() {
    ProcessManager manager;

    manager.addProcess(Process(1, 0, 5));

    bool exceptionThrown = false;

    try {
        manager.addProcess(Process(1, 2, 3));
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    check(exceptionThrown, "Duplicate PID is rejected");

    checkEqual(static_cast<int>(manager.getProcessCount()), 1,
               "Duplicate process is not added");
}


void testBasicSJF() {
    std::vector<Process> processes = {
        Process(1, 0, 7),
        Process(2, 2, 4),
        Process(3, 4, 1),
        Process(4, 5, 4)
    };

    SJFScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    checkEqual(static_cast<int>(timeline.size()), 4,
               "SJF creates four execution segments");

    checkEqual(timeline[0].pid, 1, "SJF starts with P1");
    checkEqual(timeline[1].pid, 3, "SJF selects shortest available P3");
    checkEqual(timeline[2].pid, 2, "SJF breaks tie using smaller PID");
    checkEqual(timeline[3].pid, 4, "SJF executes remaining P4");

    checkEqual(timeline[0].startTime, 0, "P1 starts at 0");
    checkEqual(timeline[0].endTime, 7, "P1 ends at 7");
    checkEqual(timeline[3].endTime, 16, "SJF finishes at 16");
}

void testSJFArrivalAndIdleTime() {
    std::vector<Process> processes = {
        Process(1, 3, 5),
        Process(2, 10, 2)
    };

    SJFScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    checkEqual(static_cast<int>(timeline.size()), 4,
               "SJF creates idle and execution segments");

    check(timeline[0].isIdle, "SJF handles initial CPU idle time");
    checkEqual(timeline[0].startTime, 0, "SJF idle starts at 0");
    checkEqual(timeline[0].endTime, 3, "SJF idle ends at 3");

    checkEqual(timeline[1].pid, 1, "P1 executes after arrival");
    check(timeline[2].isIdle, "SJF handles idle time between processes");
    checkEqual(timeline[2].startTime, 8, "Second idle starts at 8");
    checkEqual(timeline[2].endTime, 10, "Second idle ends at 10");
}


void testBasicPriority() {
    std::vector<Process> processes = {
        Process(1, 0, 6, 2),
        Process(2, 1, 3, 1),
        Process(3, 2, 2, 3),
        Process(4, 3, 1, 1)
    };

    PriorityScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    checkEqual(static_cast<int>(timeline.size()), 4,
               "Priority creates four execution segments");

    checkEqual(timeline[0].pid, 1,
               "P1 starts because it is the only available process");

    checkEqual(timeline[1].pid, 2,
               "P2 wins priority tie using smaller PID");

    checkEqual(timeline[2].pid, 4,
               "P4 executes after P2");

    checkEqual(timeline[3].pid, 3,
               "P3 executes last");

    checkEqual(timeline[0].endTime, 6,
               "P1 finishes at 6");

    checkEqual(timeline[3].endTime, 12,
               "Priority scheduling finishes at 12");
}

void testPriorityIdleTime() {
    std::vector<Process> processes = {
        Process(1, 4, 3, 2),
        Process(2, 10, 2, 1)
    };

    PriorityScheduler scheduler;
    auto timeline = scheduler.schedule(processes);

    checkEqual(static_cast<int>(timeline.size()), 4,
               "Priority creates three segments with idle time");

    check(timeline[0].isIdle,
          "Priority handles initial CPU idle time");

    checkEqual(timeline[0].startTime, 0,
               "Priority idle starts at 0");

    checkEqual(timeline[0].endTime, 4,
               "Priority idle ends at 4");

    checkEqual(timeline[1].pid, 1,
               "P1 executes after arrival");

    check(timeline[2].isIdle,
          "Priority handles idle time between processes");
}


int main() {
    std::cout << "\n===== CPU SCHEDULER STUDIO TESTS =====\n\n";

    testBasicFCFS();
    testIdleTime();
    testSimultaneousArrivals();
    testProcessMetrics();
    testOverallMetrics();
    testProcessManager();
    testBasicSJF();
testSJFArrivalAndIdleTime();
testBasicPriority();
testPriorityIdleTime();

    std::cout << "\n===== TEST SUMMARY =====\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";

    if (failed == 0) {
        std::cout << "\nAll tests passed!\n";
        return 0;
    }

    std::cout << "\nSome tests failed. Fix them before proceeding.\n";
    return 1;
}
