#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <libgpio.h>
#include "gpio_pin_numbers.h"

int main(void) {
	gpio_handle_t handle;

	handle = gpio_open(0);
	if (GPIO_INVALID_HANDLE == handle) {
		err(1, "gpio_open() failed");
	}
	gpio_pin_output(handle, WAIT);
	gpio_pin_low(handle, WAIT);
	gpio_close(handle);

	return(EXIT_SUCCESS);
}

