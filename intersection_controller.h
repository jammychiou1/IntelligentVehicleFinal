#ifndef INTERSECTION_CONTROLLER_H
#define INTERSECTION_CONTROLLER_H

#include <vector>
#include <queue>
#include <utility>

#include "graph.h"

class Vehicle;

struct TrajectoryDesc {
    //int in_lane_id;
    int out_lane_id;
    std::vector<int> conflict_zone_ids;
};

class IntersectionController {
    public:
        IntersectionController(int id = 0, std::vector<int> conflict_zone_ids = {}, std::map<int, std::vector<TrajectoryDesc>> in_lane_to_trajectories = {}, int *time_p = nullptr);
		void enter(Vehicle *veh_p, int in_lane_id, int out_lane_id);
		void update();
    private:
        struct _VehState {
            //Vehicle *veh_p;
            int in_lane_id;
            int out_lane_id;
        };
        struct _Trajectory {
            //int in_lane_id;
            //int out_lane_id;
            std::vector<int> conflict_zone_ids;
            std::deque<Vehicle*> moving_vehicles;
        };
        struct _InLane {
            int in_lane_id;
            std::deque<Vehicle*> queued_vehicles;
            std::map<int, _Trajectory> trajectories;
        };
        struct _ConflictZone {
            std::vector<std::pair<int, int>> trajectories_over;
        };
        Graph _graph;
        std::map<Vehicle*, _VehState> _vehicle_states;
        std::map<int, _InLane> _in_lanes;
        std::map<int, _ConflictZone> _conflict_zones;
        int *_time_p;
};

#endif
