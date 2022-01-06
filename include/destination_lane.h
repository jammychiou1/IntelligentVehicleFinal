#ifndef DESTINATION_LANE_H
#define DESTINATION_LANE_H

#include <queue>

class Vehicle;
class Scenario;
class Simulator;

class DestinationLane{
    public:
        DestinationLane(int id = 0, Scenario* scenario_p = nullptr, Simulator* simulator_p = nullptr);
        void enter(Vehicle* veh_p);
        void update();
    private:
        int _id;
        std::queue<Vehicle*> _destroy_queue;
        Scenario *_scenario_p;
        Simulator *_simulator_p;
};

#endif
