# 1. Compiler and Flag Definitions
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3 -g -fsanitize=address

# 2. Directory Paths
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# 3. Target Executable
TARGET = $(BUILD_DIR)/main.exe

# 4. File Discoveries and Conversions
# Finds all .c files in the src/ directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Maps src/filename.c to build/obj/filename.o
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: create_dirs $(TARGET)

# Create the output directories if they do not exist
create_dirs:
	mkdir -p $(BUILD_DIR) $(OBJ_DIR)

# Link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile each individual .cpp file into a .o file in build/obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Deep clean to completely wipe the build folder
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean create_dirs