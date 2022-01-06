#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <set>

#include "intersection_controller.h"
#include "lane_controller.h"
#include "vehicle.h"

class Scenario {
    public:
        //std::map<int, IntersectionController> intersections;
        //std::map<int, LaneController> lane_controllers;
        //std::set<Vehicle*> vehicles;
        std::map<int, std::pair<int,int>> lanes;
        std::vector<int> dst_lanes_id;
        std::map<int, std::map<int, std::vector<int>>> connectivity;
};

#endif
