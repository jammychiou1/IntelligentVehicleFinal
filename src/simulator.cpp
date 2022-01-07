#include "simulator.h"

#include "vehicle.h"
#include "scenario.h"

Simulator::Simulator(std::istream &is) {
    int n, m;
    double prob;
    is >> n >> m >> prob;
    for (int i = 0; i < m; i++) {
        int id, src, dst, waiting_time;
        is >> id >> src >> dst >> waiting_time;
        _scenario.lanes[id] = std::pair<int,int>(src, dst);
        if (src == -1) {
            SourceLane source_lane(_lane_id_counter, prob, &_scenario, this, &_gen);
            _source_lanes[_lane_id_counter] = source_lane;
        }
        else if (dst == -1) {
            _scenario.dst_lanes_id.push_back(dst);
            DestinationLane dst_lane(_lane_id_counter, &_scenario, this);
            _destination_lanes[_lane_id_counter] = dst_lane;
        }
        else {
            LaneController lane_controller(_lane_id_counter, waiting_time, this);
            _lane_controllers[_lane_id_counter] = lane_controller;
        }
        _lane_id_counter++;
    }
    for (int i = 0; i < n; i++) {
        int n_zone, n_in_lanes, intersection_id;
        std::map<int, std::vector<int>> connected;
        std::vector<int> total_conflict_zone_ids;
        std::map<int, std::vector<TrajectoryDesc>> in_lane_to_trajectories;
        is >> intersection_id >> n_zone;
        for (int j = 0; j < n_zone; j++) {
            int id;
            double x, y;
            is >> id >> x >> y;
            total_conflict_zone_ids.push_back(id);
        }
        is >> n_in_lanes;
        for (int j = 0; j < n_in_lanes; j++) {
            int in_lane_id, n_trajectory;
            std::vector<int> out_lanes_id;
            std::vector<TrajectoryDesc> trajectories;
            is >> in_lane_id >> n_trajectory;
            for (int k = 0; k < n_trajectory; k++) {
                TrajectoryDesc trajectory;
                std::vector<int> conflict_zone_ids, travel_times;
                int out_lane_id, length;
                is >> out_lane_id >> length;
                out_lanes_id.push_back(out_lane_id);
                trajectory.out_lane_id = out_lane_id;
                for (int l = 0; l < length; l++) {
                    int id;
                    is >> id;
                    conflict_zone_ids.push_back(id);
                }
                for(int l = 0; l <= length; l++) {
                    int t;
                    is >> t;
                    travel_times.push_back(t);
                }
                trajectory.conflict_zone_ids = conflict_zone_ids;
                trajectory.travel_times = travel_times;
                trajectories.push_back(trajectory);
            }
            connected[in_lane_id] = out_lanes_id;
            in_lane_to_trajectories[in_lane_id] = trajectories;
        }
        _scenario.connectivity[_intersection_id_counter] = connected;
        IntersectionController intersection_controller(_intersection_id_counter, total_conflict_zone_ids, in_lane_to_trajectories, this);
        _intersection_id_counter++;
    }   
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
    Vehicle *veh_p = new Vehicle(route, &_scenario);
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
