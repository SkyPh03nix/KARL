CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -IC:/includes/SFML-2.6.1/include -MMD -MP

# SFML path and libs
SFML_LIB_PATH = -LC:/includes/SFML-2.6.1/lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

LDFLAGS = $(SFML_LIB_PATH) $(SFML_LIBS)

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# build dir if not existing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
