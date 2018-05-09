#include <string.h>

#define UART Serial1

/*
   Using static memory allocation because I don't have enough
   experience with the String class to guarantee it won't leak
   memory in a long-running embedded process.
*/

// Function prototypes.
void turn_LED_on(void);
void turn_LED_off(void);
void check_for_overflow_of_string(void);
void update_state_machine(void);

// This data structure holds one line of console output to examine.
const int max_string_length = 1000;
char string[max_string_length];
int i = 0;

// This is the state machine.
enum states {
  initial_state,
  first_target_seen,  // Looking for the message, "The operating system has halted.\r\n".
  second_target_seen, // Looking for the message, "Please press any key to reboot.\r\n".
  third_target_seen,  // Looking for a blank line "\r\n".
} state = initial_state;

const int LED = 13; // in-built LED; change this to a GPIO pin for use.

// This function is predefined by Arduino.
void setup() {
  pinMode(LED, OUTPUT);
  UART.begin(115200); // Set the speed to 115,200 bits per second (8,N,1).
  Serial.begin(9600); // This programme natters out out the USB port for debugging.
  string[i] = '\0';
  Serial.println("beginning in inital state"); // This is a debug message only.
  turn_LED_on();
}

// This function is predefined by Arduino.
void loop() {
  int incoming_byte;

  if (UART.available() > 0) {
    check_for_overflow_of_string();
    incoming_byte = UART.read();
    string[i++] = incoming_byte;
    string[i] = '\0'; // Properly terminate the string.
    Serial.write(incoming_byte); // For debugging.
    if ('\n' == incoming_byte) {
      update_state_machine();
      string[i = 0] = '\0'; // Empty the string; code golf, but it's idiomatic C.
    }
  }
}

void turn_LED_on(void) {
  digitalWrite(LED, HIGH);
}

void turn_LED_off(void) {
  digitalWrite(LED, LOW);
}

void check_for_overflow_of_string(void) {
  if (i > max_string_length - 1) {
    Serial.println("String overflow; truncating."); // For debugging.
    i = 0;
    string[i] = '\0'; // Terminate the string properly.
  }
}

void update_state_machine(void) {
  switch (state) {
    case initial_state:
      if (!strcmp(string, "The operating system has halted.\r\n")) {
        state = first_target_seen;
        Serial.println("transitioning from inital_state to first_target_seen"); // For debugging.
      }
      break;
    case first_target_seen:
      if (!strcmp(string, "Please press any key to reboot.\r\n")) {
        state = second_target_seen;
        Serial.println("transitioning from first_target_seen to second_target_seen"); // For debugging.
      }
      else {
        state = initial_state;
        Serial.println("transitioning from first_target_seen back to initial_state."); // For debugging.
      }
      break;
    case second_target_seen:
      if (!strcmp(string, "\r\n")) {
        state = third_target_seen;
        turn_LED_off();
        Serial.println("transitioning from second_target_seen to third_target_seen and turning LED off."); // For debugging.
      }
      else {
        state = initial_state;
        Serial.println("transitioning from second_target_seen back to initial_state."); // For debugging.
      }
      break;
    case third_target_seen:
      state = initial_state;
      turn_LED_on();
      Serial.println("transitioning from third_target_seen back to initial_state and turning the LED back on."); // For debugging.
      break;
    default:
      Serial.println("impossible state! Transitioning back to initial_state."); // For debugging.
      state = initial_state;
      break;
  }
}

