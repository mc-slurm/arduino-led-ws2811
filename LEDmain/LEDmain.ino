// Load Wi-Fi library
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include "LEDController.h"
#include "NTPTime.h"
#include "Logger.h"

/** Todos
    - doc
    - set first led red if no wifi connection
    - LEDControlPositionBySlider
    - PIMPL
*/

// Set web server port number to 80
WebServer Server;
AutoConnect Portal(Server);

uint32_t uiInfoMessageTimestepSeconds = 5;
uint32_t uiInfoMessageCurTime = 0;

uint32_t uiErrorLEDCounter = 0;
uint32_t uiErrorLEDTimeSeconds = 2;


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

void printWifiStatus()
{
  tm timeInfo = NTPTime::GetInstance().GetTime();
  if ((uint32_t)timeInfo.tm_sec >= uiInfoMessageCurTime)
  {
    uiInfoMessageCurTime = ((uint32_t)timeInfo.tm_sec + uiInfoMessageTimestepSeconds) % 60;
    Serial.print(&timeInfo, "[%A, %B %d %Y %H:%M:%S] ");

    switch(WiFi.status())
    {
    case WL_NO_SHIELD:        Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_NO_SHIELD\n"); break;
    case WL_IDLE_STATUS:      Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_IDLE_STATUS\n"); break;
    case WL_NO_SSID_AVAIL:    Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_NO_SSID_AVAIL\n"); break;
    case WL_SCAN_COMPLETED:   Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_SCAN_COMPLETED\n"); break;
    case WL_CONNECTED:        Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_CONNECTED\n"); break;
    case WL_CONNECT_FAILED:   Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_CONNECT_FAILED\n"); break;
    case WL_CONNECTION_LOST:  Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_CONNECTION_LOST\n"); break;
    case WL_DISCONNECTED:     Serial.println("IP address: " + WiFi.localIP().toString() + " - WiFi status: WL_DISCONNECTED\n"); break;
    }
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Loading...\n");

  Serial.println("Init Logger");
  Logger::GetInstance().RegisterPrintFunction(printCallback);

  {
    LEDController::GetInstance().Setup("", 27, 120, 200, 60);
    LEDController::GetInstance().ShowStartupLED(500);
  }
  
  Serial.println("Init Portal");
  if (Portal.begin())
  {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    LEDController::GetInstance().Setup(WiFi.localIP().toString(), 27, 120, 200, 60);
    LEDController::GetInstance().ShowInitLED(100);
  }
  else
  {
    Serial.println("WiFi connection failed");
    LEDController::GetInstance().Setup("", 27, 120, 200, 60);
    LEDController::GetInstance().ShowErrorLED(5000);
  }

  Logger::GetInstance().RegisterPrintFunction(printCallback);
  LEDController::GetInstance().Setup(WiFi.localIP().toString(), 27, 120, 200, 60);
  std::vector<String> subPages = LEDController::GetInstance().GetSubPageLinks();
  
  // Websites
  Serial.println("Init Webserver");
  Server.on("/", OnRootEvent);
  Server.on("/global", OnEvent);
  for (int i = 0; i < subPages.size(); ++i)
  {
    Server.on(subPages[i], OnEvent);
  }

  Serial.println("Init NTPTime");
  NTPTime::GetInstance().Init(3600, 3600, "pool.ntp.org");
  tm timeInfo = NTPTime::GetInstance().GetTime();
  Serial.println(&timeInfo, "%A, %B %d %Y %H:%M:%S");
}

void loop() {
  Portal.handleClient();

  // get current time
  tm timeInfo = NTPTime::GetInstance().GetTime();
  LEDController::GetInstance().SetTime((uint8_t)timeInfo.tm_hour, (uint8_t)timeInfo.tm_min);

  printWifiStatus();
  
  if (WiFi.status() != WL_CONNECTED)
  {
    if (uiErrorLEDCounter < 20)
    {
      LEDController::GetInstance().ShowErrorLED(uiErrorLEDTimeSeconds * 1000);
      ++uiErrorLEDCounter;
    }
    else
    {
      LEDController::GetInstance().ShowNoLED();
    }
  }

  LEDController::GetInstance().Loop();
}
