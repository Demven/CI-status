#include <Colorduino.h>
#include <Wire.h>

// I2C address of the master board
#define TWI_PIN 8

/*
 * Possible commands that could be sent to Colorduino
 */
#define LEDMATRIX_COMMAND_RED "red"
#define LEDMATRIX_COMMAND_GREEN "grn"
#define LEDMATRIX_COMMAND_YELLOW "yel"

byte whiteBalVal[3] = { 37, 63, 63 };
String msg = "";

void setup() {
  // configure Colorduino lib
  Colorduino.Init();
  Colorduino.SetWhiteBal(whiteBalVal);

  // confugire Two Wire Interface
  Wire.begin(TWI_PIN);
  Wire.onReceive(getMsg);
}

void loop() {
  delay(100);
}

void getMsg(int howMany) {
  while (Wire.available()) {
    msg.concat((char)Wire.read());
  }

  if (msg == LEDMATRIX_COMMAND_RED) {
    showRed();
  } else if (msg == LEDMATRIX_COMMAND_YELLOW) {
    showYellow();
  } else if (msg == LEDMATRIX_COMMAND_GREEN) {
    showGreen();
  }

  msg = "";
}

void showRed() {
  Colorduino.ColorFill(255,0,0);
}

void showYellow() {
  Colorduino.ColorFill(255,255,0);
}

void showGreen() {
  Colorduino.ColorFill(0,255,0);
}
