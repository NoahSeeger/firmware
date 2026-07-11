#ifndef __BRUCE_BOOTLOADER_H__
#define __BRUCE_BOOTLOADER_H__

// Reboot the ESP32-S3 into its ROM USB/UART bootloader when the target
// provides the native USB persistence API.
void enterBootloader();

#endif
