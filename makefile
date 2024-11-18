
# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS := -lcurl -lpthread

# Directories
SRC_DIR := src
BUILD_DIR := build
DEBUG_DIR := $(BUILD_DIR)/debug
RELEASE_DIR := $(BUILD_DIR)/release

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(SRC_DIR)/*.h)
DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(DEBUG_DIR)/%.o, $(SOURCES))
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(RELEASE_DIR)/%.o, $(SOURCES))

# Targets
DEBUG_TARGET := $(DEBUG_DIR)/bittorrent_client
RELEASE_TARGET := $(RELEASE_DIR)/bittorrent_client

.PHONY: all debug release clean

# Default target
all: debug release

# Debug build
debug: CXXFLAGS += -g -fsanitize=thread -DDEBUG
debug: $(DEBUG_TARGET)

$(DEBUG_DIR):
	@mkdir -p $@

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(DEBUG_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEBUG_TARGET): $(DEBUG_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: $(RELEASE_TARGET)

$(RELEASE_DIR):
	@mkdir -p $@

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RELEASE_TARGET): $(RELEASE_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Clean build directories
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned build directories."
