#include <vector>

#include "vehicle.h"
#include "intersection_controller.h"
#include "lane_controller.h"
#include "scenario.h"

using std::vector;
int main() {
    Scenario scenario;
    vector<RouteNode> route;
    Vehicle(route, &scenario);
    int time;
    IntersectionController intersection(0, {}, {}, nullptr);
    LaneController lane(0, 5, nullptr);
}
