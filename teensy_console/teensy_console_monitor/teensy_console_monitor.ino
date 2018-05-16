#include <string.h>

#define UART Serial1

/*
   Using static memory allocation because I don't have enough
   experience with the String class to guarantee it won't leak
   memory in a long-running embedded process.
*/

// Function prototypes.
void turn_LEDs_on(void);
void turn_LEDs_off(void);
void check_for_overflow_of_string(void);
void update_state_machine(void);

// This data structure holds one line of console output to examine.
const int max_string_length = 1000;
char string[max_string_length];
int i = 0;

// This is the state machine.
enum states {
  initial_state,
  first_target_seen,  // Looking for "The operating system has halted.\r\n".
  second_target_seen, // Looking for "Please press any key to reboot.\r\n".
  third_target_seen,  // Looking for a blank line, i.e., "\r\n".
} state = initial_state;

const int LED_1 = 2;  // external LED connected to pin 2
const int LED_2 = 13; // in-built LED

// This function is predefined by Arduino.
void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  UART.begin(115200); // Set the speed to 115,200 bits per second (8,N,1).
  string[i] = '\0';
  turn_LEDs_on();
}

// This function is predefined by Arduino.
void loop() {
  int incoming_byte;

  if (UART.available() > 0) {
    check_for_overflow_of_string();
    incoming_byte = UART.read();
    string[i++] = incoming_byte;
    string[i] = '\0'; // Properly terminate the string.
    if ('\n' == incoming_byte) {
      update_state_machine();
      string[i = 0] = '\0'; // Code golf, but it's idiomatic C.
    }
  }
}

void turn_LEDs_on(void) {
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
}

void turn_LEDs_off(void) {
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
}

void check_for_overflow_of_string(void) {
  if (i > max_string_length - 1) {
    i = 0;
    string[i] = '\0'; // Terminate the string properly.
  }
}

void update_state_machine(void) {
  switch (state) {
    case initial_state:
      if (!strcmp(string, "The operating system has halted.\r\n")) {
        state = first_target_seen;
      }
      break;
    case first_target_seen:
      if (!strcmp(string, "Please press any key to reboot.\r\n")) {
        state = second_target_seen;
      }
      else {
        state = initial_state;
      }
      break;
    case second_target_seen:
      if (!strcmp(string, "\r\n")) {
        state = third_target_seen;
        turn_LEDs_off();
      }
      else {
        state = initial_state;
      }
      break;
    case third_target_seen:
      state = initial_state;
      turn_LEDs_on();
      break;
    default:
      state = initial_state;
      break;
  }
}

