.PHONY: all clean

$(shell mkdir -p obj)

TARGET := test/graph test/optimize main

CC := g++
CCFLAGS := -Wall -std=gnu++17 -ggdb

all: $(TARGET)

obj/graph.o: graph.cpp graph.h
	$(CC) $(CCFLAGS) -c -o $@ $<

obj/vehicle.o: vehicle.cpp vehicle.h intersection_controller.h lane_controller.h scenario.h
	$(CC) $(CCFLAGS) -c -o $@ $<

obj/intersection_controller.o: intersection_controller.cpp intersection_controller.h graph.h vehicle.h
	$(CC) $(CCFLAGS) -c -o $@ $<

obj/lane_controller.o: lane_controller.cpp lane_controller.h vehicle.h
	$(CC) $(CCFLAGS) -c -o $@ $<

obj/source_lane.o: source_lane.cpp source_lane.h vehicle.h scenario.h
	$(CC) $(CCFLAGS) -c -o $@ $<

obj/destination_lane.o: destination_lane.cpp destination_lane.h vehicle.h
	$(CC) $(CCFLAGS) -c -o $@ $<
	
test/graph: obj/graph.o graph.h test/graph.cpp
	$(CC) $(CCFLAGS) -o $@ $^

test/optimize: obj/graph.o graph.h test/optimize.cpp
	$(CC) $(CCFLAGS) -o $@ $^

main: obj/graph.o obj/vehicle.o obj/intersection_controller.o obj/lane_controller.o graph.h main.cpp
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) obj/*.o
