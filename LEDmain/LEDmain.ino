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

#define SIMPLE_STARTUP
#ifndef SIMPLE_STARTUP
uint32_t portalTimeoutSeconds = 120;
uint32_t uiErrorRestartTimeSeconds = 10;
#endif

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
  Serial.println("Loading...");

  Serial.println("Init Logger");
  Logger::GetInstance().RegisterPrintFunction(printCallback);

#ifdef SIMPLE_STARTUP
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
#else
  {
    Serial.println("Show LED Ping");
    LEDController::GetInstance().Setup(WiFi.localIP().toString(), 27, 120, 200, 60);
    LEDController::GetInstance().ShowInitLED(100);
  }

  Serial.println("Init Portal");
  
  AutoConnectConfig& rConfig = Portal.getConfig();
  rConfig.portalTimeout = portalTimeoutSeconds * 1000;
  Serial.println("portal timeout: " + String(rConfig.portalTimeout));
  Serial.println("begin timeout: " + String(rConfig.beginTimeout));

  // try to run portal
  bool bPortalRunning = false;
  for (uint8_t i = 0; (i < 3) && !bPortalRunning; ++i)
  {
    bPortalRunning = Portal.begin();

    Serial.println("Init LEDController");
    LEDController::GetInstance().Setup(WiFi.localIP().toString(), 27, 120, 200, 60);
    if (bPortalRunning)
    {
      Serial.println("WiFi connected: " + WiFi.localIP().toString());
      LEDController::GetInstance().ShowInitLED(100);
    }
    else
    {
      Serial.println("WiFi connection failed");

      Serial.println("Restarting in " + String(uiErrorRestartTimeSeconds) + " seconds");
      for (uint32_t i = 0; i < uiErrorRestartTimeSeconds/uiErrorLEDTimeSeconds; ++i)
      {
        LEDController::GetInstance().ShowErrorLED(uiErrorLEDTimeSeconds * 1000);
      }
//      ESP.restart();
    }
  }
#endif
  

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

void printWifiStatus()
{
  tm timeInfo = NTPTime::GetInstance().GetTime();
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

void loop()
{
  Portal.handleClient();

  tm timeInfo = NTPTime::GetInstance().GetTime();
  LEDController::GetInstance().SetTime((uint8_t)timeInfo.tm_hour, (uint8_t)timeInfo.tm_min);

  if ((uint32_t)timeInfo.tm_sec >= uiInfoMessageCurTime)
  {
    uiInfoMessageCurTime = ((uint32_t)timeInfo.tm_sec + uiInfoMessageTimestepSeconds) % 60;
    printWifiStatus();
  }

  
  if (WiFi.status() != WL_CONNECTED)
  {
    printWifiStatus();
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
  else
  {
   
    LEDController::GetInstance().Loop();
  }
}
