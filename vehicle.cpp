#include "vehicle.h"

#include "intersection_controller.h"
#include "lane_controller.h"
#include "scenario.h"

using std::vector;

Vehicle::Vehicle(vector<RouteNode> route, Scenario *scenario) {
	_route = route;
	_should_go_next = false;
	_now_location = 0;
    _scenario = scenario;
}
void Vehicle::go_next() {
    _should_go_next = true;
}
void Vehicle::update() {
	if (_should_go_next) {
		_now_location++;
		if (_now_location < (int)_route.size()) {
            RouteNode node = _route[_now_location];
			if (node.type == RouteNodeType::intersection) {
                _scenario->intersections[node.id].enter(this, _route[_now_location - 1].id, _route[_now_location + 1].id);
			}
			else {
                _scenario->lanes[node.id].enter(this);
			}
		}
	}
}
