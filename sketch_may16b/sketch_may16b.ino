/*
 * A better version of MakerBotwPS2 example used with makerbot BANHMI hardware platform
 * This version simplify the joystick driving method, where the functionality stay the same.
 * Added new controling menthod, switch driving mode by pressing SELECT button
 * Added nitro like speed booster when holding L2
 * 
 * Writen by Tu Dang - Makerviet
 * Optimize by Do Dinh Hoang Anh (a.k.a Rimi) - Xuan Dinh High School
 * This code include OTA Update because Type-C port has broken due to physic reason, you need changed wifi ssid and password to use that, make sure your computer and esp32 connected to the same network (Modified by Rimi)
 */

//For controller
#include "motors.h"
#include "PS2_controller.h"

//For wifi
//#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <AsyncElegantOTA.h>

//AsyncWebServer server(80);

//const char* hostName = "xuandinh";
//const char* ssid = "XuanDinhWinner";
//const char* password = "G4m3L@D3";

void setup()
{
  Serial.begin(115200);
  
  //WiFi.mode(WIFI_AP);
  //WiFi.setHostname(hostName);
  //WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  //WiFi.softAP(ssid, password);
  
  //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //  request->send(200, "text/plain", "Lmao");
  //});
  //AsyncElegantOTA.begin(&server, "RoXD23", "coderdeptrai");    // Start AsyncElegantOTA
  //server.begin();
  //Serial.println("HTTP server started");
  initMotors();
  setupPS2controller();
}

void loop()
{
  ps2x.read_gamepad(0, 0);
  PS2control();
  delay(50);
}
