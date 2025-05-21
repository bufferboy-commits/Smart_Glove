#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>


#define WIFI_SSID "Galaxy A22 5G4934"
#define WIFI_PASSWORD "A123456M"
#define DATABASE_URL "https://iot-glove-77273-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_SECRET "YOUR_DATABASE_SECRET" 

unsigned long sendDataPrevMillis = 0;



// Pin configuration for flex sensors
const int flexPins[5] = {32, 33, 34, 35, 39};

// Calibration values for flex sensors
int flexMin[5] = {0}; // Minimum values when the hand is open
int flexMax[5] = {0}; // Maximum values when the hand is closed


String checkResult(String result) {
    if (result.equals("10110")) {
        return "A";
    }
    if (result.equals("10100")) {
        return "E";
    }
    if (result.equals("10011")) {
        return "H";
    }
    if (result.equals("10101")) {
        return "I";
    }
    if (result.equals("00111")) {
        return "J";
    }
    if (result.equals("00101")) {
        return "K";
    }
    if (result.equals("10010")) {
        return "L";
    }
    if (result.equals("00110")) {
        return "N";
    }
    if (result.equals("00011")) {
        return "O";
    }
    if (result.equals("00100")) {
        return "P";
    }
    if (result.equals("10000")) {
        return "R";
    }
    if (result.equals("10111")) {
        return "S";
    }
    if (result.equals("00000")) {
        return "T";
    }
    if (result.equals("00001")) {
        return "U";
    }
    return "Z"; // Default case
}

void setChar(String c){
    String currentGesture = c;
    String path = DATABASE_URL;
    path += "gestures/gesture1.json?auth=" + String(DATABASE_SECRET);

    HTTPClient http;
    http.begin(path);  // Initialize HTTP client
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"name\":\"" + currentGesture + "\"}";  // JSON payload

    int httpResponseCode = http.PUT(payload);  // Send HTTP PUT request
    if (httpResponseCode > 0) {
      Serial.println("Gesture sent: " + currentGesture);
      Serial.println("Response code: " + String(httpResponseCode));
    } else {
      Serial.println("FAILED to send gesture: " + http.errorToString(httpResponseCode));
    }

    http.end();  // Close the HTTP connection
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to WiFi");




  // Configure flex sensor pins as input
  for (int i = 0; i < 5; i++) {
    pinMode(flexPins[i], INPUT);
  }

  Serial.println("Starting calibration...");

  // Take readings for the open hand (minimum values)
  Serial.println("Please keep your hand open...");
  delay(5000); // Wait for 5 seconds
  for (int i = 0; i < 5; i++) {
    flexMin[i] = analogRead(flexPins[i]);
  }
  Serial.println("Open hand calibration complete.");

  // Take readings for the closed hand (maximum values)
  Serial.println("Please close your hand into a fist...");
  delay(5000); // Wait for 5 seconds
  for (int i = 0; i < 5; i++) {
    flexMax[i] = analogRead(flexPins[i]);
  }
  Serial.println("Closed hand calibration complete.");

  // Print calibration values
  Serial.println("Calibration Results:");
  for (int i = 0; i < 5; i++) {
    Serial.printf("Flex Sensor %d: Min = %d, Max = %d\n", i + 1, flexMin[i], flexMax[i]);
  }
}

String currchar="Z";
unsigned long currentTime =0;
void loop() {
  int flexValues[5];
  int flexBinary[5];

  // Read and process values from flex sensors
  for (int i = 0; i < 5; i++) {
    flexValues[i] = analogRead(flexPins[i]);

    // Convert to binary: 0 for open, 1 for closed
    int threshold = (flexMin[i] + flexMax[i]) / 2;
    flexBinary[i] = (flexValues[i] > threshold) ? 1 : 0;
  }

  // Print sensor readings and binary values
  Serial.println("Sensor Readings:");
  String readings="";
  for (int i = 0; i < 5; i++) {
    readings+=String(flexBinary[i]);
    // Serial.printf("Flex Sensor %d: Value = %d, Binary = %d\n", i + 1, flexValues[i], flexBinary[i]);
  }
  // Serial.printf("%s\n", readings.c_str());
  // Serial.printf("%s\n", checkResult(readings).c_str());
  if (checkResult(readings) != currchar || millis() - currentTime > 2500) {
    Serial.println("newupdate");
    Serial.printf("%s\n", checkResult(readings).c_str());
    currchar = checkResult(readings);
    setChar(currchar);
    currentTime = millis();
    Serial.println("Flex Sensor Values:");
  for (int i = 0; i < 5; i++) {
    flexValues[i] = analogRead(flexPins[i]);
    Serial.printf("Flex Sensor %d: %d\n", i + 1, flexValues[i], flexBinary[i]);
  }
  }

  Serial.println("----------------------------------");
  delay(3000); // Short delay before the next loop iteration
}
