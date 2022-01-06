#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "route.h"

class Scenario;
class Simulator;

class Vehicle {
	public:
		Vehicle(std::vector<RouteNode> route = {}, Scenario *scenario = nullptr, Simulator *simulator = nullptr);
        //void go_next();
		void update();
	private:
		std::vector<RouteNode> _route;
		bool _should_go_next;
		int _now_location;
        Scenario *_scenario;
        Simulator *_simulator;
};

#endif
