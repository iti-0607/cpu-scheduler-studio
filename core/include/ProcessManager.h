
#pragma once

#include "Process.h"
#include <vector>

class ProcessManager {
private:
    std::vector<Process> processes;

public:
    // Add a process
    void addProcess(const Process& process);

    // Search for a process using PID
    Process* getProcessById(int pid);
    const Process* getProcessById(int pid) const;

    // Remove a process
    bool removeProcess(int pid);

    // Check whether a PID exists
    bool containsProcess(int pid) const;

    // Get all processes
    const std::vector<Process>& getAllProcesses() const;

    // Get total number of processes
    size_t getProcessCount() const;
};
