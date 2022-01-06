#include <iostream>
#include <vector>
#include <cassert>

#include "graph.h"

using namespace std;

int main() {
    Graph graph;
    auto [pid1, ids1] = graph.add_path({3, 3, 3}, 5);
    auto [pid2, ids2] = graph.add_path({3, 3, 3, 3}, 5);
    int p1 = graph.add_type3_pair(ids1[1], ids2[0], ids2[1], ids1[0]);
    int p2 = graph.add_type3_pair(ids1[2], ids2[2], ids2[3], ids1[1]);

    graph.change_used({{p1, true}, {p2, false}});
    assert(!graph.calc_time());

    graph.change_used({{p1, false}, {p2, true}});
    assert(graph.calc_time());
    //for (int id : ids1) {
    //    cout << graph.get_time(id) << '\n';
    //}
    //for (int id : ids2) {
    //    cout << graph.get_time(id) << '\n';
    //}
    assert(graph.get_time(ids2[3]) == 23);
    graph.commit_used();

    graph.fix_node_time(ids2[0]);
    assert(graph.calc_time());
    assert(graph.get_time(ids2[3]) == 23);

    graph.fix_node_time(ids2[1]);
    graph.remove_node(ids2[0]);
    assert(graph.calc_time());
    assert(graph.get_time(ids2[3]) == 23);

    graph.fix_node_time(ids1[0]);
    graph.fix_node_time(ids2[2]);
    graph.remove_node(ids2[1]);
    assert(graph.calc_time());
    assert(graph.get_time(ids2[3]) == 23);

    cout << "done\n";
}
