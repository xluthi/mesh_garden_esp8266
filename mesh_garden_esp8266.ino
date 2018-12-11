/* Demo/Test sketch for MQTT on an ESP8266
 * 
 * Copyright (C) 2018 Xavier Lüthi
 * 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// Wifi library
#include <ESP8266WiFi.h>
// MQTT library
#include <PubSubClient.h>
#define MQTT_PORT 1883
// Personal configuration (Wifi)
#include "my_config.h"
char my_hostname[10];

// Variables declaration
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

// for MQTT test purposes
long lastMsg = 0;
byte val = 0;

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.printf("Connection to %s", my_wifi.ssid);
  // Actual connection...
  WiFi.begin(my_wifi.ssid, my_wifi.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" done");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());
  sprintf(my_hostname, "ESP%06X", ESP.getChipId());
  // mDNS
  Serial.printf("ESP8266 Chip id = %06X\n", ESP.getChipId());
  Serial.printf("My MAC address is: %s\n", WiFi.macAddress().c_str());
  Serial.printf("My hostname is: %s\n", my_hostname);
  WiFi.hostname(my_hostname);  
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Message [%s] ", topic);
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();

  // test MQTT message
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void mqtt_reconnect() {
  while (!mqtt_client.connected()) {
    if (!mqtt_client.connect(my_hostname)) {
      Serial.printf("!! Error while connecting to the MQTT broker, code: %d\n", mqtt_client.state());
      delay(500);
      continue;
    }
    Serial.println("Connection to MQTT broker done :-)");
    mqtt_client.subscribe("ctrl_led");
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); 
  setup_wifi();
  mqtt_client.setServer(mqtt_server_ip, MQTT_PORT);
  mqtt_client.setCallback(mqtt_callback);
}

void loop() {
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }

  // main MQTT loop
  mqtt_client.loop();

  //sample MQTT messages
  char msg[16];
  char topic[64];
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    val++;
    sprintf(msg, "hello world  #%hu", val);
    sprintf(topic, "house/%s/value", my_hostname);
    Serial.printf("Sending message '%s' to topic '%s'\n", msg, topic); 
    mqtt_client.publish(topic, msg);
  }
}
