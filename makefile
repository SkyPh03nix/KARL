CXX = g++
CXXFLAGS = -std=c++11 -IC:/includes/SFML-2.6.1/include
LDFLAGS = -LC:/includes/SFML-2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

all:
	$(CXX) main.cpp Entity.cpp -o main $(CXXFLAGS) $(LDFLAGS)