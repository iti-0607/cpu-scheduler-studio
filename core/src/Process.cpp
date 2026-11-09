
#include "Process.h"

#include <stdexcept>

Process::Process(int pid, int arrivalTime, int burstTime, int priority)
    : pid(pid),
      arrivalTime(arrivalTime),
      burstTime(burstTime),
      priority(priority),
      remainingTime(burstTime),
      completionTime(-1),
      firstStartTime(-1),
      state(ProcessState::NEW) {

    if (pid <= 0) {
        throw std::invalid_argument("PID must be positive");
    }

    if (arrivalTime < 0) {
        throw std::invalid_argument("Arrival time cannot be negative");
    }

    if (burstTime <= 0) {
        throw std::invalid_argument("Burst time must be positive");
    }
}

// Getters

int Process::getPID() const {
    return pid;
}

int Process::getArrivalTime() const {
    return arrivalTime;
}

int Process::getBurstTime() const {
    return burstTime;
}

int Process::getPriority() const {
    return priority;
}

int Process::getRemainingTime() const {
    return remainingTime;
}

int Process::getCompletionTime() const {
    return completionTime;
}

int Process::getFirstStartTime() const {
    return firstStartTime;
}

ProcessState Process::getState() const {
    return state;
}

// Runtime updates

void Process::setState(ProcessState newState) {
    state = newState;
}

void Process::setRemainingTime(int time) {
    if (time < 0 || time > burstTime) {
        throw std::invalid_argument("Invalid remaining time");
    }

    remainingTime = time;
}

void Process::markFirstStart(int currentTime) {
    if (firstStartTime == -1) {
        firstStartTime = currentTime;
    }
}

void Process::markCompleted(int currentTime) {
    completionTime = currentTime;
    remainingTime = 0;
    state = ProcessState::TERMINATED;
}
