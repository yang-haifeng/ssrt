# Files for conditional compilation

PROBLEM_FILE = IRS.cpp

# General compiler specifications

CXX := g++
CXXFLAGS := -O3

# Preliminary definitions

EXE_DIR := bin/
EXECUTABLE := $(EXE_DIR)ssrt
SRC_FILES := $(wildcard src/*.cpp) \
             $(wildcard src/model/*.cpp) \
             src/user_models/$(PROBLEM_FILE)
OBJ_DIR := obj/
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(SRC_FILES:.cpp=.o)))
SRC_DIR := $(dir $(SRC_FILES) $(PROB_FILES))
VPATH := $(SRC_DIR)

.PHONY : all dirs clean

all : dirs $(EXECUTABLE)

objs : dirs $(OBJ_FILES)

dirs : $(EXE_DIR) $(OBJ_DIR)

$(EXE_DIR):
	mkdir -p $(EXE_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link objects into executable

$(EXECUTABLE) : $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES)

# Create objects from source files

$(OBJ_DIR)%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup

clean :
	rm -rf $(OBJ_DIR)*
	rm -rf $(EXECUTABLE)
