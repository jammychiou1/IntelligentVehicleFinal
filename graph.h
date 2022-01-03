#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <utility>

class Graph {
    public:
        Graph();
        std::vector<int> add_path(std::vector<int> delays, int start_time);
        void add_type2_edge(int u, int v);
        int add_type3_pair(int u1, int v1, int u2, int v2);
        void remove_node(int u);
        bool calc_time();
        void change_used(std::map<int, bool> used);
        void commit_used();

    private:
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
            int time;
            std::map<int, _Edge> adjs;
        };
        struct _Type3Pair {
            int u1, v1, u2, v2;
            int eg1, eg2;
            bool use_first;
        };
        std::map<int, _Node> _nodes;
        std::map<int, _Type3Pair> _pairs;
        int _path_id_now = 0;
        int _edge_id_now = 0;
        int _node_id_now = 0;
        int _pair_id_now = 0;
};

#endif
