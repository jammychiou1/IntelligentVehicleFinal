#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <set>
#include <vector>
#include <map>
#include <iostream> 
#include <random>

#include "route.h"
#include "intersection_controller.h"
#include "lane_controller.h"
#include "source_lane.h"
#include "destination_lane.h"
#include "scenario.h"

class Vehicle;

class Simulator {
    public:
        Simulator(std::istream& is);
        void tell_enter_intersection(Vehicle* veh_p, int intersection_id, int in_lane_id, int out_lane_id);
        void tell_enter_lane(Vehicle* veh_p, int lane_id);
        void tell_go_next(Vehicle* veh_p);
        Vehicle* generate_vehicle(std::vector<RouteNode> route, int src_lane_id);
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
        int _lane_id_counter = 0, _intersection_id_counter = 0, _vehicle_id_counter = 0;
        std::default_random_engine _gen;
        Scenario _scenario;
        std::set<Vehicle*> _vehicles;
        std::map<int, IntersectionController> _intersections;
        std::map<int, LaneController> _lane_controllers; 
        std::map<int, SourceLane> _source_lanes;
        std::map<int, DestinationLane> _destination_lanes;
        std::vector<_EnterIntersectionInfo> _enter_intsersection_infos;
        std::vector<_EnterLaneInfo> _enter_lane_infos;
        std::vector<Vehicle*> _go_next_info;
        std::vector<Vehicle*> _generate_buffer;
        std::vector<Vehicle*> _remove_buffer;
        int _time = 0;
};

#endif
