#ifndef _WEBSERVER_HPP
#define _WEBSERVER_HPP
#include <functional>
#include <ESP8266WebServer.h>

class WebServer {
public:
  WebServer();
  void setup(char* ssid, char* password);
  void loop();
  void registerFeedCallback(std::function<void(void)> f);
  void registerUnfeedCallback(std::function<void(void)> f);

private:
  ESP8266WebServer server;
  std::function<void(void)> feedCallback;
  std::function<void(void)> unfeedCallback;
};
#endif
