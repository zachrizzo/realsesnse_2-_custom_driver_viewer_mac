# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/zachrizzo/programing/realsesnse_2 _river_custom"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/zachrizzo/programing/realsesnse_2 _river_custom/build"

# Include any dependencies generated for this target.
include CMakeFiles/realsense_driver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/realsense_driver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/realsense_driver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/realsense_driver.dir/flags.make

CMakeFiles/realsense_driver.dir/codegen:
.PHONY : CMakeFiles/realsense_driver.dir/codegen

CMakeFiles/realsense_driver.dir/src/main.cpp.o: CMakeFiles/realsense_driver.dir/flags.make
CMakeFiles/realsense_driver.dir/src/main.cpp.o: /Users/zachrizzo/programing/realsesnse_2\ _river_custom/src/main.cpp
CMakeFiles/realsense_driver.dir/src/main.cpp.o: CMakeFiles/realsense_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/zachrizzo/programing/realsesnse_2 _river_custom/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/realsense_driver.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/realsense_driver.dir/src/main.cpp.o -MF CMakeFiles/realsense_driver.dir/src/main.cpp.o.d -o CMakeFiles/realsense_driver.dir/src/main.cpp.o -c "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/main.cpp"

CMakeFiles/realsense_driver.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/realsense_driver.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/main.cpp" > CMakeFiles/realsense_driver.dir/src/main.cpp.i

CMakeFiles/realsense_driver.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/realsense_driver.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/main.cpp" -o CMakeFiles/realsense_driver.dir/src/main.cpp.s

CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o: CMakeFiles/realsense_driver.dir/flags.make
CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o: /Users/zachrizzo/programing/realsesnse_2\ _river_custom/src/usb_controller.cpp
CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o: CMakeFiles/realsense_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/zachrizzo/programing/realsesnse_2 _river_custom/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o -MF CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o.d -o CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o -c "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/usb_controller.cpp"

CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/usb_controller.cpp" > CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.i

CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/usb_controller.cpp" -o CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.s

CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o: CMakeFiles/realsense_driver.dir/flags.make
CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o: /Users/zachrizzo/programing/realsesnse_2\ _river_custom/src/camera_device.cpp
CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o: CMakeFiles/realsense_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/zachrizzo/programing/realsesnse_2 _river_custom/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o -MF CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o.d -o CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o -c "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/camera_device.cpp"

CMakeFiles/realsense_driver.dir/src/camera_device.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/realsense_driver.dir/src/camera_device.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/camera_device.cpp" > CMakeFiles/realsense_driver.dir/src/camera_device.cpp.i

CMakeFiles/realsense_driver.dir/src/camera_device.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/realsense_driver.dir/src/camera_device.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/camera_device.cpp" -o CMakeFiles/realsense_driver.dir/src/camera_device.cpp.s

CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o: CMakeFiles/realsense_driver.dir/flags.make
CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o: /Users/zachrizzo/programing/realsesnse_2\ _river_custom/src/visualizer.cpp
CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o: CMakeFiles/realsense_driver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/zachrizzo/programing/realsesnse_2 _river_custom/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o -MF CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o.d -o CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o -c "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/visualizer.cpp"

CMakeFiles/realsense_driver.dir/src/visualizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/realsense_driver.dir/src/visualizer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/visualizer.cpp" > CMakeFiles/realsense_driver.dir/src/visualizer.cpp.i

CMakeFiles/realsense_driver.dir/src/visualizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/realsense_driver.dir/src/visualizer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/zachrizzo/programing/realsesnse_2 _river_custom/src/visualizer.cpp" -o CMakeFiles/realsense_driver.dir/src/visualizer.cpp.s

# Object files for target realsense_driver
realsense_driver_OBJECTS = \
"CMakeFiles/realsense_driver.dir/src/main.cpp.o" \
"CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o" \
"CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o" \
"CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o"

# External object files for target realsense_driver
realsense_driver_EXTERNAL_OBJECTS =

realsense_driver: CMakeFiles/realsense_driver.dir/src/main.cpp.o
realsense_driver: CMakeFiles/realsense_driver.dir/src/usb_controller.cpp.o
realsense_driver: CMakeFiles/realsense_driver.dir/src/camera_device.cpp.o
realsense_driver: CMakeFiles/realsense_driver.dir/src/visualizer.cpp.o
realsense_driver: CMakeFiles/realsense_driver.dir/build.make
realsense_driver: /opt/homebrew/lib/libusb-1.0.dylib
realsense_driver: /opt/homebrew/lib/libglfw.3.4.dylib
realsense_driver: CMakeFiles/realsense_driver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/zachrizzo/programing/realsesnse_2 _river_custom/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable realsense_driver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/realsense_driver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/realsense_driver.dir/build: realsense_driver
.PHONY : CMakeFiles/realsense_driver.dir/build

CMakeFiles/realsense_driver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/realsense_driver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/realsense_driver.dir/clean

CMakeFiles/realsense_driver.dir/depend:
	cd "/Users/zachrizzo/programing/realsesnse_2 _river_custom/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/zachrizzo/programing/realsesnse_2 _river_custom" "/Users/zachrizzo/programing/realsesnse_2 _river_custom" "/Users/zachrizzo/programing/realsesnse_2 _river_custom/build" "/Users/zachrizzo/programing/realsesnse_2 _river_custom/build" "/Users/zachrizzo/programing/realsesnse_2 _river_custom/build/CMakeFiles/realsense_driver.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/realsense_driver.dir/depend
