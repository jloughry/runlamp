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
} state = initial;

const int LED = 13;

void setup() {
  pinMode(LED, OUTPUT);
  UART.begin(115200);
  Serial.begin(9600); // echo to USB for testing
  string[i] = '\0';
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
    blink_LED();
    Serial.write(incoming_byte); // to watch what's happening
    string[i++] = incoming_byte;
    string[i] = '\0';
    if ('\n' == incoming_byte) {
      switch (state) {
        case initial:
          if (!strcmp(string, "abc\n")) {
            state = first_target_seen;
            Serial.println("transitioning from inital to first_target_seen");
          }
          break;
        case first_target_seen:
          if (!strcmp(string, "def\n")) {
            state = second_target_seen;
            Serial.println("transitioning from first_target_seen to second_target_seen");
          }
          break;
        case second_target_seen:
          state = initial;
          Serial.println("transitioning from second_target_seen back to initial");
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

void blink_LED() {
  digitalWrite(LED, HIGH);
  delay(20);
  digitalWrite(LED, LOW);
}

