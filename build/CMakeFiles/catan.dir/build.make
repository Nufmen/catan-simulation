# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cypher/Catan/catan-simulation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cypher/Catan/catan-simulation/build

# Include any dependencies generated for this target.
include CMakeFiles/catan.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/catan.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/catan.dir/flags.make

CMakeFiles/catan.dir/src/main.cpp.o: CMakeFiles/catan.dir/flags.make
CMakeFiles/catan.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cypher/Catan/catan-simulation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/catan.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/catan.dir/src/main.cpp.o -c /home/cypher/Catan/catan-simulation/src/main.cpp

CMakeFiles/catan.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/catan.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cypher/Catan/catan-simulation/src/main.cpp > CMakeFiles/catan.dir/src/main.cpp.i

CMakeFiles/catan.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/catan.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cypher/Catan/catan-simulation/src/main.cpp -o CMakeFiles/catan.dir/src/main.cpp.s

# Object files for target catan
catan_OBJECTS = \
"CMakeFiles/catan.dir/src/main.cpp.o"

# External object files for target catan
catan_EXTERNAL_OBJECTS =

catan.cpython-38-x86_64-linux-gnu.so: CMakeFiles/catan.dir/src/main.cpp.o
catan.cpython-38-x86_64-linux-gnu.so: CMakeFiles/catan.dir/build.make
catan.cpython-38-x86_64-linux-gnu.so: CMakeFiles/catan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cypher/Catan/catan-simulation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module catan.cpython-38-x86_64-linux-gnu.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/catan.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/strip /home/cypher/Catan/catan-simulation/build/catan.cpython-38-x86_64-linux-gnu.so

# Rule to build all files generated by this target.
CMakeFiles/catan.dir/build: catan.cpython-38-x86_64-linux-gnu.so

.PHONY : CMakeFiles/catan.dir/build

CMakeFiles/catan.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/catan.dir/cmake_clean.cmake
.PHONY : CMakeFiles/catan.dir/clean

CMakeFiles/catan.dir/depend:
	cd /home/cypher/Catan/catan-simulation/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cypher/Catan/catan-simulation /home/cypher/Catan/catan-simulation /home/cypher/Catan/catan-simulation/build /home/cypher/Catan/catan-simulation/build /home/cypher/Catan/catan-simulation/build/CMakeFiles/catan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/catan.dir/depend

