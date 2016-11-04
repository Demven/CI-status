void translateResponseToArray(String* arrayRef, String res, int quantity) {
  int resLen = res.length();
  if (resLen == CIPROJECT_RESPONSE_LEN) {
    String tempRes;
    for (int i = 0; i < quantity; i++){
      tempRes = res.substring(i * CIPROJECT_PROJECT_LEN, (i + 1) * CIPROJECT_PROJECT_LEN);
      if (tempRes == LEDMATRIX_COMMAND_GREEN || 
          tempRes == LEDMATRIX_COMMAND_YELLOW || 
          tempRes == LEDMATRIX_COMMAND_RED) {
        arrayRef[i] = tempRes;
      }
    }
  }
}
