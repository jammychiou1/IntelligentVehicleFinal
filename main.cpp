#include <vector>
#include <iostream>
#include <fstream>
#include "vehicle.h"
#include "intersection_controller.h"
#include "lane_controller.h"
#include "scenario.h"
#include "simulator.h"

using std::cin;
int main(int argc, const char* argv[]) {
    std::istream *is;
    std::ifstream fs(argv[1], std::fstream::in);
    if (argc < 2) {
        //simulator = Simulator(std::cin);
        is = &cin;
    }
    else {
        //simulator = Simulator(fs);
        fs = std::ifstream(argv[1], std::fstream::in);
        is = &fs;
    }
    Simulator simulator(*is);
    simulator.start_simulation();
}
