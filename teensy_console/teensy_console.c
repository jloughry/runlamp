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

    /* The state machine begins in the LED_ON state, turns on
       the LED, and begins reading characters from the UART.
       When it sees a newline, it compares the string to a constant
       value and if it finds a match, transitions to the LED_OFF
       state. It keeps on looking at the UART, and if it sees any
       other activity there, it transitions back to the LED_ON
       state. Actually, there are two constant value strings, so
       it's a bit more complicated than that, but I'm going to
       implement it naïvely first and see if that works acceptably
       well. */

    enum states {
        LED_ON,
        LED_OFF,
    } state = LED_ON;

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

