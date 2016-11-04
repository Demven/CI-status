#include <Wire.h>
#include <Adafruit_CC3000.h>

#define PROJECTS {"gaia", "publishing-toolkit-ui", "publishing-toolkit-api", "publishing-toolkit-dal"}
#define PROJECTS_QUANTITY 4

/*
 * Define the http address of the server that returns data for Arduino
 */
#define CIPROJECT_HOST "arduino-demven.rhcloud.com"
#define CIPROJECT_PAGE "/circleci/test/grn/grn/grn/yel"
#define CIPROJECT_PORT 80
#define CIPROJECT_PROJECT_LEN 3
#define CIPROJECT_RESPONSE_LEN 12

/*
 * Define the name and password of your WiFi router
 * Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
 */
#define WLAN_SSID "WIFIFA89C4"
#define WLAN_PASS "64P5Z3RM64IDZRYD"
#define WLAN_SECURITY WLAN_SEC_WPA2

/*
 * Possible commands that could be sent to Colorduino
 */
#define LEDMATRIX_COMMAND_RED "red"
#define LEDMATRIX_COMMAND_GREEN "grn"
#define LEDMATRIX_COMMAND_YELLOW "yel"

/*
 * How often need to send requests (ms)
 */
#define TIMEOUT_BTW_REQUESTS_MS 10000

/*
 * Globals
 */
int PROJECT_PINS[] = {A0, A1, A2, A3};
String projectsResults[PROJECTS_QUANTITY];

void setup(void) {
  Serial.begin(115200);
  
  initLEDMatrixClient();

  initWebClient();
  connectToInternet();
  obtainIPAdress();

  for (int i = 0; i < PROJECTS_QUANTITY; i++) {
    pinMode(PROJECT_PINS[i], OUTPUT);
  }
}

void loop(void) {
  Adafruit_CC3000_Client connection = createConnection();
  
  String resultFromServer = getServerResult(createConnection());
  translateResponseToArray(projectsResults, resultFromServer, PROJECTS_QUANTITY);
  indicateProjects(projectsResults);
  
  String commandForColorduino = getCommandForColorduino(projectsResults);
  sendCommandToColorduino(commandForColorduino);
  
  stopConnection(connection);
  
  delay(TIMEOUT_BTW_REQUESTS_MS);
}

