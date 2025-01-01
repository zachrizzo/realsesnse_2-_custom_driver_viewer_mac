#include "usb_controller.hpp"
#include <iostream>
#include <thread>

USBController::USBController() : context_(nullptr), device_handle_(nullptr), is_initialized_(false) {}

USBController::~USBController() {
    if (device_handle_) {
        libusb_close(device_handle_);
    }
    if (context_) {
        libusb_exit(context_);
    }
}

bool USBController::initialize() {
    if (is_initialized_) {
        return true;
    }

    int result = libusb_init(&context_);
    if (result < 0) {
        std::cerr << "Failed to initialize libusb: " << libusb_error_name(result) << std::endl;
        return false;
    }

    libusb_set_debug(context_, LIBUSB_LOG_LEVEL_INFO);
    is_initialized_ = true;
    return true;
}

bool USBController::connect_to_device(uint16_t vendor_id, uint16_t product_id) {
    if (!is_initialized_) {
        std::cerr << "USB controller not initialized" << std::endl;
        return false;
    }

    // List all USB devices to help with debugging
    libusb_device **list;
    ssize_t count = libusb_get_device_list(context_, &list);
    std::cout << "Found " << count << " USB devices" << std::endl;

    libusb_device *target_device = nullptr;
    for (ssize_t i = 0; i < count; i++) {
        libusb_device *device = list[i];
        libusb_device_descriptor desc;
        int result = libusb_get_device_descriptor(device, &desc);
        if (result < 0) {
            continue;
        }

        std::cout << "Device " << i << ": VID=" << std::hex << desc.idVendor
                  << " PID=" << desc.idProduct << std::dec
                  << " (Bus=" << (int)libusb_get_bus_number(device)
                  << " Port=" << (int)libusb_get_port_number(device) << ")" << std::endl;

        if (desc.idVendor == vendor_id && desc.idProduct == product_id) {
            target_device = device;
            libusb_ref_device(target_device);
            break;
        }
    }

    if (!target_device) {
        std::cerr << "Failed to find device " << std::hex << vendor_id << ":" << product_id << std::endl;
        std::cerr << "Make sure you have the necessary permissions and the device is connected" << std::endl;
        libusb_free_device_list(list, 1);
        return false;
    }

    // Close the existing device if it's open
    if (device_handle_) {
        libusb_close(device_handle_);
        device_handle_ = nullptr;
    }

    // Open the device
    int result = libusb_open(target_device, &device_handle_);
    libusb_unref_device(target_device);
    libusb_free_device_list(list, 1);

    if (result < 0) {
        std::cerr << "Failed to open device: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Get the active configuration
    int config = 0;
    result = libusb_get_configuration(device_handle_, &config);
    if (result < 0) {
        std::cerr << "Failed to get configuration: " << libusb_error_name(result) << std::endl;
    } else {
        std::cout << "Current configuration: " << config << std::endl;
    }

    // Set configuration if it's not already set to 1
    if (config != 1) {
        result = libusb_set_configuration(device_handle_, 1);
        if (result < 0) {
            std::cerr << "Failed to set configuration: " << libusb_error_name(result) << std::endl;
            return false;
        }
    }

    // Reset the device
    result = libusb_reset_device(device_handle_);
    if (result < 0) {
        std::cerr << "Failed to reset device: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // On macOS, we don't need to detach the kernel driver
    #ifndef __APPLE__
    if (libusb_kernel_driver_active(device_handle_, 0) == 1) {
        if (libusb_detach_kernel_driver(device_handle_, 0) != 0) {
            std::cerr << "Failed to detach kernel driver" << std::endl;
            return false;
        }
    }
    #endif

    return true;
}

bool USBController::claim_interface(int interface_number) {
    if (!device_handle_) {
        std::cerr << "No device connected" << std::endl;
        return false;
    }

    // Get the device and configuration descriptor
    libusb_device *device = libusb_get_device(device_handle_);
    libusb_config_descriptor *config;
    int result = libusb_get_active_config_descriptor(device, &config);
    if (result < 0) {
        std::cerr << "Failed to get config descriptor: " << libusb_error_name(result) << std::endl;
        return false;
    }

    // Find the interface and its endpoints
    bool interface_found = false;
    for (int i = 0; i < config->bNumInterfaces; i++) {
        const libusb_interface *interface = &config->interface[i];
        std::cout << "Interface " << i << " has " << interface->num_altsetting << " alternate settings" << std::endl;

        for (int j = 0; j < interface->num_altsetting; j++) {
            const libusb_interface_descriptor *interface_desc = &interface->altsetting[j];

            std::cout << "  Alt " << j << ": Interface number=" << (int)interface_desc->bInterfaceNumber
                      << ", Class=" << (int)interface_desc->bInterfaceClass
                      << ", SubClass=" << (int)interface_desc->bInterfaceSubClass
                      << ", Protocol=" << (int)interface_desc->bInterfaceProtocol
                      << ", Endpoints=" << (int)interface_desc->bNumEndpoints << std::endl;

            if (interface_desc->bInterfaceNumber == interface_number) {
                interface_found = true;

                // Print endpoint information
                for (int k = 0; k < interface_desc->bNumEndpoints; k++) {
                    const libusb_endpoint_descriptor *endpoint_desc = &interface_desc->endpoint[k];
                    std::cout << "    Endpoint " << k << ": "
                              << "Address=0x" << std::hex << (int)endpoint_desc->bEndpointAddress
                              << " Type=0x" << (int)endpoint_desc->bmAttributes
                              << " MaxPacketSize=" << std::dec << endpoint_desc->wMaxPacketSize
                              << std::endl;
                }

                // Release the interface if it's already claimed
                libusb_release_interface(device_handle_, interface_number);

                // Claim the interface
                result = libusb_claim_interface(device_handle_, interface_number);
                if (result < 0) {
                    std::cerr << "Failed to claim interface: " << libusb_error_name(result) << std::endl;
                    libusb_free_config_descriptor(config);
                    return false;
                }

                // Set alternate setting
                result = libusb_set_interface_alt_setting(device_handle_, interface_number, j);
                if (result < 0) {
                    std::cerr << "Failed to set alternate setting: " << libusb_error_name(result) << std::endl;
                    // Continue anyway, as this might not be fatal
                }

                std::cout << "Successfully claimed interface " << interface_number << std::endl;
            }
        }
    }

    libusb_free_config_descriptor(config);

    if (!interface_found) {
        std::cerr << "Interface " << interface_number << " not found" << std::endl;
        return false;
    }

    return true;
}

bool USBController::bulk_transfer(unsigned char endpoint, unsigned char* data, int length,
                                int* actual_length, unsigned int timeout) {
    if (!device_handle_) {
        std::cerr << "No device connected" << std::endl;
        return false;
    }

    int result = libusb_bulk_transfer(device_handle_, endpoint, data, length, actual_length, timeout);
    if (result < 0) {
        std::cerr << "Bulk transfer failed: " << libusb_error_name(result) << std::endl;
        return false;
    }

    return true;
}
