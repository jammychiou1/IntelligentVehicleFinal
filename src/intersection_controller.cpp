#include <cassert>

#include "intersection_controller.h"

#include "vehicle.h"
#include "simulator.h"

using std::pair, std::vector, std::map;
using std::make_pair;

IntersectionController::IntersectionController(int id, std::vector<int> conflict_zone_ids, std::map<int, std::vector<TrajectoryDesc>> in_lane_to_trajectories, Simulator* simulator_p) {
    //assert(time_p != nullptr);
    _simulator_p = simulator_p;
    for (int czid : conflict_zone_ids) {
        _conflict_zones[czid] = {};
    }
    for (auto &p : in_lane_to_trajectories) {
        int in_lane_id = p.first;
        vector<TrajectoryDesc> &trajs = p.second;

        _InLane in_lane;
        in_lane.in_lane_id = in_lane_id;
        in_lane.queued_vehicles = {};
        in_lane.trajectories = {};
        //map<int, _Trajectory> out_lanes_to_traj;
        for (TrajectoryDesc &traj : trajs) {
            for (int czid : traj.conflict_zone_ids) {
                assert(_conflict_zones.count(czid));
                _conflict_zones[czid].trajectories_over.push_back(make_pair(in_lane_id, traj.out_lane_id));
            }

            _Trajectory new_traj;
            //new_traj.in_lane_id = traj.in_lane_id;
            //new_traj.out_lane_id = traj.out_lane_id;
            new_traj.conflict_zone_ids = traj.conflict_zone_ids;
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
    _vehicle_states[veh_p] = state;
}

void IntersectionController::update() {
}
