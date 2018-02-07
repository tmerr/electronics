#include "WebServer.hpp"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include "build/index.hpp"

WebServer::WebServer(): server(80) {
    this->z = 0;
}

void WebServer::setup(char* ssid, char* password) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [&]() {
      server.send(200, "text/html", INDEX_HTML);
  });

  server.on("/feed", [&](){
    this->feedCallback();
    server.send(200, "text/plain", "feeding");
  });

  server.on("/unfeed", [&](){
    this->unfeedCallback();
    server.send(200, "text/plain", "unfeeding");
  });

  server.on("/state", [&](){
    String json = "{\"ip\": \"";
    json += WiFi.localIP().toString();
    json += "\",\"z\": ";
    json += String(this->z);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.onNotFound([&](){
      String message = "File Not Found\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += (server.method() == HTTP_GET)?"GET":"POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";
      for (uint8_t i=0; i<server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }
      server.send(404, "text/plain", message);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void WebServer::loop() {
  server.handleClient();
}

void WebServer::registerFeedCallback(std::function<void(void)> f) {
  this->feedCallback = f;
}

void WebServer::registerUnfeedCallback(std::function<void(void)> f) {
  this->unfeedCallback = f;
}

void WebServer::setZ(int val) {
  this->z = val;
}
