// If you want to run this programme from /etc/rc.d/ then it can't
// be named anything that has an underscore in the name or it won't
// work; instead, the programme is named runlamp.

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <libgpio.h>
#include "gpio_pin_numbers.h"

#define PERIOD 1000000 /* microseconds */
#define WHICH_LAMP RUN

int main(void) {
	gpio_handle_t handle;

	handle = gpio_open(0);
	if (GPIO_INVALID_HANDLE == handle) {
		err(1, "gpio_open() failed");
	}
	gpio_pin_output(handle, WHICH_LAMP);
	for (;;) {
		gpio_pin_high(handle, WHICH_LAMP);
		usleep(PERIOD);
		gpio_pin_low(handle, WHICH_LAMP);
		usleep(PERIOD);
	}
	gpio_close(handle);

	return(EXIT_SUCCESS);
}

