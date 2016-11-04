#include <Colorduino.h>
#include <Wire.h>

#define TWI_PIN 8

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

  if (msg == "red") {
    showRed();
  } else if (msg == "yel") {
    showYellow();
  } else if (msg == "grn") {
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
