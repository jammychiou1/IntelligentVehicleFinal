#include <vector>
#include <iostream>

#include "vehicle.h"
#include "intersection_controller.h"
#include "lane_controller.h"
#include "scenario.h"
#include "simulator.h"

using std::cin;
int main(int argc, const char* argv[]) {
    if (argc < 2) Simulator(std::cin);
    else{
        fstream fs(argv[1], std::fstream::in);
        Simulator(fs);
    }    
}
