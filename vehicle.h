#ifndef VEHICLE_H
#define VEHICLE_H

#include <variant>
#include <vector>
#include "intersection_controller.h"
#include "lane_controller.h"

class Vehicle{
	public:
		Vehicle(std::vector<std::variant<IntersectionController*, LaneController*>> route);
		bool go_next;
		void update();
	private:
		std::vector<std::variant<IntersectionController*, LaneController*>> _route;
		int now_location;
};

#endif