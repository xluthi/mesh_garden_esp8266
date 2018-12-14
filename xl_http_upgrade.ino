/*
   Copyright (C) 2018 Xavier Lüthi

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


   Provide firmware upgrade via HTTP
   based on https://github.com/Hackable-magazine/Hackable25/blob/master/espFSwebNTP/espFSwebNTP.ino
*/

#include "xl_debug.h"
#include <ESP8266httpUpdate.h>

const char* urlbase = "http://espupgrade.local/esp/mesh_garden_esp8266/";
extern WiFiClient wifi_client;

void xl_http_upgrade() {
  // first get "version" file and compare with current VERSION,
  // then update if required.
  DEBUG_PRINTLN("Check HTTP version available...");
  String url_version = String(urlbase);
  url_version.concat("version.txt");
  HTTPClient httpClient;
  //httpClient.begin(wifi_client, url_version);
  httpClient.begin(url_version);
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK) {
    String nVersion = httpClient.getString();
    nVersion.trim();
    Serial.printf("Current version is %d, remote version is %ld\n", VERSION, nVersion.toInt());
    // Upgrade only if remote version is newer (higher) than current one
    if (nVersion.toInt() > VERSION) {
      Serial.print("An update is available: ");
      // build upgrade URL
      String urlfirmware = String(urlbase);
      urlfirmware.concat("firmware.");
      urlfirmware.concat(nVersion);
      Serial.println(urlfirmware);
      Serial.println("Upgrading, please wait... ");
      // actual upgrade
      t_httpUpdate_return ret = ESPhttpUpdate.update(urlfirmware);
      if (ret == HTTP_UPDATE_FAILED) {
        // we should never reach here as the ESP should reboot before...
        Serial.print("!! Error while upgrading  (");
        Serial.print(ESPhttpUpdate.getLastError());
        Serial.print(") : ");
        Serial.println(ESPhttpUpdate.getLastErrorString().c_str());
      }
    } else {
      // VERSION >= remote version
      Serial.println("No update available.");
    }
  } else {
    // I cannot find "version.txt" on the remote server
    Serial.println("!! Error while fetching version.txt !");
  }

}

