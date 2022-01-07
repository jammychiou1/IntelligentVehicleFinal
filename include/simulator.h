#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <set>
#include <vector>
#include <map>

#include "route.h"
#include "intersection_controller.h"
#include "lane_controller.h"
#include "source_lane.h"
#include "destination_lane.h"

class Vehicle;
class Scenario;

class Simulator {
    public:
        Simulator(Scenario* scenario_p = nullptr);
        void tell_enter_intersection(Vehicle* veh_p, int intersection_id, int in_lane_id, int out_lane_id);
        void tell_enter_lane(Vehicle* veh_p, int lane_id);
        void tell_go_next(Vehicle* veh_p);
        Vehicle* generate_vehicle(std::vector<RouteNode> route);
        void remove_vehicle(Vehicle* veh_p);
        int get_time();
        void start_simulation();
    private:
        struct _EnterIntersectionInfo {
            Vehicle* veh_p;
            int intersection_id;
            int in_lane_id;
            int out_lane_id;
        };
        struct _EnterLaneInfo {
            Vehicle* veh_p;
            int lane_id;
        };

        std::set<Vehicle*> _vehicles;
        std::map<int, IntersectionController> _intersections;
        std::map<int, LaneController> _lane_controllers; 
        std::map<int, SourceLane> _source_lanes;
        std::map<int, DestinationLane> _destination_lanes;
        Scenario* _scenario_p;
        std::vector<_EnterIntersectionInfo> _enter_intsersection_infos;
        std::vector<_EnterLaneInfo> _enter_lane_infos;
        std::vector<Vehicle*> _go_next_info;
        std::vector<Vehicle*> _generate_buffer;
        std::vector<Vehicle*> _remove_buffer;
        int _time = 0;
};

#endif
