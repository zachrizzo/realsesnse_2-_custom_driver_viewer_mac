#pragma once

#include <libusb-1.0/libusb.h>
#include <cstdint>

class USBController {
public:
    USBController();
    ~USBController();

    bool initialize();
    bool connect_to_device(uint16_t vendor_id, uint16_t product_id);
    bool claim_interface(int interface_number);
    bool bulk_transfer(unsigned char endpoint, unsigned char* data, int length,
                      int* actual_length, unsigned int timeout);

    // Get the device handle for control transfers
    libusb_device_handle* get_device_handle() const { return device_handle_; }

    // Get the libusb context
    libusb_context* get_context() const { return context_; }

    // Device constants
    static constexpr uint16_t INTEL_VENDOR_ID = 0x8086;
    static constexpr uint16_t D435I_PRODUCT_ID = 0x0b3a;

private:
    libusb_context* context_;
    libusb_device_handle* device_handle_;
    bool is_initialized_;
};
