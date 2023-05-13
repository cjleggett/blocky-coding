#include <HardwareSerial.h>
#include "WiFi.h"
#include <esp_now.h>
#include "Buzzer.h"
#include "notes.h"
#define buzzerPin 9
const int LEN = 30;

Buzzer buzz = Buzzer(buzzerPin);

// Indicates whether current string is being sent to robot
bool uploading = false;

// To keep track of former messages to decide if message is stable
int prev_len = 0;
int reps = 0;

// For sending via ESP-now
char toSend[LEN];
String success;

// Define Serial device mapped to the two internal UARTs
HardwareSerial MySerial0(0);

// Current and best overall messages
String incomingString;
String bestString;

// Robot's MAC address
uint8_t broadcastAddress[] =  {0xC8, 0xF0, 0x9E, 0x47, 0xF1, 0x7C};


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  return;
}


void setup() {
  // Serial setup
  Serial.begin(9600);
  while (!Serial);
  MySerial0.begin(9600, SERIAL_8N1, -1, -1);
  Serial.println("MySerial0 Set Up");

  // Prepare buzzer
  pinMode(buzzerPin, OUTPUT);

  // Set up WiFi
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());

  // Communications
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
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

// Calculate length of message
int getSequenceLength(String s) {
  for (int i = 0; i < s.length(); i++) {
    if (s.charAt(i) == '!') {
      return i;
    }
  }
  return -1;
}

// Ensure only capital letters and end chars are detected
bool checkValid(String s) {
  for (int i = 0; i < s.length(); i++) {
    char cur = s.charAt(i);
    if (cur != '!' && cur != '\0' && (cur < 65 || cur > 90)) {
      return false;
    }
  }
  return true;
}


void loop() {
  if (!uploading && MySerial0.available())
  {
    // Indicate serial data received
    buzz.playNote(C4, 10);

    incomingString = MySerial0.readStringUntil('\0');
    Serial.print("received: ");
    Serial.println(incomingString);
    bool valid = checkValid(incomingString);
    if (!valid) {
      // If current string not valid and last one was, use the last one
      if (prev_len > 0) {
        uploading = true;
      }
    } else 
    {
      int curLen = getSequenceLength(incomingString);
      if (curLen < prev_len || (curLen == prev_len && reps > 5)) {
        // Message is shorter or repeated enough, so upload!
        uploading = true;
      } else {
        // New, longer message is received, so keep reading...
        reps = 0;
        bestString = incomingString;
        prev_len = curLen;
      }
    }
  } else if (uploading) {
    // Convert to char array for sending
    for (int i = 0; i < LEN; i++) {
      toSend[i] = bestString.charAt(i);
    }
    buzz.playUploaded();
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &toSend, sizeof(toSend));

    // Send message and reset
    if (result == ESP_OK) {
      Serial.print(toSend);
      Serial.println(" sent with success!");
      uploading = false;
      prev_len = 0;
      reps = 0;
    } else {
      Serial.println("Error sending the data");
    }
  }
  delay(200);
  buzz.off();
  delay(800);
}