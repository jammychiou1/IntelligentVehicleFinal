#include "vehicle.h"
#include "lane_controller.h"

LaneController::LaneController(int* time_p, int waiting_time){
	this._time_p = time_p;
	this._waiting_time = waiting_time;
}
void LaneController::enter(Vehicle veh_p){
	_Veh_Time tmp;
	tmp.veh_p = veh_p;
	tmp.in_time = *(this._time_p);
	this.queue.push_back(tmp);
}
void LaneController::update(){
	while(this.queue.size() > 0 && this.queue.front().in_time + this._waiting_time <= this._time_p){
		Vehicle* veh_p = tis.queue.pop_front();
		veh_p->go_next = true;
	}
}