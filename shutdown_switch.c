// If you want to run this programme directly from /etc/rc.d/
// then it can't be named anything that has an underscore in
// the name or it won't work (because reasons).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <libgpio.h>
#include "gpio_pin_numbers.h"

#define SWITCH_OPEN 1
#define SWITCH_CLOSED 0

int main(void) {
    gpio_handle_t handle;

    handle = gpio_open(0);
    if (GPIO_INVALID_HANDLE == handle) {
        err(1, "gpio_open() failed");
    }

    // External switches without a pull-up resistor are flaky.
    gpio_pin_input(handle, SHUTDOWN_SW);
    gpio_pin_pullup(handle, SHUTDOWN_SW);

    // This pin is the LED.
    gpio_pin_output(handle, WAIT);
    gpio_pin_low(handle, WAIT); // Begin with he WAIT lamp off.

    gpio_value_t switch_state = 0;

    for (;;) {
        const int tenth_of_a_second = 100000; // microseconds

        switch_state = gpio_pin_get(handle, SHUTDOWN_SW);

        switch (switch_state) {
            case SWITCH_OPEN:
                break;
            case SWITCH_CLOSED:
                gpio_pin_high(handle, WAIT); // Turn on WAIT lamp.
                system ("/sbin/shutdown -h now");
                break;
            default:
                // FIXME: This really ought to go to syslog(3).
                printf ("Unknown switch state: %d\n", switch_state);
                break;
        }
        usleep(tenth_of_a_second); // Poll the switch this often.
    }
    // We should never actually get here.
    gpio_close(handle);

    return(EXIT_SUCCESS);
}

