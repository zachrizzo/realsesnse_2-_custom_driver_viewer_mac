#include "camera_device.hpp"
#include "visualizer.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    // Create camera device
    CameraDevice camera;
    if (!camera.initialize()) {
        std::cerr << "Failed to initialize camera" << std::endl;
        return 1;
    }

    // Create visualizer
    Visualizer visualizer(848, 480);
    if (!visualizer.initialize()) {
        std::cerr << "Failed to initialize visualizer" << std::endl;
        return 1;
    }

    // Start camera streaming
    if (!camera.start_streaming()) {
        std::cerr << "Failed to start streaming" << std::endl;
        return 1;
    }

    std::cout << "Starting depth stream. Press ESC to exit." << std::endl;

    // Main loop
    while (!visualizer.should_close()) {
        DepthFrame frame;
        if (camera.get_depth_frame(frame)) {
            visualizer.render_frame(frame);
        }

        visualizer.process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }

    // Cleanup
    camera.stop_streaming();

    return 0;
}
