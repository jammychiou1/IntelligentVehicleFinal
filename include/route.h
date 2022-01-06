#ifndef ROUTE_H
#define ROUTE_H
enum class RouteNodeType {
    intersection, lane
};

struct RouteNode {
    RouteNodeType type;
    int id;
};
#endif
