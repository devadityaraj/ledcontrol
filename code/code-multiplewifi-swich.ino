// Author - Aditya Raj
// Only for ESP-8266 ONLY...
// If needed, change the libraries compatible with ESP32...

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseESP8266.h>

// Define Wi-Fi credentials
struct WiFiCredentials {
  const char* ssid;
  const char* password;
};

// List of Wi-Fi networks to try 
//add only three if you have to add more than change the code i < 3 in connecttowifi....
WiFiCredentials wifiNetworks[] = {
  {"Aditya-Desk", "12312345"},
  {"Galgotias-H", ""},
  {"GCET-122", "korean@122"}
};

// Firebase credentials
#define API_KEY "AIzaSyCZTx5qW355GSRTDeprdvP-a9Akzy79rwI"
#define DATABASE_URL "https://esp-74hc959n-3led-control-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "aditya@4net.in"
#define USER_PASSWORD "Aditya321"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int LED1_PIN = D1;
const int LED2_PIN = D2;
const int LED3_PIN = D3;

const int BUTTON1_PIN = D5;
const int BUTTON2_PIN = D6;
const int BUTTON3_PIN = D7;

int ledState1 = 0;
int ledState2 = 0;
int ledState3 = 0;

int lastButton1State = HIGH;
int lastButton2State = HIGH;
int lastButton3State = HIGH;

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long debounceDelay = 50;

void connectToWiFi() {
  for (int i = 0; i < 3; i++) { //to add more or less ssid, change the number 3
    WiFi.begin(wifiNetworks[i].ssid, wifiNetworks[i].password);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifiNetworks[i].ssid);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
      delay(500);
      Serial.print(".");
      timeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("Connected to ");
      Serial.println(wifiNetworks[i].ssid);
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      return;
    } else {
      Serial.println();
      Serial.print("Failed to connect to ");
      Serial.println(wifiNetworks[i].ssid);
    }
  }

  Serial.println("Unable to connect to any Wi-Fi network");
}

void updateFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    Firebase.setInt(fbdo, "/Value1", ledState1);
    Firebase.setInt(fbdo, "/Value2", ledState2);
    Firebase.setInt(fbdo, "/Value3", ledState3);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  connectToWiFi();
  Serial.println("");
  Serial.println("WiFi connected");

  WiFiClientSecure client;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);

  if (WiFi.status() == WL_CONNECTED) {
    if (Firebase.getInt(fbdo, "/Value1")) {
      ledState1 = fbdo.intData();
      digitalWrite(LED1_PIN, ledState1 ? HIGH : LOW);
    }
    if (Firebase.getInt(fbdo, "/Value2")) {
      ledState2 = fbdo.intData();
      digitalWrite(LED2_PIN, ledState2 ? HIGH : LOW);
    }
    if (Firebase.getInt(fbdo, "/Value3")) {
      ledState3 = fbdo.intData();
      digitalWrite(LED3_PIN, ledState3 ? HIGH : LOW);
    }
  }
}

void loop() {
  // Check and control LED1
  if (Firebase.getInt(fbdo, "/Value1")) {
    if (fbdo.dataType() == "int") {
      int value1 = fbdo.intData();
      digitalWrite(LED1_PIN, value1 ? HIGH : LOW);
      ledState1 = value1;
    }
  }

  // Check and control LED2
  if (Firebase.getInt(fbdo, "/Value2")) {
    if (fbdo.dataType() == "int") {
      int value2 = fbdo.intData();
      digitalWrite(LED2_PIN, value2 ? HIGH : LOW);
      ledState2 = value2;
    }
  }

  // Check and control LED3
  if (Firebase.getInt(fbdo, "/Value3")) {
    if (fbdo.dataType() == "int") {
      int value3 = fbdo.intData();
      digitalWrite(LED3_PIN, value3 ? HIGH : LOW);
      ledState3 = value3;
    }
  }

  // Handle button presses
  int button1State = digitalRead(BUTTON1_PIN);
  int button2State = digitalRead(BUTTON2_PIN);
  int button3State = digitalRead(BUTTON3_PIN);

  // Button 1 debounce
  if (button1State != lastButton1State) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (button1State == LOW) {
      ledState1 = !ledState1;
      digitalWrite(LED1_PIN, ledState1 ? HIGH : LOW);
      updateFirebase();
    }
  }
  lastButton1State = button1State;

  // Button 2 debounce
  if (button2State != lastButton2State) {
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (button2State == LOW) {
      ledState2 = !ledState2;
      digitalWrite(LED2_PIN, ledState2 ? HIGH : LOW);
      updateFirebase();
    }
  }
  lastButton2State = button2State;

  // Button 3 debounce
  if (button3State != lastButton3State) {
    lastDebounceTime3 = millis();
  }
  if ((millis() - lastDebounceTime3) > debounceDelay) {
    if (button3State == LOW) {
      ledState3 = !ledState3;
      digitalWrite(LED3_PIN, ledState3 ? HIGH : LOW);
      updateFirebase();
    }
  }
  lastButton3State = button3State;

  delay(100); // Adjust the delay as needed
}
