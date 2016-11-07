/*
  note  frequency
  c     262 Hz
  d     294 Hz
  e     330 Hz
  f     349 Hz
  g     392 Hz
  a     440 Hz
  b     494 Hz
  C     523 Hz
*/

// PWM pin for piezo buzzer
#define BUZZER_PIN 9

String previousCommand = "";

void initBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void sendCommandToBuzzer(String msg) {
  if (msg == LEDMATRIX_COMMAND_RED && previousCommand != LEDMATRIX_COMMAND_RED) {
    playWarning();
  } else if (msg == LEDMATRIX_COMMAND_GREEN && previousCommand == "" || previousCommand == LEDMATRIX_COMMAND_RED) {
    playSuccess();
  }

  previousCommand = msg;
}

void playError() {
  tone(BUZZER_PIN, 392, 300);
  delay(200);
  tone(BUZZER_PIN, 262, 300);
}

void playWarning() {
  _makeWarningNoize();
  _makeWarningNoize();
  _makeWarningNoize();
}

void playSuccess() {
  tone(BUZZER_PIN, 262, 300);
  delay(200);
  tone(BUZZER_PIN, 392, 300);
}

void playGreeting() {
  tone(BUZZER_PIN, 494, 300);
  delay(200);
  tone(BUZZER_PIN, 392, 300);
  delay(200);
  tone(BUZZER_PIN, 294, 300);
  delay(200);
  tone(BUZZER_PIN, 392, 300);
}

void _makeWarningNoize() {
  tone(BUZZER_PIN, 523, 260);
  delay(320);
}
