#pragma once

#include "SimulationResult.h"
#include "Scheduler.h"

#include <vector>

class SimulationEngine {
public:
    SimulationResult run(
        const Scheduler& scheduler,
        const std::vector<Process>& processes
    ) const;
};