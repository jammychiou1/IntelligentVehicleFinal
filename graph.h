#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <utility>
#include <queue>

class Graph {
    public:
        Graph();

        // invalidate last calc_time
        std::pair<int, std::vector<int>> add_path(std::vector<int> delays, int start_time);
        void add_type2_edge(int u, int v);
        int add_type3_pair(int u1, int v1, int u2, int v2);
        void change_used(std::map<int, bool> used);

        // call before calc_time/ optimize
        void update_time_now(int now);
        
        // need to commit / calc after optimize
        void optimize();
        
        // calc_time before commit_used / get_time
        bool calc_time();
        int get_time(int u);
        void commit_used();

        // after commit used
        void remove_node(int u);
        void fix_node_time(int u);

    private:
        int _sum_exit();
        void _optimize_sub(std::vector<int> considered_paths, std::vector<int> considered_pairs);

        struct _Edge {
            int id;
            int type;
            int u, v;
            int pair_id;
        };
        struct _Node {
            int id;
            int path_id;
            int delay;
            int in_deg;
            int tmp_in_deg;
            int lowest_time;
            int tmp_time;
            bool time_fixed;
            std::map<int, _Edge> adjs;
        };
        struct _Type3Pair {
            int u1, v1, u2, v2;
            int eg1, eg2;
            bool use_first;
            bool enabled;
        };
        struct _Path {
            std::deque<int> nodes;
            //bool time_fixed;
            int start_time;
        };
        std::map<int, _Node> _nodes;
        std::map<int, _Type3Pair> _pairs;
        std::map<int, _Path> _paths;
        int _path_id_now = 0;
        int _edge_id_now = 0;
        int _node_id_now = 0;
        int _pair_id_now = 0;
        bool _just_calc_ed = false;
        int _time_now = -1;
};

#endif
