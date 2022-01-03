#ifndef LANE_CONTROLLER_H
#define LANE_CONTROLLER_H

#include <queue>

class Vehicle;

class LaneController{
	public:
		LaneController(int* time_p);
		void enter(Vehicle* veh_p);
		void update();
	private:
		struct _Veh_Time{
			Vehicle* veh_p;
			int in_time;
		};
		std::queue<_Veh_Time> queue;
		int *time, waiting_time;
};

#endif