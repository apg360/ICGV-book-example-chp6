# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build

# Include any dependencies generated for this target.
include CMakeFiles/volk.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/volk.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/volk.dir/flags.make

CMakeFiles/volk.dir/volk.c.o: CMakeFiles/volk.dir/flags.make
CMakeFiles/volk.dir/volk.c.o: ../volk.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/volk.dir/volk.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/volk.dir/volk.c.o -c /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/volk.c

CMakeFiles/volk.dir/volk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/volk.dir/volk.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/volk.c > CMakeFiles/volk.dir/volk.c.i

CMakeFiles/volk.dir/volk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/volk.dir/volk.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/volk.c -o CMakeFiles/volk.dir/volk.c.s

# Object files for target volk
volk_OBJECTS = \
"CMakeFiles/volk.dir/volk.c.o"

# External object files for target volk
volk_EXTERNAL_OBJECTS =

libvolk.a: CMakeFiles/volk.dir/volk.c.o
libvolk.a: CMakeFiles/volk.dir/build.make
libvolk.a: CMakeFiles/volk.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libvolk.a"
	$(CMAKE_COMMAND) -P CMakeFiles/volk.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/volk.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/volk.dir/build: libvolk.a

.PHONY : CMakeFiles/volk.dir/build

CMakeFiles/volk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/volk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/volk.dir/clean

CMakeFiles/volk.dir/depend:
	cd /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140 /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140 /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build /Users/a0/MyProjects/GitHub/buildTemplate/libs/volk-1.2.140/_build/CMakeFiles/volk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/volk.dir/depend
