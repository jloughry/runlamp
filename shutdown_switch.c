// If you want to run this programme from /etc/rc.d/ then it can't
// be named anything that has an underscore in the name or it won't
// work; instead, the programme is named runlamp.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <libgpio.h>
#include "gpio_pin_numbers.h"

#define SWITCH_OPEN 1
#define SWITCH_CLOSED 0

#define FLASH_RATE 500000 // microseconds; half a second

/* Global because this is a simple programme; there's no need to make it
   more complicated by passing handle around as an explicit parameter. */

gpio_handle_t handle;

void start_flashing_the_WAIT_light(void);

int main(void) {
    handle = gpio_open(0);
    if (GPIO_INVALID_HANDLE == handle) {
        err(1, "gpio_open() failed");
    }

    // Switches without a pull-up resistor are flaky.
    gpio_pin_input(handle, SHUTDOWN_SW);
    gpio_pin_pullup(handle, SHUTDOWN_SW);

    // This one is the LED.
    gpio_pin_output(handle, WAIT);
    gpio_pin_low(handle, WAIT); // Turn the WAIT lamp off to begin with.

    gpio_value_t switch_state = 0;

    for (;;) {
        const int tenth_of_a_second = 100000; // microseconds

        switch_state = gpio_pin_get(handle, SHUTDOWN_SW);

        switch (switch_state) {
            case SWITCH_OPEN:
                break;
            case SWITCH_CLOSED:
                system ("/sbin/shutdown -h now"); // do this first, because
                start_flashing_the_WAIT_light(); // never returns
                break;
            default:
                printf ("Unknown switch state: %d\n", switch_state);
                break;
        }
        usleep(tenth_of_a_second); // Poll the switch this often.
    }
    // We should never actually get here.
    gpio_close(handle);

    return(EXIT_SUCCESS);
}

void start_flashing_the_WAIT_light(void) {
    const int flash_rate = 333333; // microseconds

    for (;;) {
        gpio_pin_high(handle, WAIT);
        usleep(flash_rate);
        gpio_pin_low(handle, WAIT);
        usleep(flash_rate);
    }
    // At least for now, this function never returns.
}

