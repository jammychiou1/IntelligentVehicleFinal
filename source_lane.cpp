#include <algorithm>

#include "source_lane.h"
#include "vehicle.h"

SourceLane::SourceLane(int id, double prob, Scenario* scenario_p, std::default_random_engine* gen_p){
    _id = id;
    _prob = prob;
    _scenario_p = scenario_p;
    _distribution = std::uniform_real_distribution<double>(0.0,1.0);
    _gen_p = gen_p;
}
bool SourceLane::_dfs_route(int now_intersection_id, int last_lane_id, int dst_lane_id, std::vector<RouteNode>& now_route, std::set<int>& arrival){
    std::vector<int> out_lanes_id = (_scenario_p->connectivity[now_intersection_id])[last_lane_id];
    shuffle(out_lanes_id.begin(), out_lanes_id.end(), *_gen_p);
    arrival.insert(now_intersection_id);
    RouteNode node;
    node.type = RouteNodeType::lane;
    node.id = last_lane_id;
    now_route.push_back(node);
    node.type = RouteNodeType::intersection;
    node.id = now_intersection_id;
    now_route.push_back(node);
    for(int i = 0; i < (int)out_lanes_id.size(); i++){
        if(out_lanes_id[i] == dst_lane_id){
            node.type = RouteNodeType::lane;
            node.id = dst_lane_id;
            now_route.push_back(node);
            return true;
        }
    }
    for(int i = 0; i < (int)out_lanes_id.size(); i++){
        if(_scenario_p->lanes[out_lanes_id[i]].second >= 0
                && arrival.find(_scenario_p->lanes[out_lanes_id[i]].second) == arrival.end()
                && _dfs_route(_scenario_p->lanes[out_lanes_id[i]].second, out_lanes_id[i], dst_lane_id, now_route, arrival))
            return true;
    }
    now_route.pop_back();
    now_route.pop_back();
    return false;
}
void SourceLane::update(){
    double number = _distribution(*_gen_p);
    if(number < _prob){
        std::vector<RouteNode> now_route;
        std::set<int> arrival;
        std::uniform_int_distribution<int> distribution(0, _scenario_p->dst_lanes_id.size() - 1);
        int dst_lane_id = _scenario_p->dst_lanes_id[distribution(*_gen_p)];
        _dfs_route(_scenario_p->lanes[_id].second, _id, dst_lane_id, now_route, arrival);
        Vehicle* veh_p = new Vehicle(now_route, _scenario_p);
        _scenario_p->vehicles.insert(veh_p);
        veh_p->go_next();
    }
}