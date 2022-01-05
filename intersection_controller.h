#ifndef INTERSECTION_CONTROLLER_H
#define INTERSECTION_CONTROLLER_H

#include <vector>

#include "graph.h"

class Vehicle;

class IntersectionController {
    public:
        IntersectionController(int id = 0, int *time_p = nullptr);
		void enter(Vehicle *veh_p, int in_lane_id, int out_lane_id);
		void update();
    private:
        struct _VehState {
            //Vehicle *veh_p;
        };
        Graph _graph;
        std::map<Vehicle*, _VehState> _vehicle_states;
        int *_time_p;
};

#endif
