#include <cassert>

#include "intersection_controller.h"

#include "vehicle.h"
#include "simulator.h"

using std::pair, std::vector, std::map, std::deque;
using std::make_pair;

IntersectionController::IntersectionController(int id, vector<int> conflict_zone_ids, map<int, vector<TrajectoryDesc>> in_lane_to_trajectories, Simulator* simulator_p) {
    _id = id;
    //assert(time_p != nullptr);
    _simulator_p = simulator_p;
    //for (int czid : conflict_zone_ids) {
    //    _conflict_zones[czid] = {};
    //}
    for (auto &p : in_lane_to_trajectories) {
        int in_lane_id = p.first;
        vector<TrajectoryDesc> &trajs = p.second;

        _InLane in_lane;
        in_lane.in_lane_id = in_lane_id;
        in_lane.queued_vehicles = {};
        in_lane.trajectories = {};
        //map<int, _Trajectory> out_lanes_to_traj;
        for (TrajectoryDesc &traj : trajs) {
            //for (int czid : traj.conflict_zone_ids) {
            //    assert(_conflict_zones.count(czid));
            //    _conflict_zones[czid].trajectories_over.push_back(make_pair(in_lane_id, traj.out_lane_id));
            //}

            _Trajectory new_traj;
            //new_traj.in_lane_id = traj.in_lane_id;
            //new_traj.out_lane_id = traj.out_lane_id;
            new_traj.conflict_zone_ids = traj.conflict_zone_ids;
            new_traj.travel_times = traj.travel_times;
            assert(!in_lane.trajectories.count(traj.out_lane_id));
            in_lane.trajectories[traj.out_lane_id] = new_traj;
        }

        assert(!_in_lanes.count(in_lane_id));
        _in_lanes[in_lane_id] = in_lane;
    }
}

void IntersectionController::enter(Vehicle *veh_p, int in_lane_id, int out_lane_id) {
    assert(_in_lanes.count(in_lane_id));
    _InLane &in_lane = _in_lanes[in_lane_id];
    in_lane.queued_vehicles.push_back(veh_p);

    assert(in_lane.trajectories.count(out_lane_id));
    //_Trajectory &traj = in_lane.trajectories[out_lane_id];

    assert(!_vehicle_states.count(veh_p));
    _VehState state;
    state.in_lane_id = in_lane_id;
    state.out_lane_id = out_lane_id;
    state.actions = {};
    state.added = false;
    state.exit_intersection_time = -1;
    _vehicle_states[veh_p] = state;
}

void IntersectionController::update() {
    int time_now = _simulator_p->get_time();
    // add vehicle
    for (auto &p : _in_lanes) {
        _InLane &in_lane = p.second;
        if (in_lane.queued_vehicles.empty()) {
            continue;
        }
        Vehicle *veh_p = in_lane.queued_vehicles.front();
        assert(_vehicle_states.count(veh_p));
        _VehState &state = _vehicle_states[veh_p];
        if (!state.added) {
            // add to graph
            assert(in_lane.trajectories.count(state.out_lane_id));
            vector<int> &czids = in_lane.trajectories[state.out_lane_id].conflict_zone_ids;
            vector<int> &trvl_times = in_lane.trajectories[state.out_lane_id].travel_times;
            auto [path_id, node_ids] =_graph.add_path(trvl_times, time_now + 1);

            state.path_id = path_id;
            deque<_VehAction> tmp_actions;
            for (int i = 0; i <= (int) czids.size(); i++) {
                _VehAction action;
                action.from = i == 0 ? -1 : czids[i - 1];
                action.to = i == (int) czids.size() ? -1 : czids[i];
                action.frozen = false;
                action.node_id = node_ids[i];
                action.travel_time = trvl_times[i];
                tmp_actions.push_back(action);
            }
            //add type 2
            //for (int i = 0; i < (int) tmp_actions.size() - 1; i++) {
            //    _VehAction action = tmp_actions[i];
            //    for (auto &q : _vehicle_states) {
            //        _VehState &other_state = q.second;
            //        for (int j = 0; j < (int) other_state.actions.size() - 1; j++) {
            //            _VehAction other_action = other_state.actions[j];
            //            _VehAction other_next_action = other_state.actions[j + 1];
            //            if (action.to == other_action.from) {
            //                assert(action.to != -1);
            //                if (other_state.in_lane_id == state.in_lane_id || other_action.frozen /* || j == 0 */) {
            //                    _graph.add_type2_edge(other_action.node_id, action.node_id);
            //                }
            //            }
            //        }
            //    }
            //}
            //add type 3
            for (int i = 0; i < (int) tmp_actions.size() - 1; i++) {
                _VehAction action = tmp_actions[i];
                for (auto &q : _vehicle_states) {
                    _VehState &other_state = q.second;
                    for (int j = 0; j < (int) other_state.actions.size() - 1; j++) {
                        _VehAction other_action = other_state.actions[j];
                        if (action.to == other_action.to) {
                            assert(action.to != -1);
                            _VehAction next_action = tmp_actions[i + 1];
                            _VehAction other_next_action = other_state.actions[j + 1];

                            if (other_state.in_lane_id == state.in_lane_id || other_action.frozen) {
                                _graph.add_type2_edge(other_next_action.node_id, action.node_id);
                            }
                            else {
                                _graph.add_type3_pair(other_next_action.node_id, action.node_id, next_action.node_id, other_action.node_id);
                            }
                        }
                    }
                }
            }
            state.actions = tmp_actions;
            state.added = true;
        }
    }
    // schedule new vehicle
    _graph.update_time_now(time_now + 1);
    _graph.optimize();
    assert(_graph.calc_time());
    _graph.commit_used();
    // freeze near future actions
    // anounce about-to-happen actions and remove
    vector<Vehicle*> remove_buffer;
    for (auto &p : _vehicle_states) {
        Vehicle *veh_p = p.first;
        _VehState &state = p.second;
        if (!state.added) {
            continue;
        }
        //freeze
        for (_VehAction &action : state.actions) {
            if (action.frozen) {
                continue;
            }
            int start_time = _graph.get_time(action.node_id);
            if (start_time < time_now + 5) {
                _graph.fix_node_time(action.node_id);
                action.frozen = true;
                action.start_time = start_time;
                action.end_time = start_time + action.travel_time;
            }
            else {
                break;
            }
        }
        //anounce
        for (_VehAction action : state.actions) {
            if (action.frozen) {
                //assert(action.start_time >= time_now + 1);
                if (action.start_time == time_now + 1) {
                    std::string from_str, to_str;
                    if (action.from == -1) {
                        from_str = "lane " + std::to_string(state.in_lane_id);
                    }
                    else {
                        from_str = "conflict zone " + std::to_string(action.from);
                    }
                    if (action.to == -1) {
                        to_str = "lane " + std::to_string(state.out_lane_id);
                    }
                    else {
                        to_str = "conflict zone " + std::to_string(action.to);
                    }
                    std::cout << "move vehicle " <<  veh_p->get_id() << " in intersection " << _id << " from " << from_str << " to " << to_str << " in time [" << action.start_time << ", " << action.end_time << "]\n";
                    if (action.from == -1) {
                        assert(_in_lanes[state.in_lane_id].queued_vehicles.front() == veh_p);
                        _in_lanes[state.in_lane_id].queued_vehicles.pop_front();
                    }
                    if (action.to == -1) {
                        state.exit_intersection_time = action.end_time;
                    }
                }
            }
        }
        //remove
        if (state.exit_intersection_time == time_now + 1 && state.exit_intersection_time != -1) {
            for (_VehAction action : state.actions) {
                _graph.remove_node(action.node_id);
            }
            remove_buffer.push_back(veh_p);
            _simulator_p->tell_go_next(veh_p);
        }
        //while (!state.actions.empty()) {
        //    _VehAction action = state.actions.front();
        //    if (action.end_time == time_now + 1) {
        //        state.actions.pop_front();
        //        _graph.remove_node(action.node_id);
        //        if (state.actions.empty()) {
        //            assert(state.exit_intersection_time == time_now + 1);
        //            _simulator_p->tell_go_next(veh_p);
        //            remove_buffer.push_back(veh_p);
        //        }
        //    }
        //    else {
        //        break;
        //    }
        //}

        //while (true) { _VehAction action = state.actions.front();
        //    if (action.start_time == time_now + 1) {
        //        assert(action.frozen);
        //        std::cout << "intersection tell " << std::hex << veh_p << std::dec << " time [" << action.start_time << ", " << action.end_time << ") from " << action.from << " to " << action.to << '\n';
        //        _graph.remove_node(action.node_id);
        //        if (action.from == -1) {
        //            assert(_in_lanes[state.in_lane_id].queued_vehicles.front() == veh_p);
        //            _in_lanes[state.in_lane_id].queued_vehicles.pop_front();
        //        }
        //        if (action.to == -1) {
        //            assert(state.actions.size() == 1);
        //            state.actions.pop_front();
        //            state.exit_intersection_time = action.end_time;
        //            break;
        //        }
        //        state.actions.pop_front();
        //    }
        //    else {
        //        break;
        //    }
        //}
    }
    for (Vehicle *veh_p : remove_buffer) {
        _vehicle_states.erase(veh_p);
    }
}
