
// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

// set id for sensor
const int id = 1;

// Replace with your network credentials
const char* ssid = "XXX";
const char* password = "XXX";

// Initialize Telegram BOT
const char* BotKey = "XXX";
const char* chat_id = "XXX";
String text;

// Fingerprint for URL, expiration date = 2022-05-24 00:17:38
const uint8_t fingerprint[20] = {0xf2, 0xad, 0x29, 0x9c, 0x34, 0x48, 0xdd, 0x8d, 0xf4, 0xcf, 0x52, 0x32, 0xf6, 0x57, 0x33, 0x68, 0x2e, 0x81, 0xc1, 0x90};
// thresholds
const float t_threshold = 35.0;
const float h_threshold = 35.0;

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTPOWPIN 5   // power to DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.setTimeout(2000);
  
  // disable WiFi, coming from DeepSleep, as we do not need it right away
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay( 1 );

  pinMode (DHTPOWPIN, OUTPUT);   //output mode
  digitalWrite(DHTPOWPIN, HIGH);   //turn on DHT sensor
  delay( 100 );
  dht.begin();

}

void loop() {
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  while (isnan(t) or isnan(h)){
    Serial.println("Failed to read from DHT sensor!");
    delay(1000);
    t = dht.readTemperature();
    h = dht.readHumidity();
  }
  Serial.println(t);
  Serial.println(h);
  
  //alarm function & send to telegram
  if (t > t_threshold or h > h_threshold) {
    char t_str[8];
    char h_str[8];
    sprintf(t_str, "%2.2f", t);
    sprintf(h_str, "%2.2f", h);
    text = String("SomeThing Wrong in sensor ") + id + " ! Temp: " + t_str + "; RH: " + h_str;

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println(".");
    }
    // Print ESP8266 Local IP Address
    Serial.println(WiFi.localIP());

    if (WiFi.status() == WL_CONNECTED) {
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  
      client->setFingerprint(fingerprint);
  
      HTTPClient https;
      String serverPath = String("https://api.telegram.org/bot") + BotKey + "/sendMessage?chat_id=" + chat_id + "&text=" + text;
      // Your Domain name with URL path or IP address with path
      https.begin(*client, serverPath);
  
      // Send HTTP GET request
      int httpResponseCode = https.GET();
      String payload = https.getString();
  
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        Serial.println(payload);
      }
      https.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }

  //ESP.deepSleep(30e6);         //30s
  ESP.deepSleep(3600e6);   //1 hr (max71mins)
}
