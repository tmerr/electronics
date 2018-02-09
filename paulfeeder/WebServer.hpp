#ifndef _WEBSERVER_HPP
#define _WEBSERVER_HPP
#include <functional>
#include <ESP8266WebServer.h>
#include "Mode.hpp"

class WebServer {
public:
  WebServer();
  void setup(char* ssid, char* password);
  void loop();
  void registerFeedCallback(std::function<void(void)> f);
  void registerUnfeedCallback(std::function<void(void)> f);
  void setZ(int val);
  void setMode(Mode mode);

private:
  ESP8266WebServer server;
  std::function<void(void)> feedCallback;
  std::function<void(void)> unfeedCallback;
  int z;
  Mode mode;
};
#endif
