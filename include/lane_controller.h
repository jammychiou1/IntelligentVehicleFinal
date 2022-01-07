#ifndef LANE_CONTROLLER_H
#define LANE_CONTROLLER_H

#include <queue>

class Vehicle;
class Simulator;

class LaneController{
	public:
		LaneController(int id = 0, int waiting_time = 0, Simulator* simulator_p = nullptr);
		void enter(Vehicle *veh_p);
		void update();
		//int id;
	private:
		struct _VehTime{
			Vehicle *veh_p;
			int in_time;
		};
		std::deque<_VehTime> queue;
        int _waiting_time;
        Simulator *_simulator_p;
};

#endif
