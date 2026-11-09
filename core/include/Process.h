
#pragma once

#include <string>

enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

class Process {
private:
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;

    int remainingTime;
    int completionTime;
    int firstStartTime;

    ProcessState state;

public:
    Process(int pid, int arrivalTime, int burstTime, int priority = 0);

    // Getters
    int getPID() const;
    int getArrivalTime() const;
    int getBurstTime() const;
    int getPriority() const;
    int getRemainingTime() const;
    int getCompletionTime() const;
    int getFirstStartTime() const;

    ProcessState getState() const;

    // Runtime updates
    void setState(ProcessState newState);
    void setRemainingTime(int time);
    void markFirstStart(int currentTime);
    void markCompleted(int currentTime);
};
