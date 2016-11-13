/*
 * These are the interrupt and control pins
 * Use hardware SPI for the remaining pins
 * On an UNO, SCK = 13, MISO = 12, and MOSI = 11
 * IRQ - MUST be an interrupt pin!
 * VBAT, CS - can be any two pins
 */
#define ADAFRUIT_CC3000_IRQ 3
#define ADAFRUIT_CC3000_VBAT 5
#define ADAFRUIT_CC3000_CS 10
/*
 * Amount of time to wait (in milliseconds) with no data received before closing the connection.
 * If you know the server you're accessing is quick to respond, you can reduce this value.
 */
#define IDLE_TIMEOUT_MS 3000

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);
uint32_t ip;

void initWebClient() {
  #if DEBUG_MODE
    Serial.println("Init WiFi module...");
  #endif

  if (!cc3000.begin()) {
    #if DEBUG_MODE
      Serial.println("Couldn't begin()! Check your wiring?");
    #endif 
    while(1);
  }

  // SSID scan
  #if WIFI_DEBUG_MODE
    listSSIDResults();
  #endif
}

void connectToInternet() {
  #if DEBUG_MODE
    Serial.print("\nAttempting to connect to SSID: "); Serial.println(WLAN_SSID);
  #endif
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    #if DEBUG_MODE
      Serial.print("ERROR: Failed to connect to "); Serial.println(WLAN_SSID);
    #endif
    playError();
    while(1);
  }

  #if DEBUG_MODE
    Serial.println("Connected to WLAN network!");
  #endif

  playSuccess();
  
  // Wait for DHCP to complete
  // Serial.println("Request DHCP...");
  while (!cc3000.checkDHCP()) {
    delay(100);
  }  

  // Display the IP address DNS, Gateway, etc.
  #if WIFI_DEBUG_MODE
    while (! displayConnectionDetails()) {
      delay(1000);
    }
  #endif
}

void obtainIPAdress() {
  ip = 0;
  
  // Try looking up the website's IP address
  #if WIFI_DEBUG_MODE
    Serial.print(CIPROJECT_HOST); Serial.print(" -> ");
    cc3000.printIPdotsRev(ip);
  #endif

  while (ip == 0) {
    if (!cc3000.getHostByName(CIPROJECT_HOST, &ip)) {
      #if DEBUG_MODE
        Serial.print("Couldn't resolve host!"); Serial.println(CIPROJECT_HOST);
      #endif
    }
    delay(500);
  }

  #if WIFI_DEBUG_MODE
    cc3000.printIPdotsRev(ip);
  #endif
}

Adafruit_CC3000_Client createConnection() {
  // Note: HTTP/1.1 protocol is used to keep the server from closing the connection before all data is read
  Adafruit_CC3000_Client www = cc3000.connectTCP(ip, CIPROJECT_PORT);
  if (www.connected()) {
    www.fastrprint("GET ");
    www.fastrprint(CIPROJECT_PAGE);
    www.fastrprint(" HTTP/1.1\r\n");
    www.fastrprint("Host: "); www.fastrprint(CIPROJECT_HOST); www.fastrprint("\r\n");
    www.fastrprint("\r\n");
    www.println();
    return www;
  } else {
    #if DEBUG_MODE
      Serial.println("ERROR: Failed to create connection");
    #endif
    playError();   
    return;
  }
}

String getServerResult(Adafruit_CC3000_Client www) {
  #if DEBUG_MODE
    Serial.println("\nSend new request to server...");
  #endif
  
  String serverMsg = "";
  char serverResultArray[CIPROJECT_RESPONSE_LEN];
  
  // Read data until either the connection is closed, or the idle timeout is reached
  unsigned long lastRead = millis();
  long serverMsgLen;
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      serverMsg.concat((char)www.read());
      serverMsgLen = serverMsg.length();

      if (serverMsgLen > CIPROJECT_RESPONSE_LEN) {
        serverMsg = serverMsg.substring(1);
      }
      
      lastRead = millis();
    }
  }
  www.close();

  #if DEBUG_MODE
    Serial.print("\nGot result from server: "); Serial.println(serverMsg);
  #endif

  return serverMsg;
}

void stopConnection(Adafruit_CC3000_Client connection) {
  // Serial.println("Clear conection...");
  connection.stop();
  connection.flush();
}

void disconnectFromNetwork() {
  // Serial.println("\nDisconnecting from WLAN...");
  cc3000.disconnect();
}

#if WIFI_DEBUG_MODE
  void listSSIDResults(void) {
    uint32_t index;
    uint8_t valid, rssi, sec;
    char ssidname[33];

    Serial.println("Scan WiFi networks around...");

    if (!cc3000.startSSIDscan(&index)) {
      Serial.println("ERROR: SSID scan failed!");
      return;
    }

    Serial.print("Networks found: "); Serial.println(index);
    Serial.println("================================================");

    while (index) {
      index--;

      valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

      Serial.print("SSID Name    : "); Serial.println(ssidname);
      Serial.print("RSSI         : "); Serial.println(rssi);
      Serial.print("Security Mode: "); Serial.println(sec);
    }

    Serial.println("================================================");

    cc3000.stopSSIDscan();
  }
#endif

#if WIFI_DEBUG_MODE
  bool displayConnectionDetails(void) {
    uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

    Serial.println("Retrieve connection details...");

    if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) {
       Serial.println("ERROR: Unable to retrieve the IP Address!");
      return false;
    } else {
       Serial.print("\nIP Addr: "); cc3000.printIPdotsRev(ipAddress);
       Serial.print("\nNetmask: "); cc3000.printIPdotsRev(netmask);
       Serial.print("\nGateway: "); cc3000.printIPdotsRev(gateway);
       Serial.print("\nDHCPsrv: "); cc3000.printIPdotsRev(dhcpserv);
       Serial.print("\nDNSserv: "); cc3000.printIPdotsRev(dnsserv);
       Serial.println();
      return true;
    }
  }
#endif
