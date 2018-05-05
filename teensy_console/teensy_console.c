#include <string.h>

/* This programme doesn't control anything except the LED; it
   simply watches the serial console and blinks the LED when it
   sees a particular pattern of text. */

int main(void)
{
    uint8_t c;
    const int max = 1000;
    char string[max];
    char first_string[] = "The operating system has halted";
    char second_string[] = "Please press any key to reboot";

    /* The state machine begins in the on_steady state, turns on
       the LED, and begins reading characters from the UART. When
       it sees a newline, it compares the string to a constant value
       and if it finds a match, transitions to the fast_blinking
       state. It keeps on looking at the UART, and if it sees any
       other activity there, it transitions to the slow_flash state;
       from the slow_flash state, it may transition back to the
       fast_blinking state. */

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

