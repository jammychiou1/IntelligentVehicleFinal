#include "vehicle.h"

Vehicle::Vehicle(std::vector<std::variant<IntersectionController*, LaneController*>> route){
	this->_route = route;
	this->go_next = false;
	this->now_location = 0;
}
void Vehicle::update(){
	if(this->go_next){
		this->now_location++;
		if(this->now_location < this->_route.size()){
			if(this->_route[now_location].index() == 0){
				std::get<IntersectionController*>(this->_route[now_location])->enter(this);
			}
			else if(this->_route[now_location].index() == 1){
				std::get<LaneController*>(this->_route[now_location])->enter(this);	
			}
		}
	}
}