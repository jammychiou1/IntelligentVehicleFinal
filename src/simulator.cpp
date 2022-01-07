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
int Simulator::get_time() {
    return _time;
}

void Simulator::start_simulation() {
    while (true) {
        // updates
        for (Vehicle* veh_p : _vehicles) veh_p->update();
        for (auto p : _intersections) p.second.update();
        for (auto p : _lane_controllers) p.second.update();
        for (auto p : _source_lanes) p.second.update();
        for (auto p : _destination_lanes) p.second.update();
        // flush buffer
        for (auto p : _enter_intsersection_infos) {
            _intersections[p.intersection_id].enter(p.veh_p, p.in_lane_id, p.out_lane_id);
        }
        _enter_intsersection_infos.clear();
        for (auto p : _enter_lane_infos) {
            _lane_controllers[p.lane_id].enter(p.veh_p);
        }
        _enter_lane_infos.clear();
        for (Vehicle* veh_p : _go_next_info) {
            veh_p->go_next();
        }
        _go_next_info.clear();
        for (Vehicle* veh_p : _generate_buffer) {
            _vehicles.insert(veh_p);
        }
        _generate_buffer.clear();
        for (Vehicle* veh_p : _remove_buffer) {
            delete veh_p;
        }
        _remove_buffer.clear();
        _time++;
    }
}
