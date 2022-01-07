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
    if (argc < 2) {
        //simulator = Simulator(std::cin);
        is = &cin;
    }
    else {
        std::ifstream fs(argv[1], std::fstream::in);
        //simulator = Simulator(fs);
        is = &fs;
    }
    Simulator simulator(*is);
}
