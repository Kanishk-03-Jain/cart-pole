CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Isrc
LDLIBS   = -lraylib

TARGET = build/main
SRCS   = src/main.cpp
HDRS   = $(wildcard src/*/*.h)

.PHONY: build run clean

build: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDLIBS)

run: build
	./$(TARGET)

clean:
	rm -rf build