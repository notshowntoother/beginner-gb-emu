CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = gbemu
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS) directories
	@echo "directories made!"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "project compiled!"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean-build:
	rm -f $(TARGET) $(OBJS)
	@echo "build files erased.""

clean-directory:
	rm -ri ~/gbemu
	@echo " directories erased."

run: $(TARGET)
	./$(TARGET)

directories:
	mkdir -p ~/gbemu
	mkdir -p ~/gbemu/roms 
	mkdir -p ~/gbemu/saves
