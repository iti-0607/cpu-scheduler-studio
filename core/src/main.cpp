
#include <iostream>
#include "MetricsCalculator.h"
#include <iomanip>
#include "Process.h"
#include "ProcessManager.h"
#include "FCFS.h"
#include "Priority.h"
#include "SJF.h"
#include "RoundRobin.h"
#include "SRTF.h"

int main() {

    ProcessManager manager;

    manager.addProcess(Process(1, 0, 5, 2));
    manager.addProcess(Process(2, 1, 3, 1));
    manager.addProcess(Process(3, 2, 8, 3));

    FCFSScheduler fcfs;

    std::vector<ExecutionSegment> timeline =
        fcfs.schedule(manager.getAllProcesses());

    std::cout << "\n===== CPU SCHEDULER STUDIO =====\n";
    std::cout << "Scheduling Algorithm: FCFS\n\n";

    std::cout << "GANTT CHART\n";

    for (const ExecutionSegment& segment : timeline) {

        if (segment.isIdle) {
            std::cout << "| IDLE ";
        }
        else {
            std::cout << "| P" << segment.pid << " ";
        }
    }

    std::cout << "|\n";

    std::cout << "\nEXECUTION DETAILS\n";
    std::cout << "PID\tStart\tEnd\n";

    for (const ExecutionSegment& segment : timeline) {

        if (segment.isIdle) {
            std::cout << "IDLE\t";
        }
        else {
            std::cout << "P" << segment.pid << "\t";
        }

        std::cout << segment.startTime << "\t"
                  << segment.endTime << "\n";
    }

    std::cout << "\nTotal execution time: "
              << (timeline.empty() ? 0 : timeline.back().endTime)
              << "\n";

              // Calculate individual process metrics
              std::vector<ProcessMetrics> processMetrics =
                  MetricsCalculator::calculateProcessMetrics(
                      manager.getAllProcesses(),
                      timeline
                  );
              
              // Calculate overall metrics
              OverallMetrics overall =
                  MetricsCalculator::calculateOverallMetrics(
                      processMetrics,
                      timeline
                  );
              
              std::cout << "\nPROCESS PERFORMANCE METRICS\n";
              
              std::cout << "PID\tCT\tTAT\tWT\tRT\n";
              
              for (const ProcessMetrics& metric : processMetrics) {
                  std::cout << "P" << metric.pid << "\t"
                            << metric.completionTime << "\t"
                            << metric.turnaroundTime << "\t"
                            << metric.waitingTime << "\t"
                            << metric.responseTime << "\n";
              }
              
              std::cout << "\nOVERALL PERFORMANCE\n";
              
              std::cout << std::fixed << std::setprecision(2);
              
              std::cout << "Average Waiting Time: "
                        << overall.averageWaitingTime << "\n";
              
              std::cout << "Average Turnaround Time: "
                        << overall.averageTurnaroundTime << "\n";
              
              std::cout << "Average Response Time: "
                        << overall.averageResponseTime << "\n";
              
              std::cout << "CPU Utilization: "
                        << overall.cpuUtilization << "%\n";
              
              std::cout << "Throughput: "
                        << overall.throughput
                        << " processes per time unit\n";


                        // Run SJF scheduling
                        SJFScheduler sjf;
                        
                        std::vector<ExecutionSegment> sjfTimeline =
                            sjf.schedule(manager.getAllProcesses());
                        
                        std::cout << "\n\n===== SJF SCHEDULING =====\n";
                        
                        for (const ExecutionSegment& segment : sjfTimeline) {
                            if (segment.isIdle) {
                                std::cout << "| IDLE ";
                            } else {
                                std::cout << "| P" << segment.pid << " ";
                            }
                        }
                        
                        std::cout << "|\n";
                        
                        // Run Priority scheduling
                        PriorityScheduler priorityScheduler;
                        
                        std::vector<ExecutionSegment> priorityTimeline =
                            priorityScheduler.schedule(manager.getAllProcesses());
                        
                        std::cout << "\n\n===== PRIORITY SCHEDULING =====\n";
                        
                        for (const ExecutionSegment& segment : priorityTimeline) {
                            if (segment.isIdle) {
                                std::cout << "| IDLE ";
                            } else {
                                std::cout << "| P" << segment.pid << " ";
                            }
                        }
                        
                        std::cout << "|\n";

                        // Run Round Robin scheduling with quantum = 2
                        RoundRobinScheduler rrScheduler(2);
                        
                        std::vector<ExecutionSegment> rrTimeline =
                            rrScheduler.schedule(manager.getAllProcesses());
                        
                        std::cout << "\n\n===== ROUND ROBIN SCHEDULING =====\n";
                        std::cout << "Time Quantum: 2\n";
                        
                        for (const ExecutionSegment& segment : rrTimeline) {
                            if (segment.isIdle) {
                                std::cout << "| IDLE ";
                            } else {
                                std::cout << "| P" << segment.pid << " ";
                            }
                        }
                        
                        std::cout << "|\n";
                        // Run SRTF scheduling
                        SRTFScheduler srtfScheduler;
                        
                        std::vector<ExecutionSegment> srtfTimeline =
                            srtfScheduler.schedule(manager.getAllProcesses());
                        
                        std::cout << "\n\n===== SRTF SCHEDULING =====\n";
                        
                        for (const ExecutionSegment& segment : srtfTimeline) {
                            if (segment.isIdle) {
                                std::cout << "| IDLE ";
                            } else {
                                std::cout << "| P" << segment.pid << " ";
                            }
                        }
                        
                        std::cout << "|\n";
    return 0;
}
