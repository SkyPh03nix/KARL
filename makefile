CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -IC:/includes/SFML-2.6.1/include

# SFML Bibliothekspfad und Libraries
LDFLAGS = -LC:/includes/SFML-2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Quell- und Objektdateien
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Objektdateien
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# build-Ordner erzeugen, falls nicht vorhanden
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean-Regel
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
