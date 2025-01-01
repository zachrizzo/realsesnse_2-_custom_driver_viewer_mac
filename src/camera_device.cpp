#include "camera_device.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

CameraDevice::CameraDevice()
    : device_handle_(nullptr), is_streaming_(false),
      DEPTH_INTERFACE(0), CONTROL_INTERFACE(1), DEPTH_ENDPOINT_IN(0x82) {
}

CameraDevice::~CameraDevice() {
    if (is_streaming_) {
        stop_streaming();
    }
    if (device_handle_) {
        libusb_release_interface(device_handle_, DEPTH_INTERFACE);
        libusb_release_interface(device_handle_, CONTROL_INTERFACE);
    }
}

bool CameraDevice::initialize() {
    usb_controller_ = std::make_unique<USBController>();
    if (!usb_controller_->connect_to_device(USBController::INTEL_VENDOR_ID, USBController::D435I_PRODUCT_ID)) {
        std::cerr << "Failed to connect to RealSense device" << std::endl;
        return false;
    }

    device_handle_ = usb_controller_->get_device_handle();
    if (!device_handle_) {
        std::cerr << "Invalid device handle" << std::endl;
        return false;
    }

    std::cout << "Initializing camera..." << std::endl;

    // Get device configuration
    libusb_config_descriptor* config;
    int result = libusb_get_active_config_descriptor(libusb_get_device(device_handle_), &config);
    if (result < 0) {
        std::cerr << "Failed to get config descriptor: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Find the depth streaming interface (Class 14, SubClass 2)
    bool found_depth_interface = false;
    for (int i = 0; i < config->bNumInterfaces; i++) {
        const libusb_interface* interface = &config->interface[i];
        for (int j = 0; j < interface->num_altsetting; j++) {
            const libusb_interface_descriptor* interface_desc = &interface->altsetting[j];

            std::cout << "Interface " << i << " Alt " << j << ":" << std::endl;
            std::cout << "  bInterfaceNumber: " << (int)interface_desc->bInterfaceNumber << std::endl;
            std::cout << "  bAlternateSetting: " << (int)interface_desc->bAlternateSetting << std::endl;
            std::cout << "  bNumEndpoints: " << (int)interface_desc->bNumEndpoints << std::endl;
            std::cout << "  bInterfaceClass: " << (int)interface_desc->bInterfaceClass << std::endl;
            std::cout << "  bInterfaceSubClass: " << (int)interface_desc->bInterfaceSubClass << std::endl;

            if (interface_desc->bInterfaceClass == 14 &&
                interface_desc->bInterfaceSubClass == 2 &&
                interface_desc->bNumEndpoints > 0) {

                // Check for bulk endpoint
                for (int k = 0; k < interface_desc->bNumEndpoints; k++) {
                    const libusb_endpoint_descriptor* endpoint = &interface_desc->endpoint[k];
                    std::cout << "  Endpoint " << k << ":" << std::endl;
                    std::cout << "    bEndpointAddress: 0x" << std::hex << (int)endpoint->bEndpointAddress << std::dec << std::endl;
                    std::cout << "    bmAttributes: 0x" << std::hex << (int)endpoint->bmAttributes << std::dec << std::endl;
                    std::cout << "    wMaxPacketSize: " << endpoint->wMaxPacketSize << std::endl;

                    // Check if this is a bulk IN endpoint
                    if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) == LIBUSB_TRANSFER_TYPE_BULK &&
                        (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {

                        DEPTH_INTERFACE = interface_desc->bInterfaceNumber;
                        DEPTH_ENDPOINT_IN = endpoint->bEndpointAddress;
                        found_depth_interface = true;
                        break;
                    }
                }
            }

            if (found_depth_interface) {
                break;
            }
        }
        if (found_depth_interface) {
            break;
        }
    }

    libusb_free_config_descriptor(config);

    if (!found_depth_interface) {
        std::cerr << "Failed to find video streaming interface with bulk endpoint" << std::endl;
        return false;
    }

    // Claim the depth interface
    if (!usb_controller_->claim_interface(DEPTH_INTERFACE)) {
        std::cerr << "Failed to claim depth interface" << std::endl;
        return false;
    }

    // Send initialization sequence
    if (!send_init_sequence()) {
        std::cerr << "Failed to send initialization sequence" << std::endl;
        return false;
    }

    return true;
}

bool CameraDevice::start_streaming() {
    if (is_streaming_) {
        return true;
    }

    // Configure depth stream
    if (!configure_depth_stream()) {
        std::cerr << "Failed to configure depth stream" << std::endl;
        return false;
    }

    // Clear any stall condition
    int result = libusb_clear_halt(device_handle_, DEPTH_ENDPOINT_IN);
    if (result < 0) {
        std::cerr << "Failed to clear endpoint halt: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Test the endpoint with exactly one packet size
    std::vector<uint8_t> test_buffer(1024);  // Match the endpoint's wMaxPacketSize
    int actual_length;

    std::cout << "Testing depth endpoint with " << test_buffer.size() << " byte buffer..." << std::endl;

    // Try multiple times in case the first few reads fail
    for (int attempt = 0; attempt < 3; attempt++) {
        if (usb_controller_->bulk_transfer(DEPTH_ENDPOINT_IN, test_buffer.data(), test_buffer.size(), &actual_length, 1000)) {
            std::cout << "Successfully read " << actual_length << " bytes from depth endpoint" << std::endl;
            is_streaming_ = true;
            return true;
        }
        std::cout << "Attempt " << attempt + 1 << " failed (received " << actual_length << " bytes), retrying..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cerr << "Failed to test depth endpoint after multiple attempts" << std::endl;
    return false;
}

bool CameraDevice::stop_streaming() {
    if (!is_streaming_) {
        return true;
    }

    is_streaming_ = false;
    return true;
}

bool CameraDevice::send_init_sequence() {
    // For now, we'll skip the initialization sequence since we need to understand
    // the exact protocol for the D435i camera
    std::cout << "Skipping initialization sequence" << std::endl;
    return true;
}

bool CameraDevice::configure_depth_stream() {
    // For now, we'll skip the stream configuration since we need to understand
    // the exact protocol for the D435i camera
    std::cout << "Skipping depth stream configuration" << std::endl;
    return true;
}

bool CameraDevice::get_depth_frame(uint8_t* buffer, size_t buffer_size) {
    if (!buffer || buffer_size != DEPTH_FRAME_SIZE) {
        std::cerr << "Invalid buffer or size" << std::endl;
        return false;
    }

    if (!is_streaming_) {
        std::cerr << "Streaming is not started" << std::endl;
        return false;
    }

    size_t total_received = 0;
    const size_t chunk_size = 1024;  // Match the endpoint's wMaxPacketSize
    std::vector<uint8_t> chunk_buffer(chunk_size);
    int zero_packets = 0;
    const int max_zero_packets = 5;

    while (total_received < buffer_size) {
        int transferred = 0;
        if (!usb_controller_->bulk_transfer(DEPTH_ENDPOINT_IN, chunk_buffer.data(), chunk_size, &transferred, 1000)) {
            if (transferred == 0) {
                zero_packets++;
                if (zero_packets >= max_zero_packets) {
                    std::cout << "Received too many zero-length packets, starting new frame" << std::endl;
                    return false;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::cerr << "Failed to read from endpoint" << std::endl;
            return false;
        }

        zero_packets = 0;
        if (total_received + transferred > buffer_size) {
            std::cerr << "Buffer overflow, truncating frame" << std::endl;
            transferred = buffer_size - total_received;
        }

        memcpy(buffer + total_received, chunk_buffer.data(), transferred);
        total_received += transferred;
        std::cout << "Received " << transferred << " bytes (" << total_received << "/" << buffer_size << ")" << std::endl;
    }

    return true;
}
