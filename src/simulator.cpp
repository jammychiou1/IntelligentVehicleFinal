#include "simulator.h"

#include "vehicle.h"
#include "scenario.h"

Simulator::Simulator(Scenario* scenario_p) {
    _scenario_p = scenario_p;
}

void Simulator::tell_enter_intersection(Vehicle* veh_p, int intersection_id, int in_lane_id, int out_lane_id) {
    _EnterIntersectionInfo info;
    info.veh_p = veh_p;
    info.intersection_id = intersection_id;
    info.in_lane_id = in_lane_id;
    info.out_lane_id = out_lane_id;
    _enter_intsersection_infos.push_back(info);
}
void Simulator::tell_enter_lane(Vehicle* veh_p, int lane_id) {
    _EnterLaneInfo info;
    info.veh_p = veh_p;
    info.lane_id = lane_id;
    _enter_lane_infos.push_back(info);
}
void Simulator::tell_go_next(Vehicle* veh_p) {
    _go_next_info.push_back(veh_p);
}
Vehicle* Simulator::generate_vehicle(std::vector<RouteNode> route) {
    Vehicle *veh_p = new Vehicle(route, _scenario_p);
    _generate_buffer.push_back(veh_p);
    return veh_p;
}
void Simulator::remove_vehicle(Vehicle* veh_p) {
    _remove_buffer.push_back(veh_p);
}
void Simulator::start_simulation() {
    while (true) {
        // updates
        // flush buffer
    }
}
