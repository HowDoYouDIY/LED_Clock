#include <ESP8266WiFi.h>

void connectToWifi(const char* WIFI_NAME, const char* WIFI_PASSWORD)
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(WIFI_NAME);

  // try connecting to this wifi with this password
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  // loop while not connected to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
