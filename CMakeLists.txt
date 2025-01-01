cmake_minimum_required(VERSION 3.10)
project(RealSenseCustomDriver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Include directories
include_directories(${LIBUSB_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(include)

# Add source files
add_executable(realsense_driver
    src/main.cpp
    src/usb_controller.cpp
    src/camera_device.cpp
    src/visualizer.cpp
)

# Link directories
link_directories(${LIBUSB_LIBRARY_DIRS})

# Set the path to libusb
set(LIBUSB_PATH "/opt/homebrew/lib")

# Link libraries
target_link_libraries(realsense_driver
    ${LIBUSB_PATH}/libusb-1.0.dylib
    glfw
    "-framework OpenGL"
    "-framework CoreFoundation"
    "-framework IOKit"
)
