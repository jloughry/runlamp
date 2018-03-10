#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <err.h>
#include <libgpio.h>

#define RUN 5
#define TRAFFIC 13
#define BLOCKED 26
#define FAULT 26
#define PHOTODIODE_A 4
#define LASER_A 4
#define PHOTODIODE_B 22
#define LASER_B 22

int main(void) {
	gpio_handle_t handle;

	handle = gpio_open(0);
	if (GPIO_INVALID_HANDLE == handle) {
		err(1, "gpio_open() failed");
	}
	gpio_pin_output(handle, TRAFFIC);
	gpio_pin_low(handle, TRAFFIC);
	gpio_close(handle);

	return(EXIT_SUCCESS);
}

