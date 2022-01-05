#include "destination_lane.h"

DestinationLane::DestinationLane(int id){
    _id = id;
}
void DestinationLane::enter(Vehicle* veh_p){
    destroy_queue.push(veh_p);
}
void DestinationLane::update(){
    while(!destroy_queue.empty()){
        Vehicle* veh_p = destroy_queue.front();
        delete veh_p;
        destroy_queue.pop();
    }
}