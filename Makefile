.PHONY: all clean

$(shell mkdir -p obj)

SRCDIR := src
OBJDIR := obj
HEADERDIR := include

HEADERS := $(wildcard $(HEADERDIR)/*.h)
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
TARGET := test/graph test/optimize main

CC := g++
#CCFLAGS := -Wall -std=gnu++17 -ggdb -fsanitize=address -fsanitize=undefined
CCFLAGS := -Wall -std=gnu++17 -O2

all: $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CCFLAGS) -c -o $@ $< -I $(HEADERDIR)

#obj/graph.o: graph.cpp graph.h
#	$(CC) $(CCFLAGS) -c -o $@ $<
#
#obj/vehicle.o: vehicle.cpp vehicle.h intersection_controller.h lane_controller.h scenario.h
#	$(CC) $(CCFLAGS) -c -o $@ $<
#
#obj/intersection_controller.o: intersection_controller.cpp intersection_controller.h graph.h vehicle.h
#	$(CC) $(CCFLAGS) -c -o $@ $<
#
#obj/lane_controller.o: lane_controller.cpp lane_controller.h vehicle.h
#	$(CC) $(CCFLAGS) -c -o $@ $<
#
#obj/source_lane.o: source_lane.cpp source_lane.h vehicle.h scenario.h
#	$(CC) $(CCFLAGS) -c -o $@ $<
#
#obj/destination_lane.o: destination_lane.cpp destination_lane.h vehicle.h
#	$(CC) $(CCFLAGS) -c -o $@ $<
	
test/graph: test/graph.cpp $(OBJS) $(HEADERS)
	$(CC) $(CCFLAGS) -o $@ $< $(OBJS) -I $(HEADERDIR)

test/optimize: test/optimize.cpp $(OBJS) $(HEADERS)
	$(CC) $(CCFLAGS) -o $@ $< $(OBJS) -I $(HEADERDIR)

main: main.cpp $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ -I $(HEADERDIR)

clean:
	rm -f $(TARGET) $(OBJS)
