#include <iostream>
#include "visualizer.hpp"
#include "camera_device.hpp"

int main() {
    CameraDevice camera;
    if (!camera.initialize()) {
        std::cerr << "Failed to initialize camera" << std::endl;
        return -1;
    }

    if (!camera.start_streaming()) {
        std::cerr << "Failed to start streaming" << std::endl;
        return -1;
    }

    Visualizer visualizer(CameraDevice::DEFAULT_WIDTH, CameraDevice::DEFAULT_HEIGHT);
    if (!visualizer.initialize()) {
        std::cerr << "Failed to initialize visualizer" << std::endl;
        return -1;
    }

    std::vector<uint8_t> depth_buffer(CameraDevice::DEPTH_FRAME_SIZE);

    while (!visualizer.should_close()) {
        if (camera.get_depth_frame(depth_buffer.data(), depth_buffer.size())) {
            visualizer.render_frame(depth_buffer.data());
        }
        visualizer.process_events();
    }

    camera.stop_streaming();
    return 0;
}
