// I2C address of Colorduino
#define TWI_ADDR 8

void initLEDMatrixClient() {
  Serial.println("Init LED Matrix module...");
  Wire.begin();
}

void sendCommandToColorduino(String msg) {
  if (msg == LEDMATRIX_COMMAND_GREEN || 
      msg == LEDMATRIX_COMMAND_YELLOW || 
      msg == LEDMATRIX_COMMAND_RED) {
    Serial.print("Send message to Colorduino:");
    Serial.println(msg);
  
    char msgChars[4];
    msg.toCharArray(msgChars, 4);
    
    Wire.beginTransmission(TWI_ADDR);
    Wire.write(msgChars);
    Wire.endTransmission();
  } else {
    Serial.print("ERROR: Wrong message, can't send to Colorduino: ");
    Serial.println(msg);
  }
}
