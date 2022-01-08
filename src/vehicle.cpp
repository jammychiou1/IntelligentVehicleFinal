#include "vehicle.h"

#include "intersection_controller.h"
#include "lane_controller.h"
#include "scenario.h"
#include "simulator.h"

using std::vector;

Vehicle::Vehicle(vector<RouteNode> route, Scenario *scenario, Simulator *simulator) {
	_route = route;
	_should_go_next = false;
	_now_location = 0;
    _scenario = scenario;
    _simulator = simulator;
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
                _simulator->tell_enter_intersection(this, node.id, _route[_now_location - 1].id, _route[_now_location + 1].id);
                //_scenario->intersections[node.id].enter(this, _route[_now_location - 1].id, _route[_now_location + 1].id);
                std::cout << std::hex << this << std::dec << " go to intersection " << node.id << " in lane " << _route[_now_location - 1].id << " out lane " << _route[_now_location + 1].id << '\n';
			}
			else {
                _simulator->tell_enter_lane(this, node.id);
                //_scenario->lane_controllers[node.id].enter(this);
                std::cout << std::hex << this << std::dec << " go to lane " << node.id << '\n';
			}
		}
       _should_go_next = false;
	}
}
