#ifndef SOURCE_LANE_H
#define SOURCE_LANE_H

#include <set>
#include <random>

#include "route.h"

class Scenario;
class Simulator;

class SourceLane{
    public:
        SourceLane(int id = 0, double prob = 0.0, Scenario* scenario_p = nullptr, Simulator* simulator_p = nullptr, std::default_random_engine* gen_p = nullptr);
        void update();
    private:
        int _id;
        double _prob;
        Scenario *_scenario_p;
        std::uniform_real_distribution<double> _distribution;
        std::default_random_engine* _gen_p;
        bool _dfs_route(int, int, int, std::vector<RouteNode>&, std::set<int>&);

        Simulator* _simulator_p;
};

#endif
