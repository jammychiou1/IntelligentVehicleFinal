#include <cassert>
#include <iostream>
#include "lane_controller.h"

#include "vehicle.h"
#include "simulator.h"

LaneController::LaneController(int id, int waiting_time, Simulator* simulator_p) {
	_id = id;
    //assert(time_p != nullptr);
    _simulator_p = simulator_p;
	_waiting_time = waiting_time;
}

void LaneController::enter(Vehicle *veh_p) {
	_VehTime tmp;
	tmp.veh_p = veh_p;
	tmp.in_time = _simulator_p->get_time();
	queue.push_back(tmp);
	printf("vehicle %d enter lane %d at time %d and will leave at time %d\n", veh_p->get_id(), _id, _simulator_p->get_time(), _simulator_p->get_time() + _waiting_time);
}

void LaneController::update(){
	while (queue.size() > 0 && queue.front().in_time + _waiting_time <= _simulator_p->get_time()) {
		Vehicle* veh_p = queue.front().veh_p;
        queue.pop_front();
        _simulator_p->tell_go_next(veh_p);
		//veh_p->go_next();
	}
}
