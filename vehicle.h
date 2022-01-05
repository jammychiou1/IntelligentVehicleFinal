#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

class Scenario;

enum class RouteNodeType {
    intersection, lane
};

struct RouteNode {
    RouteNodeType type;
    int id;
};

class Vehicle {
	public:
		Vehicle(std::vector<RouteNode> route, Scenario *scenario);
        void go_next();
		void update();
	private:
		std::vector<RouteNode> _route;
		bool _should_go_next;
		int _now_location;
        Scenario *_scenario;
};

#endif
