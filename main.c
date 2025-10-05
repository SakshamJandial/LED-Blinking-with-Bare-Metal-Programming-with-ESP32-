#include <stdint.h>

// --- ADDED --- Clock Configuration Registers
#define RTC_CNTL_CLK_CONF_REG   ((volatile uint32_t*)0x3FF48070)
#define RTC_CNTL_PLL_CONF_REG   ((volatile uint32_t*)0x3FF48084)
#define DPORT_CPU_PER_CONF_REG  ((volatile uint32_t*)0x3FF000A0)

// Register definitions for GPIO
#define GPIO_OUT_W1TS_REG   ((volatile uint32_t*)0x3FF44008)
#define GPIO_OUT_W1TC_REG   ((volatile uint32_t*)0x3FF4400C)
#define GPIO_ENABLE_W1TS_REG ((volatile uint32_t*)0x3FF44024)

// --- ADDED --- Definitions for enabling the GPIO peripheral clock
#define DPORT_PERIP_CLK_EN_REG  ((volatile uint32_t*)0x3FF000C0)
#define DPORT_GPIO_CLK_EN       (1 << 4)

// Register definitions for Timer Group 0 Watchdog (already present in your code)
#define TIMG0_WDTWPROTECT_REG ((volatile uint32_t*)0x3FF5F064)
#define TIMG0_WDTCONFIG0_REG  ((volatile uint32_t*)0x3FF5F048)

// Register definitions for RTC Watchdog (already present in your code)
#define RTC_CNTL_WDTWPROTECT_REG ((volatile uint32_t*)0x3FF480A4)
#define RTC_CNTL_WDTCONFIG0_REG  ((volatile uint32_t*)0x3FF4808C)

// Magic value to disable write protection (already present in your code)
#define WDT_WKEY_VALUE 0x50D83AA1

#define BLINK_GPIO 4        // The GPIO pin

// --- ADDED --- New function to configure the CPU clock to 240MHz
void configure_clock_to_240mhz() {
    // Switch CPU clock source to PLL (Phase-Locked Loop)
    *RTC_CNTL_CLK_CONF_REG |= (1 << 8); // SOC_CLK_SEL = 1 (PLL)

    // Configure the PLL to 480 MHz for a 40 MHz crystal.
    *RTC_CNTL_PLL_CONF_REG = 0x40004060;

    // Wait for the PLL to stabilize (lock).
    while (((*RTC_CNTL_CLK_CONF_REG >> 31) & 1) == 0);

    // Set the CPU clock divider to 2. PLL (480MHz) / 2 = 240MHz CPU clock.
    *DPORT_CPU_PER_CONF_REG &= ~(0b11 << 0); // Clear current divider
    *DPORT_CPU_PER_CONF_REG |= (1 << 0);     // Set divider to 2
}

// --- REPLACED --- Replaced simple loop with robust assembly version
void simple_delay(uint32_t ticks) {
    for (uint32_t i = 0; i < ticks; i++) {
        // This inline assembly is immune to compiler optimization
        __asm__ __volatile__("nop");
    }
}

void app_main() {
    // --- ADDED and RE-ORDERED --- Correct initialization sequence

    // 1. Set CPU clock to max speed first.
    configure_clock_to_240mhz();

    // 2. Disable the watchdog timers.
    // Disable the Timer Group 0 Watchdog
    *TIMG0_WDTWPROTECT_REG = WDT_WKEY_VALUE; // Unlock
    *TIMG0_WDTCONFIG0_REG = 0;               // Disable
    *TIMG0_WDTWPROTECT_REG = 0;               // Lock

    // Disable the RTC Watchdog
    *RTC_CNTL_WDTWPROTECT_REG = WDT_WKEY_VALUE; // Unlock
    *RTC_CNTL_WDTCONFIG0_REG = 0;               // Disable
    *RTC_CNTL_WDTWPROTECT_REG = 0;               // Lock

    // 3. --- ADDED --- Enable the clock for the GPIO peripheral.
    *DPORT_PERIP_CLK_EN_REG |= DPORT_GPIO_CLK_EN;

    // 4. Now, safely configure the GPIO pin.
    *GPIO_ENABLE_W1TS_REG = (1 << BLINK_GPIO);    // Set the GPIO pin to be an output

    while (1) {
        *GPIO_OUT_W1TS_REG = (1 << BLINK_GPIO);    //turn the LED on
        // --- MODIFIED --- Increased delay value to compensate for 240MHz speed
        simple_delay(150000000); // ~0.5 second delay at 240MHz

        *GPIO_OUT_W1TC_REG = (1 << BLINK_GPIO);    //turn the LED off
        simple_delay(150000000); // ~0.5 second delay at 240MHz
    }
}