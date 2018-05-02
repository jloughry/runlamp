// If you want to run this programme from /etc/rc.d/ then it can't
// be named anything that has an underscore in the name or it won't
// work; instead, the programme is named flickertraffic.

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

#define ON_TIME 50000 // microseconds; twentieth of a second
#define MEAN_TIME_BETWEEN_FLASHES 2000000 // microseconds; two seconds

int main(void) {
	gpio_handle_t handle;

	handle = gpio_open(0);
	if (GPIO_INVALID_HANDLE == handle) {
		err(1, "gpio_open() failed");
	}
	gpio_pin_output(handle, TRAFFIC);
	for (;;) {
		gpio_pin_high(handle, TRAFFIC);
		usleep(ON_TIME);
		gpio_pin_low(handle, TRAFFIC);
		usleep(random() % MEAN_TIME_BETWEEN_FLASHES);
	}
	gpio_close(handle);

	return(EXIT_SUCCESS);
}

