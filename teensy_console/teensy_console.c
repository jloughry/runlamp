#include <string.h>

int main(void)
{
    uint8_t c;
    const int max = 1000;
    char string[max];
    char first_string[] = "The operating system has halted";
    char second_string[] = "Please press any key to reboot";

    enum States {
        on_steady,
        fast_blinking,
        slow_flash,
    } state = on_steady;

    CPU_PRESCALE(0);  // run at ?? MHz (48?)

    // Turn the LED on.

    uart_init(9600);
    for (;;) {
        if (uart_available()) {
            int i = 0;

            c = uart_getchar(); // This blocks until c arrives.
            string[i++] = c;
            string[i++] = '\0';
            if ('\n' == c) {
                if (!strncmp(string, "The operating system has halted.\n")) {
                    // Turn the LED off.
                    break;
                }
                i = 0;
            }
        }
    }
    return 0;
}

