# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/hesam/Desktop/OCIR-Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hesam/Desktop/OCIR-Project/build

# Include any dependencies generated for this target.
include CMakeFiles/HotSpotter.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HotSpotter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HotSpotter.dir/flags.make

CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o: ../src/GridEyeReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/GridEyeReader.cpp

CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/GridEyeReader.cpp > CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.i

CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/GridEyeReader.cpp -o CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.s

CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o


CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o: ../src/HotSpotter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/HotSpotter.cpp

CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/HotSpotter.cpp > CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.i

CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/HotSpotter.cpp -o CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.s

CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o


CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o: ../src/OccupancyCounter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/OccupancyCounter.cpp

CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/OccupancyCounter.cpp > CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.i

CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/OccupancyCounter.cpp -o CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.s

CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o


CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o: ../src/XMPPInterface.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/XMPPInterface.cpp

CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/XMPPInterface.cpp > CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.i

CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/XMPPInterface.cpp -o CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.s

CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o


CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o: ../src/XMPPWrapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/XMPPWrapper.cpp

CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/XMPPWrapper.cpp > CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.i

CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/XMPPWrapper.cpp -o CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.s

CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o


CMakeFiles/HotSpotter.dir/src/buffer.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/buffer.cpp.o: ../src/buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/HotSpotter.dir/src/buffer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/buffer.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/buffer.cpp

CMakeFiles/HotSpotter.dir/src/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/buffer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/buffer.cpp > CMakeFiles/HotSpotter.dir/src/buffer.cpp.i

CMakeFiles/HotSpotter.dir/src/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/buffer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/buffer.cpp -o CMakeFiles/HotSpotter.dir/src/buffer.cpp.s

CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/buffer.cpp.o


CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o: CMakeFiles/HotSpotter.dir/flags.make
CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o: ../src/mlxd.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o -c /home/hesam/Desktop/OCIR-Project/src/mlxd.cpp

CMakeFiles/HotSpotter.dir/src/mlxd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HotSpotter.dir/src/mlxd.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hesam/Desktop/OCIR-Project/src/mlxd.cpp > CMakeFiles/HotSpotter.dir/src/mlxd.cpp.i

CMakeFiles/HotSpotter.dir/src/mlxd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HotSpotter.dir/src/mlxd.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hesam/Desktop/OCIR-Project/src/mlxd.cpp -o CMakeFiles/HotSpotter.dir/src/mlxd.cpp.s

CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.requires:

.PHONY : CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.requires

CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.provides: CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.requires
	$(MAKE) -f CMakeFiles/HotSpotter.dir/build.make CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.provides.build
.PHONY : CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.provides

CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.provides.build: CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o


# Object files for target HotSpotter
HotSpotter_OBJECTS = \
"CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o" \
"CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o" \
"CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o" \
"CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o" \
"CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o" \
"CMakeFiles/HotSpotter.dir/src/buffer.cpp.o" \
"CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o"

# External object files for target HotSpotter
HotSpotter_EXTERNAL_OBJECTS =

HotSpotter: CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/src/buffer.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o
HotSpotter: CMakeFiles/HotSpotter.dir/build.make
HotSpotter: /usr/local/lib/libopencv_shape.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_stitching.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_superres.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_videostab.so.3.1.0
HotSpotter: libextendxmpp/libextendxmpp.a
HotSpotter: /usr/local/lib/libopencv_objdetect.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_calib3d.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_features2d.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_flann.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_highgui.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_ml.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_photo.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_video.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_videoio.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_imgproc.so.3.1.0
HotSpotter: /usr/local/lib/libopencv_core.so.3.1.0
HotSpotter: CMakeFiles/HotSpotter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hesam/Desktop/OCIR-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable HotSpotter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HotSpotter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HotSpotter.dir/build: HotSpotter

.PHONY : CMakeFiles/HotSpotter.dir/build

CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/GridEyeReader.cpp.o.requires
CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/HotSpotter.cpp.o.requires
CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/OccupancyCounter.cpp.o.requires
CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/XMPPInterface.cpp.o.requires
CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/XMPPWrapper.cpp.o.requires
CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/buffer.cpp.o.requires
CMakeFiles/HotSpotter.dir/requires: CMakeFiles/HotSpotter.dir/src/mlxd.cpp.o.requires

.PHONY : CMakeFiles/HotSpotter.dir/requires

CMakeFiles/HotSpotter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HotSpotter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HotSpotter.dir/clean

CMakeFiles/HotSpotter.dir/depend:
	cd /home/hesam/Desktop/OCIR-Project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hesam/Desktop/OCIR-Project /home/hesam/Desktop/OCIR-Project /home/hesam/Desktop/OCIR-Project/build /home/hesam/Desktop/OCIR-Project/build /home/hesam/Desktop/OCIR-Project/build/CMakeFiles/HotSpotter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HotSpotter.dir/depend
