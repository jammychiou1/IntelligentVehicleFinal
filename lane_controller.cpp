#include "lane_controller.h"

LaneController::LaneController(int* time_p, int waiting_time){
	this._time_p = time_p;
	this._waiting_time = waiting_time;
}
void LaneController::enter(Vehicle veh){
	_Veh_Time tmp;
	tmp.veh = veh;
	tmp.in_time = *(this._time_p);
	this.queue.push_back(tmp);
}
void LaneController::update(){
	while(this.queue.size() > 0 && this.queue.front().in_time + this._waiting_time <= this._time_p){
		Vehicle veh = tis.queue.pop_front();
		veh.go_next = true;
	}
}