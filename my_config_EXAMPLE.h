/*
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
 *
 * 
 * Enter your own configuration below and rename to my_config.h
 * Once update, do not share this file or store it in your git repository!
 */
#ifndef MY_CONFIG_h
#define MY_CONFIG_h
struct wifi_config {
  char wifi_ssid[32];
  char wifi_password[64];
};

const wifi_config my_wifi = {
  "<your-SSID>",
  "<your-Wifi-key"
};

IPAddress mqtt_server_ip(192, 168, 1, 1);
#endif
