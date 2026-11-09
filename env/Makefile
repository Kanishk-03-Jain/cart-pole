CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LDLIBS   = -lraylib

SRCS = main.cpp
HDRS = cartpole.h renderer.h strategy.h strategies/random_strategy.h evaluator.h

.PHONY: build run clean

build: main

main: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDLIBS)

run: build
	./main

clean:
	rm -f main