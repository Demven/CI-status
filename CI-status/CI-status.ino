/**********************************************************************
  CI-Status
  by Dmitry Salnikov (Demven)

  This project shows the current CI (Continuous Integration) status
  for 4 projects (we use CircleCI, but actually it doesn't matter, because
  Arduino gets CI results from my small nodejs server that returns data in one small
  string, and then uses that string to display status color on RGB matrix
  RED - is some of the projects is broken during test running and deploy,
  YELLOW - if some of the projects is being processed by CI,
  GREEN - if all of the projects are fine and deployed successfully).

  Full description, circuits and video you can find here:
  https://github.com/demven/CI-status

  Email me: dmitry_salnikov@protonmail.com

  This software is created specially for Arduino board and requires
  to use a WiFi shield (any board of cc3000 series)
  and Colorduino board connected to 8x8 RGB LED matrix.

  For generating sounds small 5v buzzer is used
  and 4 small LEDs are used to show which projects are broken.
**********************************************************************/

#include <Wire.h>
#include <Adafruit_CC3000.h>

#define DEBUG_MODE false
#define WIFI_DEBUG_MODE false

#define PROJECTS {"gaia", "publishing-toolkit-ui", "publishing-toolkit-api", "publishing-toolkit-dal"}
#define PROJECTS_QUANTITY 4

/*
 * Define the http address of the server that returns data for Arduino
 */
#define CIPROJECT_HOST "arduino-demven.rhcloud.com"
// #define CIPROJECT_PAGE "/circleci/test/grn/red/grn/grn" // for testing
#define CIPROJECT_PAGE "/circleci/all"
#define CIPROJECT_PORT 80
#define CIPROJECT_PROJECT_LEN 3
#define CIPROJECT_RESPONSE_LEN 12

/*
 * Define the name and password of your WiFi router
 * Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
 */
#define WLAN_SSID "WIFIFA89C4"
#define WLAN_PASS "64P5Z3RM64IDZRYD"
// #define WLAN_SSID "Dmitry’s iPhone"
// #define WLAN_PASS "041291kjh88hbv"
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
  initLogger();
  initBuzzer();
  
  playGreeting();
  
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
  
  String theWorstCommand = getTheWorstCommand(projectsResults);
  
  sendCommandToColorduino(theWorstCommand);
  sendCommandToBuzzer(theWorstCommand);
  
  stopConnection(connection);
  
  delay(TIMEOUT_BTW_REQUESTS_MS);
}

