#ifndef LANE_CONTROLLER_H
#define LANE_CONTROLLER_H

#include <queue>

class Vehcle;

class LaneController{
	public:
		LaneController(int* time_p);
		void enter(Vehicle veh);
		void update();
	private:
		struct _Veh_Time{
			Vehicle veh;
			int in_time;
		};
		std::queue<_Veh_Time> queue;
		int *time, waiting_time;
};

#endif