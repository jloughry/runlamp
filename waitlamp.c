#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <libgpio.h>
#include "gpio_pin_numbers.h"

#define PERIOD 500000 /* microseconds */

int main(void) {
	gpio_handle_t handle;

	handle = gpio_open(0);
	if (GPIO_INVALID_HANDLE == handle) {
		err(1, "gpio_open() failed");
	}
	gpio_pin_output(handle, WAIT);
	for (;;) {
		gpio_pin_high(handle, WAIT);
		usleep(PERIOD);
		gpio_pin_low(handle, WAIT);
		usleep(PERIOD);
	}
	gpio_close(handle);

	return(EXIT_SUCCESS);
}

