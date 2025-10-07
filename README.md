# LED-Blinking-with-Bare-Metal-Programming-with-ESP32
A bare-metal project for Blinking LED using GPIO pins of ESP32 devKit by manipulating register values without using any onboard RTOS libraries

We are trying to blink an external LED by turning the GPIO level high and low. This is done by manipulating the register values corresponding to the GPIO. Go through the user manual for getting the GPIO addresses for your board.

In this project, we are working with true bare metal appoach and in doing so, we are disabling the Watchdog timers. This is done in order to make sure our program runs. We tried to complete this project without disabling the watchdog timer. But before we could make any changes to the configuration of the watchdog timer, it was resetting the microcontroller and no line of the program was getting executed. So we disabled the watchdog timer.

But this issue can be resolved if we use ESP-IDF to build and flash the program on the microcontroller. By using ESP-IDF, the watchdog timer is taken care of by the environment.

How to run the program:

1. Download the whole package.
2. Open the project folder.
3. Check the serial port of your system and add it to the Makefile. For example, here its COM5
4. Open command prompt in the folder path.
5. Run 'make build'.
6. Run 'make flash'. LED will start blinking