// Update this to be the letter of the current command
char myName = 'K';

// Indicates whether a lower block has been found
bool found = false;

// Set up string variables
const int LEN = 30;
char str[LEN];
char TERM1 = '\0';
char TERM2 = '!';
bool newInfo = true;
int seq_len = 0;

void setup() {
  Serial.begin(9600);

  // Set up initial messageing
  str[0] = myName;
  for (int i = 1; i < LEN - 1; i++) {
    str[i] = TERM2;
  }
  str[LEN - 1] = TERM1;
}

void loop() {
  if (Serial.available())
  {
    // Read incoming
    Serial.readBytes(str, LEN);

    // Add my name to str
    found = false;
    for (int i = 0; i < LEN - 1; i++) {
      if (str[i] == TERM2 && !found) {
        str[i] = myName;
        found = true;
        if (i > seq_len) {
          seq_len = i;
          newInfo = true;
        } else {
          newInfo = false;
        }
      } else if (found) {
        str[i] = TERM2;
      }
    }

    // Add terminal character
    str[LEN - 1] = TERM1;
  }
  delay(1000);
  if (newInfo) {
    // Send new message and flush output
    Serial.write(str, LEN);
    Serial.flush();
  }  
}