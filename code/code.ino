//Author - Aditya Raj
//Only for ESP-8266 ONLY...
//If needed Change The libraries compatible with ESP32...
//button controls will be added soon


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <FirebaseESP8266.h>


// Set these to your Firebase project credentials
#define WIFI_SSID "Galgotias-H" //ssid
#define WIFI_PASSWORD ""  //password (blank for no password)
#define API_KEY "AIzaSyCZTx5qW355GSRTDeprdvP-a9Akzy79rwI"
#define DATABASE_URL "https://esp-74hc959n-3led-control-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "aditya@4net.in"
#define USER_PASSWORD "Aditya321"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int LED1_PIN = D1;
const int LED2_PIN = D2;
const int LED3_PIN = D4;  //change to D3

FirebaseData firebaseData;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

digitalWrite(LED1_PIN, LOW); 
digitalWrite(LED2_PIN, LOW);
digitalWrite(LED3_PIN, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
    WiFiClientSecure client;

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);

 /* //Not Working....
 Firebase.setInt("led1", ledState1);
  Firebase.setInt("led2", ledState2);
  Firebase.setInt("led3", ledState3);
  */
}

void loop() {
  // Check and control LED1
  if (Firebase.getInt(firebaseData, "/Value1")) {
    if (firebaseData.dataType() == "int") {
      int value1 = firebaseData.intData();
      digitalWrite(LED1_PIN, value1 ? HIGH : LOW);
    } else {
      Serial.println("Data type is not an integer for /value1");
    }
  } else {
    Serial.print("Failed to get value1: ");
    Serial.println(firebaseData.errorReason());
  }

  // Check and control LED2
  if (Firebase.getInt(firebaseData, "/Value2")) {
    if (firebaseData.dataType() == "int") {
      int value2 = firebaseData.intData();
      digitalWrite(LED2_PIN, value2 ? HIGH : LOW);
    } else {
      Serial.println("Data type is not an integer for /value2");
    }
  } else {
    Serial.print("Failed to get value2: ");
    Serial.println(firebaseData.errorReason());
  }

  // Check and control LED3
  if (Firebase.getInt(firebaseData, "/Value3")) {
    if (firebaseData.dataType() == "int") {
      int value3 = firebaseData.intData();
      digitalWrite(LED3_PIN, value3 ? HIGH : LOW);
    } else {
      Serial.println("Data type is not an integer for /value3");
    }
  } else {
    Serial.print("Failed to get value3: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(200); // Adjust the delay as needed
}
