#include <iostream>
#include <cassert>
#include <queue>

#include "graph.h"

using std::cout;
using std::vector, std::map, std::deque, std::pair;
using std::max;

const int INF = 1e9;

Graph::Graph() {
    cout << "graph created\n";
}

pair<int, vector<int>> Graph::add_path(std::vector<int> delays, int start_time) {
    int count = delays.size();
    assert(start_time >= 0);
    vector<int> new_nodes;
    for (int i = 0; i < count; i++) {
        _nodes[_node_id_now] = {};
        _nodes[_node_id_now].id = _node_id_now;
        _nodes[_node_id_now].path_id = _path_id_now;
        _nodes[_node_id_now].delay = delays[i];
        _nodes[_node_id_now].in_deg = 0;
        _nodes[_node_id_now].time_fixed = false;
        _nodes[_node_id_now].lowest_time = start_time;
        new_nodes.push_back(_node_id_now);
        _node_id_now++;
    }
    for (int i = 0; i < count - 1; i++) {
        int a = new_nodes[i], b = new_nodes[i + 1];
        _Edge edge;

        edge.id = _edge_id_now;
        edge.type = 1;
        edge.u = a;
        edge.v = b;
        _nodes[a].adjs[_edge_id_now] = edge;
        _nodes[b].in_deg++;
        _edge_id_now++;
    }
    _paths[_path_id_now] = {};
    _paths[_path_id_now].start_time = start_time;
    for (int i = 0; i < count; i++) {
        _paths[_path_id_now].nodes.push_back(new_nodes[i]);
    }
    _just_calc_ed = false;
    return {_path_id_now++, new_nodes};
}

void Graph::add_type2_edge(int u, int v) {
    assert(_nodes.count(u));
    assert(_nodes.count(v));
    assert(_nodes[u].path_id < _nodes[v].path_id);
    assert(!_nodes[v].time_fixed);
    //assert(!_paths[_nodes[v].path_id].time_fixed);
    _Edge edge;

    edge.id = _edge_id_now;
    edge.type = 2;
    edge.u = u;
    edge.v = v;
    _nodes[u].adjs[_edge_id_now] = edge;
    _nodes[v].in_deg++;
    _edge_id_now++;
    _just_calc_ed = false;
}

int Graph::add_type3_pair(int u1, int v1, int u2, int v2) {
    assert(_nodes.count(u1));
    assert(_nodes.count(v1));
    assert(_nodes.count(u2));
    assert(_nodes.count(v2));
    assert(_nodes[u1].path_id == _nodes[v2].path_id);
    assert(_nodes[u2].path_id == _nodes[v1].path_id);
    assert(_nodes[u1].path_id != _nodes[u2].path_id);
    //assert(!_paths[_nodes[u1].path_id].time_fixed);
    //assert(!_paths[_nodes[u2].path_id].time_fixed);
    assert(!_nodes[v1].time_fixed);
    assert(!_nodes[v2].time_fixed);
    _Edge edge1, edge2;
    _Type3Pair t3p;
    t3p.use_first = false;
    t3p.enabled = true;

    edge1.id = _edge_id_now;
    edge1.type = 3;
    edge1.u = u1;
    edge1.v = v1;
    edge1.pair_id = _pair_id_now;
    _nodes[u1].adjs[_edge_id_now] = edge1;
    _nodes[v1].in_deg++;
    t3p.u1 = u1;
    t3p.v1 = v1;
    t3p.eg1 = _edge_id_now;
    _edge_id_now++;

    edge2.id = _edge_id_now;
    edge2.type = 3;
    edge2.u = u2;
    edge2.v = v2;
    edge2.pair_id = _pair_id_now;
    _nodes[u2].adjs[_edge_id_now] = edge2;
    _nodes[v2].in_deg++;
    t3p.u2 = u2;
    t3p.v2 = v2;
    t3p.eg2 = _edge_id_now;
    _edge_id_now++;

    _pairs[_pair_id_now] = t3p;
    _just_calc_ed = false;
    return _pair_id_now++;
}

void Graph::remove_node(int u) {
    assert(_nodes.count(u));
    assert(_pairs.empty());
    //assert(_nodes[u].in_deg == 0);
    assert(_nodes[u].time_fixed);
    //assert(_paths[_nodes[u].path_id].time_fixed);
    for (auto p : _nodes[u].adjs) {
        _Edge edge = p.second;
        _nodes[edge.v].in_deg--;
    }
    for (auto &p : _nodes) {
        _Node &node = p.second;
        // TODO better removing
        vector<int> remove_edges;
        for (auto &q : node.adjs) {
            _Edge eg = q.second;
            if (eg.v == u) {
                assert(q.first == eg.id);
                remove_edges.push_back(eg.id);
            }
        }
        for (int eid : remove_edges) {
            node.adjs.erase(eid);
        }
    }
    assert(_paths[_nodes[u].path_id].nodes.front() == u);
    _paths[_nodes[u].path_id].nodes.pop_front();
    if (_paths[_nodes[u].path_id].nodes.empty()) {
        _paths.erase(_nodes[u].path_id);
    }
    _nodes.erase(u);
}

bool Graph::calc_time() {
    for (auto &p : _nodes) {
        _Node &node = p.second;
        node.tmp_in_deg = node.in_deg;
    }
    for (auto p : _pairs) {
        _Type3Pair t3p = p.second;
        if (!t3p.enabled) {
            _nodes[t3p.v2].tmp_in_deg--;
            _nodes[t3p.v1].tmp_in_deg--;
        }
        else {
            if (t3p.use_first) {
                _nodes[t3p.v2].tmp_in_deg--;
            }
            else {
                _nodes[t3p.v1].tmp_in_deg--;
            }
        }
    }
    deque<int> queue;
    for (auto &p : _nodes) {
        _Node &node = p.second;
        if (node.tmp_in_deg == 0) {
            queue.push_back(node.id);
        }
        if (node.time_fixed) {
            node.tmp_time = node.lowest_time;
        }
        else {
            node.tmp_time = max(_time_now, node.lowest_time);
        }
    }
    int cnt = 0;
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        cnt++;
        //vector<int> remove_edges;
        for (auto p : _nodes[u].adjs) {
            _Edge edge = p.second;
            if (edge.type == 3) {
                assert(_pairs.count(edge.pair_id));
                _Type3Pair t3p = _pairs[edge.pair_id];
                if (!t3p.enabled) {
                    continue;
                }
                if (t3p.use_first) {
                    if (u == t3p.u2) {
                        continue;
                    }
                }
                else {
                    if (u == t3p.u1) {
                        continue;
                    }
                }
            }
            //if (edge.type == 2) {
            //    if (!_nodes.count(edge.v)) {
            //        remove_edges.push_back(p.first);
            //        continue;
            //    }
            //}
            assert(_nodes.count(edge.v));
            _nodes[edge.v].tmp_time = max(_nodes[edge.v].tmp_time, _nodes[u].tmp_time + _nodes[u].delay);
            _nodes[edge.v].tmp_in_deg--;
            if (_nodes[edge.v].tmp_in_deg == 0) {
                queue.push_back(edge.v);
            }
        }
        //for (int eid : remove_edges) {
        //    _nodes[u].adjs.erase(eid);
        //}
    }
    if (cnt != (int)_nodes.size()) {
        return false;
    }
    for (auto &p : _nodes) {
        _Node &node = p.second;
        if (node.time_fixed) {
            assert(node.tmp_time == node.lowest_time);
        }
    //    if (_paths[node.path_id].time_fixed) {
    //        assert(node.tmp_time == node.time);
    //    }
    }
    _just_calc_ed = true;
    return true;
};

void Graph::change_used(map<int, bool> used) {
    for (auto p : used) {
        assert(_pairs.count(p.first));
        _pairs[p.first].use_first = p.second;
    }
    _just_calc_ed = false;
}

void Graph::commit_used() {
    assert(_just_calc_ed);
    for (auto p : _pairs) {
        _Type3Pair t3p = p.second;
        if (t3p.use_first) {
            _nodes[t3p.u2].adjs.erase(t3p.eg2);
            _nodes[t3p.v2].in_deg--;
            _nodes[t3p.u1].adjs[t3p.eg1].type = 2;
        }
        else {
            _nodes[t3p.u1].adjs.erase(t3p.eg1);
            _nodes[t3p.v1].in_deg--;
            _nodes[t3p.u2].adjs[t3p.eg2].type = 2;
        }
    }
    _pairs.clear();
    for (auto &p : _nodes) {
        _Node &node = p.second;
        node.lowest_time = node.tmp_time;
    }
}

int Graph::get_time(int u) {
    assert(_just_calc_ed);
    assert(_nodes.count(u));
    return _nodes[u].tmp_time;
}

void Graph::fix_node_time(int u) {
    assert(_nodes.count(u));
    //assert(_nodes[u].in_deg == 0);
    assert(_pairs.empty());
    assert(_just_calc_ed);
    assert(!_nodes[u].time_fixed);
    //_paths[path_id].time_fixed = true;
    _nodes[u].time_fixed = true;
}

void Graph::update_time_now(int now) {
    assert(now > _time_now);
    _time_now = now;
}

void Graph::optimize() {
    vector<int> considered_paths;
    vector<int> considered_pairs;
    for (auto &p : _paths) {
        considered_paths.push_back(p.first);
    }
    for (auto p : _pairs) {
        considered_pairs.push_back(p.first);
    }
    _optimize_sub(considered_paths, considered_pairs);
    for (auto p : _pairs) {
        assert(p.second.enabled);
    }
}
void Graph::_optimize_sub(vector<int> considered_paths, vector<int> considered_pairs) {
    for (int pid : considered_pairs) {
        _Type3Pair &t3p = _pairs[pid];
        assert(t3p.enabled);
        t3p.enabled = false;
    }
    bool failed = false;
    for (int i = 0; i < (int) considered_pairs.size(); i++) {
        int impact = INF;
        pair<int, bool> best;
        bool found = false;
        int baseline = _sum_exit();
        for (int pid : considered_pairs) {
            _Type3Pair &t3p = _pairs[pid];
            if (!t3p.enabled) {
                t3p.enabled = true;

                t3p.use_first = true;
                if (calc_time()) {
                    found = true;
                    int tmp = _sum_exit() - baseline;
                    if (tmp < impact) {
                        impact = tmp;
                        best = {pid, true};
                    }
                }
                
                t3p.use_first = false;
                if (calc_time()) {
                    found = true;
                    int tmp = _sum_exit() - baseline;
                    if (tmp < impact) {
                        impact = tmp;
                        best = {pid, false};
                    }
                }

                t3p.enabled = false;
            }
        }
        if (found) {
            int pid = best.first;
            bool use_first = best.second;
            _pairs[pid].enabled = true;
            _pairs[pid].use_first = use_first;
        }
        else {
            failed = true;
            break;
        }
    }
    if (!failed) {
        return;
    }
    int N = considered_paths.size();
    int thresh = considered_paths[N / 2];
    vector<int> considered_pairs_low;
    vector<int> considered_pairs_high;
    for (int pid : considered_pairs) {
        _Type3Pair &t3p = _pairs[pid];
        int path_id1 = _nodes[t3p.u1].path_id;
        int path_id2 = _nodes[t3p.u2].path_id;
        if (path_id1 >= thresh) {
            if (path_id2 >= thresh) {
                t3p.enabled = false;
                considered_pairs_high.push_back(pid);
            }
            else {
                t3p.enabled = true;
                t3p.use_first = false;
            }
        }
        else {
            if (path_id2 >= thresh) {
                t3p.enabled = true;
                t3p.use_first = true;
            }
            else {
                t3p.enabled = true;
                considered_pairs_low.push_back(pid);
            }
        }
    }
    vector<int> considered_paths_low;
    vector<int> considered_paths_high;
    for (int i = 0; i < N / 2; i++) {
        considered_paths_low.push_back(considered_paths[i]);
    }
    for (int i = N / 2; i < N; i++) {
        considered_paths_high.push_back(considered_paths[i]);
    }
    _optimize_sub(considered_paths_low, considered_pairs_low);
    for (int pid : considered_pairs_low) {
        assert(_pairs[pid].enabled);
    }
    for (int pid : considered_pairs_high) {
        assert(!_pairs[pid].enabled);
        _pairs[pid].enabled = true;
    }
    _optimize_sub(considered_paths_high, considered_pairs_high);
    for (int pid : considered_pairs_high) {
        assert(_pairs[pid].enabled);
    }
}
int Graph::_sum_exit() {
    int ans = 0;
    for (auto &p : _paths) {
        _Path &path = p.second;
        ans += _nodes[path.nodes.back()].tmp_time;
    }
    return ans;
}
