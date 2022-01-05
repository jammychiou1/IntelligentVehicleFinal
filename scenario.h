#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>

#include "intersection_controller.h"
#include "lane_controller.h"

class Scenario {
    public:
        std::map<int, IntersectionController> intersections;
        std::map<int, LaneController> lanes;
};

#endif
