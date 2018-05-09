#include <string.h>

#define UART Serial1

/*
 * Using static memory allocation because I don't have enough
 * experience with the String class to guarantee it won't leak 
 * memory in a long-running embedded process.
 */

const int max_string_length = 1000;
char string[max_string_length];
int i = 0;

enum states {
  initial,
  first_target_seen,
  second_target_seen,
  third_target_seen,
  fourth_target_seen,
} state = initial;

const int LED = 13;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH); // Turn the LED on.
  UART.begin(115200);
  Serial.begin(9600); // echo to USB for testing
  string[i] = '\0';
  Serial.println("beginning in inital state");
}

void loop() {
  int incoming_byte;

  if (UART.available() > 0) {
    if (i > max_string_length - 1) {
      Serial.println("String overflow; truncating.");
      i = 0;
      string[i] = '\0';
    }
    incoming_byte = UART.read();
    string[i++] = incoming_byte;
    Serial.write(incoming_byte);
    string[i] = '\0';
    if ('\n' == incoming_byte) {
      switch (state) {
        case initial:
          if (!strcmp(string, "The operating system has halted.\n")) {
            state = first_target_seen;
            Serial.println("transitioning from inital to first_target_seen");
          }
          break;
        case first_target_seen:
          if (!strcmp(string, "Please press any key to reboot.\n")) {
            state = second_target_seen;
            Serial.println("transitioning from first_target_seen to second_target_seen");
          }
          else {
            state = initial;
            Serial.println("transitioning from first_target_seen back to initial.");
          }
          break;
        case second_target_seen:
          if (!strcmp(string, "\n")) {
            state = third_target_seen;
            Serial.println("transitioning from second_target_seen to third_target_seen");
          }
          else {
            state = initial;
            Serial.println("transitioning from second_target_seen back to initial.");
          }
          break;
        case third_target_seen:
          if (!strcmp(string, "\n")) {
            state = fourth_target_seen;
            digitalWrite(LED, LOW); // Turn the LED off.
            Serial.println("transitioning from third_target_seen to fourth_target_seen and turning LED off.");
          }
          else {
            state = initial;
            Serial.println("transitioning from third_target_seen back to initial.");
          }
          break;
        case fourth_target_seen:
          state = initial;
          digitalWrite(LED, HIGH);
          Serial.println("transitioning from fourth_target_seen back to initial and turning the LED back on.");
          break;
        default:
          Serial.println("impossible state! Transitioning back to initial.");
          state = initial;
          break;
      }
      string[i=0] = '\0';
    }
  }
}

