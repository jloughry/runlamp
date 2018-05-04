int main(void)
{
    uint8_t c;
    const int max = 1000;
    char string[max];

    CPU_PRESCALE(0);  // run at 16 MHz

    // Turn the LED on.

    uart_init(9600);
    for (;;) {
        if (uart_available()) {
            int i = 0;

            c = uart_getchar(); // This blocks until c arrives.
            string[i++] = c;
            string[i++] = '\0';
            if ('\n' == c) {
                if (!strncmp(string, "System Halted\n")) {
                    // Turn the LED off.
                    break;
                }
                i = 0;
            }
        }
    }
    return 0;
}
