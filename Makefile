CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 $(shell sdl2-config --cflags)
TARGET = gbemu
SRCS = main.cpp
LDFLAGS = $(shell sdl2-config --libs)
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS) directories
	SECONDS=0
	@echo "directories made!"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "project compiled!"
	@echo "build time: $$SECONDS seconds"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean-build:
	rm -f $(TARGET) $(OBJS)
	@echo "build files erased."

clean-directory:
	rm -ri ~/gbemu
	@echo "directories erased."

run: $(TARGET)
	./$(TARGET)

directories:
	mkdir -p ~/gbemu
	mkdir -p ~/gbemu/roms 
	mkdir -p ~/gbemu/saves
