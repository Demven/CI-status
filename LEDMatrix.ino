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

String getCommandForColorduino(String* projectsStatuses) {
  String command;

  if (_contains(LEDMATRIX_COMMAND_RED, projectsStatuses)) {
    command = LEDMATRIX_COMMAND_RED;
  } else if (_contains(LEDMATRIX_COMMAND_YELLOW, projectsStatuses)) {
    command = LEDMATRIX_COMMAND_YELLOW;
  } else {
    command = LEDMATRIX_COMMAND_GREEN;
  }

  return command;
}

boolean _contains(String value, String* projectsStatuses) {
  boolean contains = false;
  for (int i = 0; i < PROJECTS_QUANTITY; i++) {
    if (projectsStatuses[i] == value) {
      contains = true;
      break;
    }
  }
  return contains;
}
