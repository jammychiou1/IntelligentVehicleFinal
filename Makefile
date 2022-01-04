.PHONY: all clean

$(shell mkdir -p obj)

TARGET := test/graph test/optimize

CC := g++
CCFLAGS := -Wall -std=gnu++17 -ggdb

all: $(TARGET)

obj/graph.o: graph.cpp graph.h
	$(CC) $(CCFLAGS) -c -o $@ $<

test/graph: obj/graph.o test/graph.cpp
	$(CC) $(CCFLAGS) -o $@ $^

test/optimize: obj/graph.o test/optimize.cpp
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) obj/*.o
