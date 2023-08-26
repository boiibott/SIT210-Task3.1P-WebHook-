#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

#define DHTPIN 4
#define DHTTYPE DHT11

char ssid[] = "Galaxy M30s2DOD"; // wiii ssid
char pass[] = "vcvw3532";// wifi password 

unsigned long channelID = 2250545;           // Your ThingSpeak Channel ID
const char *apiKey = "9OU7D717BJPF74OF";          // Your ThingSpeak API Key

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  establishWiFiConnection();
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    sendToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Failed to read data from DHT sensor!");
  }
  
  delay(30000);
}

void establishWiFiConnection() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void sendToThingSpeak(float temperature, float humidity) {
  ThingSpeak.setField(channelID, 1, temperature, apiKey);
  ThingSpeak.setField(channelID, 2, humidity, apiKey);

  int response = ThingSpeak.writeFields(channelID);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP Response Code: ");
    Serial.println(response);
  }
}
