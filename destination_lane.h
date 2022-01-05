#ifndef DESTINATION_LANE_H
#define DESTINATION_LANE_H

#include <queue>

#include "vehicle.h"

class DestinationLane{
    public:
        DestinationLane(int id = 0);
        void enter(Vehicle* veh_p);
        void update();
    private:
        int _id;
        std::queue<Vehicle*> destroy_queue;
};

#endif