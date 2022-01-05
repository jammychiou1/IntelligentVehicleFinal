#include <cassert>

#include "intersection_controller.h"

#include "vehicle.h"

IntersectionController::IntersectionController(int id, int *time_p) {
    //assert(time_p != nullptr);
    _time_p = time_p;
}
void IntersectionController::enter(Vehicle *veh_p, int in_lane_id, int out_lane_id) {
}
void update() {
}
