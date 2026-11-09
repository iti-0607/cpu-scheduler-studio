
#include "ProcessManager.h"

#include <algorithm>
#include <stdexcept>

void ProcessManager::addProcess(const Process& process) {

    if (containsProcess(process.getPID())) {
        throw std::invalid_argument("Duplicate Process ID");
    }

    processes.push_back(process);
}

Process* ProcessManager::getProcessById(int pid) {

    for (Process& process : processes) {
        if (process.getPID() == pid) {
            return &process;
        }
    }

    return nullptr;
}

const Process* ProcessManager::getProcessById(int pid) const {

    for (const Process& process : processes) {
        if (process.getPID() == pid) {
            return &process;
        }
    }

    return nullptr;
}

bool ProcessManager::removeProcess(int pid) {

    auto it = std::remove_if(
        processes.begin(),
        processes.end(),
        [pid](const Process& process) {
            return process.getPID() == pid;
        }
    );

    if (it == processes.end()) {
        return false;
    }

    processes.erase(it, processes.end());
    return true;
}

bool ProcessManager::containsProcess(int pid) const {

    return getProcessById(pid) != nullptr;
}

const std::vector<Process>&
ProcessManager::getAllProcesses() const {

    return processes;
}

size_t ProcessManager::getProcessCount() const {

    return processes.size();
}
