#pragma once

#include <libusb-1.0/libusb.h>

class USBController {
public:
    static constexpr uint16_t INTEL_VENDOR_ID = 0x8086;
    static constexpr uint16_t D435I_PRODUCT_ID = 0x0b3a;

    USBController();
    ~USBController();

    bool connect_to_device(uint16_t vendor_id, uint16_t product_id);
    bool claim_interface(uint8_t interface_number);
    bool bulk_transfer(uint8_t endpoint, uint8_t* data, int length, int* actual_length, unsigned int timeout);
    libusb_device_handle* get_device_handle();

private:
    libusb_device_handle* device_handle_;
    bool initialized_;
};
