#include "camera_device.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

CameraDevice::CameraDevice() : is_streaming_(false) {
    usb_controller_ = std::make_unique<USBController>();
}

CameraDevice::~CameraDevice() {
    if (is_streaming_) {
        stop_streaming();
    }
}

bool CameraDevice::initialize() {
    if (!usb_controller_->initialize()) {
        std::cerr << "Failed to initialize USB controller" << std::endl;
        return false;
    }

    if (!usb_controller_->connect_to_device(USBController::INTEL_VENDOR_ID, USBController::D435I_PRODUCT_ID)) {
        std::cerr << "Failed to connect to RealSense device" << std::endl;
        return false;
    }

    // Get device descriptor
    libusb_device *device = libusb_get_device(usb_controller_->get_device_handle());
    libusb_device_descriptor desc;
    int result = libusb_get_device_descriptor(device, &desc);
    if (result < 0) {
        std::cerr << "Failed to get device descriptor: " << libusb_error_name(result) << std::endl;
        return false;
    }

    std::cout << "Device Descriptor:" << std::endl;
    std::cout << "  bLength: " << (int)desc.bLength << std::endl;
    std::cout << "  bDescriptorType: " << (int)desc.bDescriptorType << std::endl;
    std::cout << "  bcdUSB: 0x" << std::hex << desc.bcdUSB << std::dec << std::endl;
    std::cout << "  bDeviceClass: " << (int)desc.bDeviceClass << std::endl;
    std::cout << "  bDeviceSubClass: " << (int)desc.bDeviceSubClass << std::endl;
    std::cout << "  bDeviceProtocol: " << (int)desc.bDeviceProtocol << std::endl;
    std::cout << "  bMaxPacketSize0: " << (int)desc.bMaxPacketSize0 << std::endl;
    std::cout << "  idVendor: 0x" << std::hex << desc.idVendor << std::dec << std::endl;
    std::cout << "  idProduct: 0x" << std::hex << desc.idProduct << std::dec << std::endl;
    std::cout << "  bcdDevice: 0x" << std::hex << desc.bcdDevice << std::dec << std::endl;

    // Get configuration descriptor
    libusb_config_descriptor *config;
    result = libusb_get_active_config_descriptor(device, &config);
    if (result < 0) {
        std::cerr << "Failed to get config descriptor: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Print configuration information
    std::cout << "Configuration:" << std::endl;
    std::cout << "  bNumInterfaces: " << (int)config->bNumInterfaces << std::endl;
    std::cout << "  bConfigurationValue: " << (int)config->bConfigurationValue << std::endl;

    // Set configuration
    result = libusb_set_configuration(usb_controller_->get_device_handle(), config->bConfigurationValue);
    if (result < 0) {
        std::cerr << "Failed to set configuration: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    // First, claim the control interface
    if (!usb_controller_->claim_interface(CONTROL_INTERFACE)) {
        std::cerr << "Failed to claim control interface" << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    // Then claim the depth interface
    if (!usb_controller_->claim_interface(DEPTH_INTERFACE)) {
        std::cerr << "Failed to claim depth interface" << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    libusb_free_config_descriptor(config);

    if (!send_init_sequence()) {
        std::cerr << "Failed to initialize camera" << std::endl;
        return false;
    }

    return true;
}

bool CameraDevice::send_init_sequence() {
    // Skip device status read since it's not supported
    std::cout << "Initializing camera..." << std::endl;

    // Wait for the camera to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return configure_depth_stream();
}

bool CameraDevice::configure_depth_stream() {
    // Get the device and configuration descriptor
    libusb_device *device = libusb_get_device(usb_controller_->get_device_handle());
    libusb_config_descriptor *config;
    int result = libusb_get_active_config_descriptor(device, &config);
    if (result < 0) {
        std::cerr << "Failed to get config descriptor: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Find the depth interface and its endpoints
    bool interface_found = false;
    for (int i = 0; i < config->bNumInterfaces; i++) {
        const libusb_interface *interface = &config->interface[i];
        for (int j = 0; j < interface->num_altsetting; j++) {
            const libusb_interface_descriptor *interface_desc = &interface->altsetting[j];

            if (interface_desc->bInterfaceNumber == DEPTH_INTERFACE) {
                interface_found = true;
                std::cout << "Found depth interface:" << std::endl;
                std::cout << "  bInterfaceNumber: " << (int)interface_desc->bInterfaceNumber << std::endl;
                std::cout << "  bAlternateSetting: " << (int)interface_desc->bAlternateSetting << std::endl;
                std::cout << "  bNumEndpoints: " << (int)interface_desc->bNumEndpoints << std::endl;
                std::cout << "  bInterfaceClass: " << (int)interface_desc->bInterfaceClass << std::endl;
                std::cout << "  bInterfaceSubClass: " << (int)interface_desc->bInterfaceSubClass << std::endl;

                // Print endpoint information
                for (int k = 0; k < interface_desc->bNumEndpoints; k++) {
                    const libusb_endpoint_descriptor *endpoint = &interface_desc->endpoint[k];
                    std::cout << "  Endpoint " << k << ":" << std::endl;
                    std::cout << "    bEndpointAddress: 0x" << std::hex << (int)endpoint->bEndpointAddress << std::dec << std::endl;
                    std::cout << "    bmAttributes: 0x" << std::hex << (int)endpoint->bmAttributes << std::dec << std::endl;
                    std::cout << "    wMaxPacketSize: " << endpoint->wMaxPacketSize << std::endl;
                }

                // Try to set alternate setting
                result = libusb_set_interface_alt_setting(
                    usb_controller_->get_device_handle(),
                    DEPTH_INTERFACE,
                    interface_desc->bAlternateSetting
                );

                if (result < 0) {
                    std::cerr << "Failed to set alternate setting " << interface_desc->bAlternateSetting
                              << ": " << libusb_error_name(result) << std::endl;
                } else {
                    std::cout << "Successfully set alternate setting " << interface_desc->bAlternateSetting << std::endl;
                }
            }
        }
    }

    libusb_free_config_descriptor(config);

    if (!interface_found) {
        std::cerr << "Depth interface not found" << std::endl;
        return false;
    }

    // Wait for the configuration to take effect
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return true;
}

bool CameraDevice::start_streaming() {
    if (is_streaming_) {
        return true;
    }

    // Reset the endpoint before starting
    int result = libusb_clear_halt(usb_controller_->get_device_handle(), DEPTH_ENDPOINT_IN);
    if (result < 0) {
        std::cerr << "Failed to clear endpoint halt: " << libusb_error_name(result) << std::endl;
        // Continue anyway, as this might not be fatal
    }

    // Get UVC streaming interface descriptor
    libusb_device *device = libusb_get_device(usb_controller_->get_device_handle());
    libusb_config_descriptor *config;
    result = libusb_get_active_config_descriptor(device, &config);
    if (result < 0) {
        std::cerr << "Failed to get config descriptor: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Find the video streaming interface
    const libusb_interface *interface = nullptr;
    const libusb_interface_descriptor *interface_desc = nullptr;
    int alt_setting = 0;
    bool found_streaming_interface = false;

    for (int i = 0; i < config->bNumInterfaces; i++) {
        interface = &config->interface[i];
        for (int j = 0; j < interface->num_altsetting; j++) {
            interface_desc = &interface->altsetting[j];
            if (interface_desc->bInterfaceNumber == DEPTH_INTERFACE &&
                interface_desc->bInterfaceClass == 14 &&    // Video class
                interface_desc->bInterfaceSubClass == 2) {  // Video Streaming

                // Look for an alternate setting with a bulk endpoint
                for (int k = 0; k < interface_desc->bNumEndpoints; k++) {
                    const libusb_endpoint_descriptor *endpoint = &interface_desc->endpoint[k];
                    if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) == LIBUSB_TRANSFER_TYPE_BULK &&
                        (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
                        alt_setting = j;
                        found_streaming_interface = true;
                        break;
                    }
                }
                if (found_streaming_interface) {
                    break;
                }
            }
        }
        if (found_streaming_interface) {
            break;
        }
    }

    if (!found_streaming_interface) {
        std::cerr << "Failed to find video streaming interface with bulk endpoint" << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    std::cout << "Found video streaming interface:" << std::endl;
    std::cout << "  bInterfaceNumber: " << (int)interface_desc->bInterfaceNumber << std::endl;
    std::cout << "  bAlternateSetting: " << alt_setting << std::endl;
    std::cout << "  bNumEndpoints: " << (int)interface_desc->bNumEndpoints << std::endl;
    std::cout << "  bInterfaceClass: " << (int)interface_desc->bInterfaceClass << std::endl;
    std::cout << "  bInterfaceSubClass: " << (int)interface_desc->bInterfaceSubClass << std::endl;

    // Release and reclaim the interface with the new alternate setting
    std::cout << "Releasing interface..." << std::endl;
    libusb_release_interface(usb_controller_->get_device_handle(), DEPTH_INTERFACE);

    std::cout << "Claiming interface..." << std::endl;
    result = libusb_claim_interface(usb_controller_->get_device_handle(), DEPTH_INTERFACE);
    if (result < 0) {
        std::cerr << "Failed to claim interface: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    // Set up video probe control
    unsigned char probe_data[26] = {0};
    probe_data[0] = 0x01;  // bmHint: dwFrameInterval field is valid
    probe_data[2] = 0x01;  // bFormatIndex
    probe_data[3] = 0x01;  // bFrameIndex
    *(uint32_t*)&probe_data[4] = 1000000 / 30;  // dwFrameInterval (30 fps)
    *(uint32_t*)&probe_data[20] = DEFAULT_WIDTH * DEFAULT_HEIGHT * 2;  // dwMaxVideoFrameSize

    // Send probe control
    std::cout << "Sending video probe control..." << std::endl;
    result = libusb_control_transfer(
        usb_controller_->get_device_handle(),
        LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
        0x01,  // SET_CUR
        0x0100,  // VS_PROBE_CONTROL
        DEPTH_INTERFACE,
        probe_data,
        sizeof(probe_data),
        1000
    );

    if (result < 0) {
        std::cerr << "Failed to send probe control: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    // Get probe control result
    std::cout << "Getting probe control result..." << std::endl;
    result = libusb_control_transfer(
        usb_controller_->get_device_handle(),
        LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
        0x81,  // GET_CUR
        0x0100,  // VS_PROBE_CONTROL
        DEPTH_INTERFACE,
        probe_data,
        sizeof(probe_data),
        1000
    );

    if (result < 0) {
        std::cerr << "Failed to get probe control result: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    std::cout << "Probe control result:" << std::endl;
    std::cout << "  bFormatIndex: " << (int)probe_data[2] << std::endl;
    std::cout << "  bFrameIndex: " << (int)probe_data[3] << std::endl;
    std::cout << "  dwFrameInterval: " << *(uint32_t*)&probe_data[4] << std::endl;
    std::cout << "  dwMaxVideoFrameSize: " << *(uint32_t*)&probe_data[20] << std::endl;

    // Send commit control
    std::cout << "Sending commit control..." << std::endl;
    result = libusb_control_transfer(
        usb_controller_->get_device_handle(),
        LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
        0x01,  // SET_CUR
        0x0200,  // VS_COMMIT_CONTROL
        DEPTH_INTERFACE,
        probe_data,
        sizeof(probe_data),
        1000
    );

    if (result < 0) {
        std::cerr << "Failed to send commit control: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    // Set alternate setting to enable streaming
    std::cout << "Setting alternate interface..." << std::endl;
    result = libusb_set_interface_alt_setting(
        usb_controller_->get_device_handle(),
        DEPTH_INTERFACE,
        alt_setting
    );

    if (result < 0) {
        std::cerr << "Failed to set alternate interface: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    // Wait for the device to stabilize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Try a single synchronous bulk transfer to test the endpoint
    unsigned char test_buffer[1024];  // Max packet size from endpoint descriptor
    int actual_length;
    result = libusb_bulk_transfer(
        usb_controller_->get_device_handle(),
        DEPTH_ENDPOINT_IN,
        test_buffer,
        sizeof(test_buffer),
        &actual_length,
        1000  // Longer timeout for initial test
    );

    if (result < 0) {
        std::cerr << "Failed to test depth endpoint: " << libusb_error_name(result) << std::endl;
        libusb_free_config_descriptor(config);
        return false;
    }

    std::cout << "Successfully tested depth endpoint, received " << actual_length << " bytes" << std::endl;
    libusb_free_config_descriptor(config);

    is_streaming_ = true;
    return true;
}

bool CameraDevice::stop_streaming() {
    if (!is_streaming_) {
        return true;
    }

    is_streaming_ = false;
    return true;
}

bool CameraDevice::get_depth_frame(DepthFrame& frame) {
    if (!is_streaming_) {
        std::cerr << "Camera is not streaming" << std::endl;
        return false;
    }

    // Try reading smaller chunks of data first
    const int chunk_size = 1024;  // Max packet size from endpoint descriptor
    const int frame_size = DEFAULT_WIDTH * DEFAULT_HEIGHT * sizeof(uint16_t);
    std::vector<unsigned char> buffer(frame_size);
    int total_received = 0;
    int actual_length;
    int retries = 0;
    const int max_retries = 3;

    while (total_received < frame_size && retries < max_retries) {
        int to_read = std::min(chunk_size, frame_size - total_received);
        int result = libusb_bulk_transfer(
            usb_controller_->get_device_handle(),
            DEPTH_ENDPOINT_IN,
            buffer.data() + total_received,
            to_read,
            &actual_length,
            1000   // Timeout in milliseconds
        );

        if (result < 0) {
            if (result == LIBUSB_ERROR_NO_DEVICE || result == LIBUSB_ERROR_IO) {
                // Try to reconnect to the device
                std::cout << "Lost connection, attempting to reconnect..." << std::endl;

                // Release the interfaces first
                libusb_release_interface(usb_controller_->get_device_handle(), DEPTH_INTERFACE);
                libusb_release_interface(usb_controller_->get_device_handle(), CONTROL_INTERFACE);

                // Close and reopen the device
                if (!usb_controller_->connect_to_device(USBController::INTEL_VENDOR_ID, USBController::D435I_PRODUCT_ID)) {
                    std::cerr << "Failed to reconnect to device" << std::endl;
                    return false;
                }

                // Reclaim the interfaces
                if (!usb_controller_->claim_interface(CONTROL_INTERFACE)) {
                    std::cerr << "Failed to reclaim control interface" << std::endl;
                    return false;
                }
                if (!usb_controller_->claim_interface(DEPTH_INTERFACE)) {
                    std::cerr << "Failed to reclaim depth interface" << std::endl;
                    return false;
                }

                retries++;
                continue;
            }
            std::cerr << "Failed to read depth data chunk: " << libusb_error_name(result) << std::endl;
            return false;
        }

        total_received += actual_length;
        std::cout << "Received " << actual_length << " bytes (" << total_received << "/" << frame_size << ")" << std::endl;

        if (actual_length == 0) {
            std::cerr << "Zero-length packet received" << std::endl;
            retries++;
            continue;
        }
    }

    if (total_received != frame_size) {
        std::cerr << "Incomplete frame received (got " << total_received << " bytes, expected " << frame_size << ")" << std::endl;
        return false;
    }

    // Convert buffer to depth data
    frame.width = DEFAULT_WIDTH;
    frame.height = DEFAULT_HEIGHT;
    frame.depth_data.resize(DEFAULT_WIDTH * DEFAULT_HEIGHT);
    memcpy(frame.depth_data.data(), buffer.data(), frame_size);

    return true;
}
