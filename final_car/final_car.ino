#include "WiFi.h"
#include <esp_now.h>
#include "ColorSensor.h"
#include "Buzzer.h"
#include <FastLED.h>
#include "strip.h"
#include "constants.h"
#include "Drive.h"
#define beep_note 1000
#define buzzer_pin 26

// Max message Length
const int LEN = 30;

// How long to run for at maximum
const int timeout_minutes = 10;

// Initialize helper objects
Drive driver = Drive();
ColorSensor sensor = ColorSensor();
Buzzer buzz = Buzzer(buzzer_pin, beep_note);
int rgbc[4];
CRGB leds[NUM_LEDS];
Strip stp = Strip(leds);

// Is the above if statement satisfied?
bool ifSatisfied;

char commands[LEN] = "";
int num_commands = 0;

/*
Start of code having to do with ESP-Now
---
---
---
*/
String success;
char dataReceived[LEN];
// char incomingString[LEN];
uint8_t broadcastAddress[] = { 0x34, 0x85, 0x18, 0x03, 0x19, 0x80 };

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&dataReceived, incomingData, sizeof(dataReceived));
}

void setupESPNow() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

/*
End of code having to do with ESP-Now
---
---
---
*/

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());

  sensor.setup();
  setupESPNow();
  buzz.setup();
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  driver.setup();
  driver.stop();
  randomSeed(analogRead(2));

  // Beep and turn on light
  stp.change_color();
  buzz.beep();
}

// Check if seeing the color passed in as argument. Either "Yellow", "Blue", "Green", or "Any"
bool checkSatisfied(String currentColor) {
  String colorSensed = sensor.getColorName();
  return (currentColor == colorSensed) || (currentColor == "Any" && colorSensed != "Other");
}

// Handles mapping of single characters to actions.
// See documentation for explanation of this mapping
void handleCommand(char cmd) {
  Serial.println(cmd);
  if (cmd == 'D') {
    ifSatisfied = checkSatisfied("Any");
  } else if (cmd == 'H') {
    ifSatisfied = true;
  } else if (!ifSatisfied) {
    return;
  } else if (cmd == 'I') {
    buzz.beep();
  } else if (cmd == 'J') {
    stp.change_color();
  } else if (cmd == 'K') {
    stp.change_mode();
  } else if (cmd == 'A') {
    driver.forward();
  } else if (cmd == 'B') {
    driver.turnRandom();
  } else if (cmd == 'C') {
    driver.stop();
    delay(500);
  }
}

// Checks if data recieved is in valid form
bool checkValid() {
  for (int i = 0; i < LEN; i++) {
    char cur = dataReceived[i];
    if (cur != '!' && cur != '\0' && (cur < 65 || cur > 90)) {
      return false;
    }
  }
  return true;
}

// Creates new set of commands and updates length of commands
// Characters are sent in reverse order
int makeNewCommands() {
  // Locate first '!'
  int first = -1;
  for (int i = 0; i < LEN; i++) {
    if (first < 0 && dataReceived[i] == '!') {
      first = i;
    }
  }

  // Reverse Commands:
  for (int i = 0; i < first; i++) {
    commands[i] = dataReceived[first - i - 1];
    Serial.print(dataReceived[first - i - 1]);
  }
  return first;
}



void loop() {
  // Update command string
  if (checkValid()) {
    num_commands = makeNewCommands();
    if (num_commands > 0) {
      Serial.println(commands);
      Serial.println(num_commands);
    }
  }
  
  // Update helper objects
  sensor.getColors(rgbc);
  // buzz.update();
  stp.update();
  driver.update();

  // Process Code String if timeout not occurred
  ifSatisfied = true;
  if (millis() < timeout_minutes * 60 * 1000) {
    for (int i = 0; i < num_commands; i++) {
      char cmd = commands[i];
      handleCommand(cmd);
    }
  } else {
    driver.stop();
  }
}