#include "usb_controller.hpp"
#include <iostream>

USBController::USBController() : device_handle_(nullptr) {
    int result = libusb_init(nullptr);
    if (result < 0) {
        std::cerr << "Failed to initialize libusb: " << libusb_error_name(result) << std::endl;
        return;
    }
    initialized_ = true;
}

USBController::~USBController() {
    if (device_handle_) {
        libusb_close(device_handle_);
    }
    if (initialized_) {
        libusb_exit(nullptr);
    }
}

bool USBController::connect_to_device(uint16_t vendor_id, uint16_t product_id) {
    if (!initialized_) {
        std::cerr << "USB controller not initialized" << std::endl;
        return false;
    }

    // Get list of USB devices
    libusb_device **device_list;
    ssize_t device_count = libusb_get_device_list(nullptr, &device_list);
    if (device_count < 0) {
        std::cerr << "Failed to get device list: " << libusb_error_name(device_count) << std::endl;
        return false;
    }

    // Find our device
    libusb_device *target_device = nullptr;
    for (ssize_t i = 0; i < device_count; i++) {
        libusb_device_descriptor desc;
        int result = libusb_get_device_descriptor(device_list[i], &desc);
        if (result < 0) {
            std::cerr << "Failed to get device descriptor: " << libusb_error_name(result) << std::endl;
            continue;
        }

        if (desc.idVendor == vendor_id && desc.idProduct == product_id) {
            target_device = device_list[i];
            break;
        }
    }

    if (!target_device) {
        std::cerr << "Device not found (VID=0x" << std::hex << vendor_id
                  << ", PID=0x" << product_id << std::dec << ")" << std::endl;
        libusb_free_device_list(device_list, 1);
        return false;
    }

    // Open the device
    int result = libusb_open(target_device, &device_handle_);
    libusb_free_device_list(device_list, 1);

    if (result < 0) {
        std::cerr << "Failed to open device: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Reset the device
    result = libusb_reset_device(device_handle_);
    if (result < 0) {
        std::cerr << "Failed to reset device: " << libusb_error_name(result) << std::endl;
        libusb_close(device_handle_);
        device_handle_ = nullptr;
        return false;
    }

    // Set configuration
    result = libusb_set_configuration(device_handle_, 1);
    if (result < 0) {
        std::cerr << "Failed to set configuration: " << libusb_error_name(result) << std::endl;
        libusb_close(device_handle_);
        device_handle_ = nullptr;
        return false;
    }

    return true;
}

bool USBController::claim_interface(uint8_t interface_number) {
    if (!device_handle_) {
        std::cerr << "No device connected" << std::endl;
        return false;
    }

    // Detach kernel driver if necessary
    if (libusb_kernel_driver_active(device_handle_, interface_number)) {
        int result = libusb_detach_kernel_driver(device_handle_, interface_number);
        if (result < 0) {
            std::cerr << "Failed to detach kernel driver: " << libusb_error_name(result) << std::endl;
            return false;
        }
    }

    // Claim the interface
    int result = libusb_claim_interface(device_handle_, interface_number);
    if (result < 0) {
        std::cerr << "Failed to claim interface: " << libusb_error_name(result) << std::endl;
        return false;
    }

    return true;
}

bool USBController::bulk_transfer(uint8_t endpoint, uint8_t* data, int length, int* actual_length, unsigned int timeout) {
    if (!device_handle_) {
        std::cerr << "No device connected" << std::endl;
        return false;
    }

    int result = libusb_bulk_transfer(device_handle_, endpoint, data, length, actual_length, timeout);
    if (result < 0) {
        if (result != LIBUSB_ERROR_TIMEOUT) {
            std::cerr << "Bulk transfer failed: " << libusb_error_name(result) << std::endl;
        }
        return false;
    }

    return true;
}

libusb_device_handle* USBController::get_device_handle() {
    return device_handle_;
}
