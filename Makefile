CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Isrc
LDLIBS   = -lraylib

CLANG_FORMAT ?= clang-format

TARGET = build/main
SRCS   = src/main.cpp
HDRS   = $(wildcard src/*/*.h)

.PHONY: build run clean format

build: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDLIBS)

run: build
	./$(TARGET)

clean:
	rm -rf build

format:
	$(CLANG_FORMAT) -i $(SRCS) $(HDRS)
