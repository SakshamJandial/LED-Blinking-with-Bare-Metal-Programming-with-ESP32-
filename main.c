#include <stdint.h>

// Register definitions for GPIO
#define GPIO_OUT_W1TS_REG       ((volatile uint32_t*)0x3FF44008)
#define GPIO_OUT_W1TC_REG       ((volatile uint32_t*)0x3FF4400C)
#define GPIO_ENABLE_W1TS_REG    ((volatile uint32_t*)0x3FF44024)

// Timer Group 0 Watchdog
#define TIMG0_WDTWPROTECT_REG ((volatile uint32_t*)0x3FF5F064)
#define TIMG0_WDTCONFIG0_REG  ((volatile uint32_t*)0x3FF5F048)

// RTC Watchdog
#define RTC_CNTL_WDTWPROTECT_REG ((volatile uint32_t*)0x3FF480A4)
#define RTC_CNTL_WDTCONFIG0_REG  ((volatile uint32_t*)0x3FF4808C)

// Magic value to disable write protection
#define WDT_WKEY_VALUE 0x50D83AA1

#define BLINK_GPIO 4

// Delay function
void simple_delay(uint32_t ticks) {
    for (uint32_t i = 0; i < ticks; i++) {
    }
}

void app_main() {
    // Disable the watchdogs
    *RTC_CNTL_WDTWPROTECT_REG = WDT_WKEY_VALUE; // Unlock RTC WDT
    *RTC_CNTL_WDTCONFIG0_REG = 0;               // Disable
    *RTC_CNTL_WDTWPROTECT_REG = 0;               // Lock

    *TIMG0_WDTWPROTECT_REG = WDT_WKEY_VALUE; // Unlock TIMG0 WDT
    *TIMG0_WDTCONFIG0_REG = 0;               // Disable
    *TIMG0_WDTWPROTECT_REG = 0;               // Lock

    // GPIO pin to be an output.
    *GPIO_ENABLE_W1TS_REG = (1 << BLINK_GPIO);

    while (1) {

        //LED on
        *GPIO_OUT_W1TS_REG = (1 << BLINK_GPIO);
        simple_delay(3000000); 

        //LED off
        *GPIO_OUT_W1TC_REG = (1 << BLINK_GPIO);
        simple_delay(3000000);
    }
}