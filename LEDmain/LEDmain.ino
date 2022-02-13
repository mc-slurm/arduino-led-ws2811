/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include "LEDController.h"
#include "NTPTime.h"

/** Todos
    - set server adress to LEDBase for proper usage (HTML)
    - base functions for html controls
    - add debug-action to control m_printFunc
    - add scheduler
    - add brightness function
    - add num leds function
    - save values (eprom)
    - LEDControlPositionBySlider
    - LEDFireLEDBallByClick
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

  if (Server.uri() == "/global")
  {
    for (int i = 0; i < Server.args(); i++)
    {
      if (Server.argName(i) == "numLED")
      {
        // todo.
      }
    }
  }
  else
  {
    Serial.println(Server.uri());
    String html;
    LEDController::GetInstance().OnEvent(Server.uri(), arguments, html);
    Server.send(200, "text/html", html);
  }
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
  LEDController::GetInstance().Setup(27, 120, 200, 60);
  std::vector<String> subPages = LEDController::GetInstance().GetSubPageLinks();
  
  // Websites
  Server.on("/", OnRootEvent);
  for (int i = 0; i < subPages.size(); ++i)
  {
    Server.on(subPages[i], OnEvent);
  }

  NTPTime::GetInstance().Init(3600, 3600, "pool.ntp.org");
  tm timeInfo = NTPTime::GetInstance().GetTime();
  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
}

void loop() {
  Portal.handleClient();

  LEDController::GetInstance().Loop();
}