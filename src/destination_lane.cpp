#include "destination_lane.h"

#include "vehicle.h"
#include "scenario.h"
#include "simulator.h"

DestinationLane::DestinationLane(int id, Scenario* scenario_p, Simulator* simulator_p){
    _id = id;
    _scenario_p = scenario_p;
    _simulator_p = simulator_p;
}
void DestinationLane::enter(Vehicle* veh_p){
    _destroy_queue.push(veh_p);
}
void DestinationLane::update(){
    while(!_destroy_queue.empty()){
        Vehicle* veh_p = _destroy_queue.front();
        _simulator_p->remove_vehicle(veh_p);
        //_scenario_p->vehicles.erase(veh_p);
        //delete veh_p;
        _destroy_queue.pop();
    }
}
