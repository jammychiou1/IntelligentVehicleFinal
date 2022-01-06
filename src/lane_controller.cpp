#include <cassert>

#include "lane_controller.h"

#include "vehicle.h"
#include "simulator.h"

LaneController::LaneController(int id, int waiting_time, Simulator* simulator_p) {
	//id = id;
    //assert(time_p != nullptr);
    _simulator_p = simulator_p;
	_waiting_time = waiting_time;
}

void LaneController::enter(Vehicle *veh_p) {
	_VehTime tmp;
	tmp.veh_p = veh_p;
	tmp.in_time = *_time_p;
	queue.push_back(tmp);
}

void LaneController::update(){
	while (queue.size() > 0 && queue.front().in_time + _waiting_time <= *_time_p) {
		Vehicle* veh_p = queue.front().veh_p;
        queue.pop_front();
        _simulator_p->tell_go_next(veh_p);
		//veh_p->go_next();
	}
}
