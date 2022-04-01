// Load Wi-Fi library
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include "LEDController.h"
#include "NTPTime.h"

/** Todos
    - define global logger class
    - make first led red if no wifi connection
    - LEDControlPositionBySlider
    - PIMPL
*/

// Set web server port number to 80
WebServer Server;
AutoConnect Portal(Server);

void OnRootEvent()
{
  String str;
  LEDController::GetInstance().CreateHTML(str);
  Server.send(200, "text/html", str);  
}

void OnEvent()
{
    // Extract arguments
    std::vector<std::pair<String,String>> arguments(Server.args());
    for (int i = 0; i < Server.args(); i++)
    {
        arguments[i] = std::make_pair(Server.argName(i), Server.arg(i));
        Serial.println(Server.argName(i) + " -> " + Server.arg(i));
    }

    Serial.println(Server.uri());
    String html;
    LEDController::GetInstance().OnEvent(Server.uri(), arguments, html);
    Server.send(200, "text/html", html);
}

void printCallback(const String& rString)
{
  Serial.println(rString);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Hello\n");
  
  if (Portal.begin())
  {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.println("WiFi connection failed\n");
  }

  LEDController::GetInstance().RegisterPrintFunction(printCallback);
  LEDController::GetInstance().Setup(WiFi.localIP().toString(), 27, 120, 200, 60);
  std::vector<String> subPages = LEDController::GetInstance().GetSubPageLinks();
  
  // Websites
  Server.on("/", OnRootEvent);
  Server.on("/global", OnEvent);
  for (int i = 0; i < subPages.size(); ++i)
  {
    Server.on(subPages[i], OnEvent);
  }

  NTPTime::GetInstance().Init(3600, 3600, "pool.ntp.org");
  tm timeInfo = NTPTime::GetInstance().GetTime();
  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
}

void loop() {
  if (!Portal.client().available())
  {
    LEDController::GetInstance().ShowErrorLED();
  }
  else
  {
    Portal.handleClient();
  
    // get current time
    tm timeInfo = NTPTime::GetInstance().GetTime();
    LEDController::GetInstance().SetTime((uint8_t)timeInfo.tm_hour, (uint8_t)timeInfo.tm_min);
    
    LEDController::GetInstance().Loop();
  }
}
