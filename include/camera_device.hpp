#pragma once

#include <libusb-1.0/libusb.h>
#include <memory>
#include <vector>
#include "usb_controller.hpp"

class CameraDevice {
public:
    static constexpr int DEFAULT_WIDTH = 640;
    static constexpr int DEFAULT_HEIGHT = 480;
    static constexpr size_t DEPTH_FRAME_SIZE = DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(uint16_t);

    CameraDevice();
    ~CameraDevice();

    bool initialize();
    bool start_streaming();
    bool stop_streaming();
    bool get_depth_frame(uint8_t* buffer, size_t buffer_size);

private:
    bool send_init_sequence();
    bool configure_depth_stream();

    std::unique_ptr<USBController> usb_controller_;
    libusb_device_handle* device_handle_;
    bool is_streaming_;

    // Interface and endpoint numbers (will be set during initialization)
    uint8_t DEPTH_INTERFACE;
    uint8_t CONTROL_INTERFACE;
    uint8_t DEPTH_ENDPOINT_IN;
};
