// I2C address of Colorduino
#define TWI_ADDR 8

void initLEDMatrixClient() {
  #if DEBUG_MODE
    Serial.println("Init LED Matrix module...");
  #endif 
  Wire.begin();
}

void sendCommandToColorduino(String msg) {
  if (msg == LEDMATRIX_COMMAND_GREEN || 
      msg == LEDMATRIX_COMMAND_YELLOW || 
      msg == LEDMATRIX_COMMAND_RED) {

    #if DEBUG_MODE
      Serial.print("Send message to Colorduino:");
      Serial.println(msg);
    #endif
    
    char msgChars[4];
    msg.toCharArray(msgChars, 4);
    
    Wire.beginTransmission(TWI_ADDR);
    Wire.write(msgChars);
    Wire.endTransmission();
  } else {
    #if DEBUG_MODE
      Serial.print("ERROR: Wrong message, can't send to Colorduino: ");
      Serial.println(msg);
    #endif 
  }
}
