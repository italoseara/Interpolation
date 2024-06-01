CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude
LIBS = -lm -lGL -lGLU -lglut

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, bin/%.o, $(SRCS))

TARGET = bin/main

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

bin/%.o: src/%.cpp | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

bin:
	mkdir -p bin

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin

all: bin $(TARGET)