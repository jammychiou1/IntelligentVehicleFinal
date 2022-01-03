#include "graph.h"

#include <iostream>
#include <cassert>
#include <queue>

using std::cout;
using std::vector, std::map, std::deque;
using std::max;

Graph::Graph() {
    cout << "graph created\n";
}

vector<int> Graph::add_path(std::vector<int> delays, int start_time) {
    int count = delays.size();
    assert(start_time >= 0);
    vector<int> new_nodes;
    for (int i = 0; i < count; i++) {
        _nodes[_node_id_now] = {};
        _nodes[_node_id_now].id = _node_id_now;
        _nodes[_node_id_now].path_id = _path_id_now;
        _nodes[_node_id_now].delay = delays[i];
        _nodes[_node_id_now].in_deg = 0;
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
    _nodes[new_nodes[0]].time = start_time;
    _path_id_now++;
    return new_nodes;
}

void Graph::add_type2_edge(int u, int v) {
    assert(_nodes.count(u));
    assert(_nodes.count(v));
    assert(_nodes[u].path_id != _nodes[v].path_id);
    _Edge edge;

    edge.id = _edge_id_now;
    edge.type = 2;
    edge.u = u;
    edge.v = v;
    _nodes[u].adjs[_edge_id_now] = edge;
    _nodes[v].in_deg++;
    _edge_id_now++;
}

int Graph::add_type3_pair(int u1, int v1, int u2, int v2) {
    assert(_nodes.count(u1));
    assert(_nodes.count(v1));
    assert(_nodes.count(u2));
    assert(_nodes.count(v2));
    assert(_nodes[u1].path_id == _nodes[v1].path_id);
    assert(_nodes[u2].path_id == _nodes[v2].path_id);
    assert(_nodes[u1].path_id != _nodes[u2].path_id);
    _Edge edge1, edge2;
    _Type3Pair t3p;
    t3p.use_first = false;

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
    return _pair_id_now++;
}

void Graph::remove_node(int u) {
    assert(_nodes.count(u));
    assert(_pairs.size() == 0);
    assert(_nodes[u].in_deg == 0);
    for (auto p : _nodes[u].adjs) {
        _Edge edge = p.second;
        _nodes[edge.v].in_deg--;
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
        if (t3p.use_first) {
            _nodes[t3p.v2].tmp_in_deg--;
        }
        else {
            _nodes[t3p.v1].tmp_in_deg--;
        }
    }
    deque<int> queue;
    for (auto &p : _nodes) {
        _Node &node = p.second;
        if (node.tmp_in_deg == 0) {
            queue.push_back(node.id);
        }
        else {
            node.time = 0;
        }
    }
    int cnt = 0;
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        cnt++;
        for (auto p : _nodes[u].adjs) {
            _Edge edge = p.second;
            if (edge.type == 3) {
                assert(_pairs.count(edge.pair_id));
                _Type3Pair t3p = _pairs[edge.pair_id];
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
            _nodes[edge.v].time = max(_nodes[edge.v].time, _nodes[u].time + _nodes[u].delay);
            _nodes[edge.v].tmp_in_deg--;
            if (_nodes[edge.v].tmp_in_deg == 0) {
                queue.push_back(edge.v);
            }
        }
    }
    if (cnt != (int)_nodes.size()) {
        return false;
    }
    return true;
};

void Graph::change_used(map<int, bool> used) {
    for (auto p : used) {
        assert(_pairs.count(p.first));
        _pairs[p.first].use_first = p.second;
    }
}

void Graph::commit_used() {
    for (auto p : _pairs) {
        _Type3Pair t3p = p.second;
        if (t3p.use_first) {
            _nodes[t3p.u2].adjs.erase(t3p.eg2);
            _nodes[t3p.v2].in_deg--;
        }
        else {
            _nodes[t3p.u1].adjs.erase(t3p.eg1);
            _nodes[t3p.v1].in_deg--;
        }
    }
}
