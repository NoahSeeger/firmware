#include "bootloader.h"

#include "display.h"
#include <Arduino.h>
#include <soc/soc_caps.h>

#if SOC_USB_OTG_SUPPORTED && CONFIG_TINYUSB_ENABLED
#include <esp32-hal-tinyusb.h>
#endif

void enterBootloader() {
#if SOC_USB_OTG_SUPPORTED && CONFIG_TINYUSB_ENABLED
    Serial.println("Entering USB bootloader...");
    Serial.flush();
    delay(50);
    usb_persist_restart(RESTART_BOOTLOADER);
    delay(1000); // usb_persist_restart should not return
#else
    displayError("USB bootloader unavailable", true);
#endif
}
