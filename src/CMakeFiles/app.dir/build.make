# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_SOURCE_DIR = /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src

# Include any dependencies generated for this target.
include CMakeFiles/app.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/app.dir/flags.make

CMakeFiles/app.dir/main.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/app.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/main.cpp.o -c /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/main.cpp

CMakeFiles/app.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/main.cpp > CMakeFiles/app.dir/main.cpp.i

CMakeFiles/app.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/main.cpp -o CMakeFiles/app.dir/main.cpp.s

CMakeFiles/app.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/app.dir/main.cpp.o.requires

CMakeFiles/app.dir/main.cpp.o.provides: CMakeFiles/app.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/app.dir/main.cpp.o.provides

CMakeFiles/app.dir/main.cpp.o.provides.build: CMakeFiles/app.dir/main.cpp.o


CMakeFiles/app.dir/Individuals.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Individuals.cpp.o: Individuals.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/app.dir/Individuals.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/Individuals.cpp.o -c /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Individuals.cpp

CMakeFiles/app.dir/Individuals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Individuals.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Individuals.cpp > CMakeFiles/app.dir/Individuals.cpp.i

CMakeFiles/app.dir/Individuals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Individuals.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Individuals.cpp -o CMakeFiles/app.dir/Individuals.cpp.s

CMakeFiles/app.dir/Individuals.cpp.o.requires:

.PHONY : CMakeFiles/app.dir/Individuals.cpp.o.requires

CMakeFiles/app.dir/Individuals.cpp.o.provides: CMakeFiles/app.dir/Individuals.cpp.o.requires
	$(MAKE) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/Individuals.cpp.o.provides.build
.PHONY : CMakeFiles/app.dir/Individuals.cpp.o.provides

CMakeFiles/app.dir/Individuals.cpp.o.provides.build: CMakeFiles/app.dir/Individuals.cpp.o


CMakeFiles/app.dir/Genotypes.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Genotypes.cpp.o: Genotypes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/app.dir/Genotypes.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/Genotypes.cpp.o -c /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Genotypes.cpp

CMakeFiles/app.dir/Genotypes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Genotypes.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Genotypes.cpp > CMakeFiles/app.dir/Genotypes.cpp.i

CMakeFiles/app.dir/Genotypes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Genotypes.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Genotypes.cpp -o CMakeFiles/app.dir/Genotypes.cpp.s

CMakeFiles/app.dir/Genotypes.cpp.o.requires:

.PHONY : CMakeFiles/app.dir/Genotypes.cpp.o.requires

CMakeFiles/app.dir/Genotypes.cpp.o.provides: CMakeFiles/app.dir/Genotypes.cpp.o.requires
	$(MAKE) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/Genotypes.cpp.o.provides.build
.PHONY : CMakeFiles/app.dir/Genotypes.cpp.o.provides

CMakeFiles/app.dir/Genotypes.cpp.o.provides.build: CMakeFiles/app.dir/Genotypes.cpp.o


CMakeFiles/app.dir/Weights.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Weights.cpp.o: Weights.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/app.dir/Weights.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/Weights.cpp.o -c /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Weights.cpp

CMakeFiles/app.dir/Weights.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Weights.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Weights.cpp > CMakeFiles/app.dir/Weights.cpp.i

CMakeFiles/app.dir/Weights.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Weights.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/Weights.cpp -o CMakeFiles/app.dir/Weights.cpp.s

CMakeFiles/app.dir/Weights.cpp.o.requires:

.PHONY : CMakeFiles/app.dir/Weights.cpp.o.requires

CMakeFiles/app.dir/Weights.cpp.o.provides: CMakeFiles/app.dir/Weights.cpp.o.requires
	$(MAKE) -f CMakeFiles/app.dir/build.make CMakeFiles/app.dir/Weights.cpp.o.provides.build
.PHONY : CMakeFiles/app.dir/Weights.cpp.o.provides

CMakeFiles/app.dir/Weights.cpp.o.provides.build: CMakeFiles/app.dir/Weights.cpp.o


# Object files for target app
app_OBJECTS = \
"CMakeFiles/app.dir/main.cpp.o" \
"CMakeFiles/app.dir/Individuals.cpp.o" \
"CMakeFiles/app.dir/Genotypes.cpp.o" \
"CMakeFiles/app.dir/Weights.cpp.o"

# External object files for target app
app_EXTERNAL_OBJECTS =

app: CMakeFiles/app.dir/main.cpp.o
app: CMakeFiles/app.dir/Individuals.cpp.o
app: CMakeFiles/app.dir/Genotypes.cpp.o
app: CMakeFiles/app.dir/Weights.cpp.o
app: CMakeFiles/app.dir/build.make
app: /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/lib/libsavvy.a
app: /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/lib/libhts.a
app: /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/lib/libz.a
app: /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/lib/libzstd.a
app: CMakeFiles/app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/app.dir/build: app

.PHONY : CMakeFiles/app.dir/build

CMakeFiles/app.dir/requires: CMakeFiles/app.dir/main.cpp.o.requires
CMakeFiles/app.dir/requires: CMakeFiles/app.dir/Individuals.cpp.o.requires
CMakeFiles/app.dir/requires: CMakeFiles/app.dir/Genotypes.cpp.o.requires
CMakeFiles/app.dir/requires: CMakeFiles/app.dir/Weights.cpp.o.requires

.PHONY : CMakeFiles/app.dir/requires

CMakeFiles/app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app.dir/clean

CMakeFiles/app.dir/depend:
	cd /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/CMakeFiles/app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/app.dir/depend

