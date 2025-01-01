#pragma once

#include "usb_controller.hpp"
#include <memory>
#include <vector>
#include <cstdint>

struct DepthFrame {
    int width;
    int height;
    std::vector<uint16_t> depth_data;
};

class CameraDevice {
public:
    CameraDevice();
    ~CameraDevice();

    bool initialize();
    bool start_streaming();
    bool stop_streaming();
    bool get_depth_frame(DepthFrame& frame);

private:
    bool send_init_sequence();
    bool configure_depth_stream();

    std::unique_ptr<USBController> usb_controller_;
    bool is_streaming_;

    // Camera constants
    static constexpr int DEFAULT_WIDTH = 640;
    static constexpr int DEFAULT_HEIGHT = 480;

    // Interface numbers (based on USB descriptor)
    static constexpr int CONTROL_INTERFACE = 0;  // Class 14, SubClass 1
    static constexpr int DEPTH_INTERFACE = 1;    // Class 14, SubClass 2

    // Endpoint addresses (based on USB descriptor)
    static constexpr uint8_t CONTROL_ENDPOINT_IN = 0x87;   // Interface 0, Endpoint 0
    static constexpr uint8_t DEPTH_ENDPOINT_IN = 0x82;     // Interface 1, Endpoint 0
};
