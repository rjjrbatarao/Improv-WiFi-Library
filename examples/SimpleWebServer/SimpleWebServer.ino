
#define LED_BUILTIN 2

#if defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ImprovWiFiLibrary.h>

AsyncWebServer server(80);
ImprovWiFi improvSerial(&Serial);

char linebuf[80];
int charcount = 0;

void handleWebIndexGet(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", "it works");
  //response->addHeader("Cache-Control", "public, max-age=2678400"); // cache 31 days
  request->send(response);  
}

void onImprovWiFiErrorCb(ImprovTypes::Error err)
{
  server.end();
  blink_led(2000, 3);
}

void onImprovWiFiConnectedCb(const char *ssid, const char *password)
{
  // Save ssid and password here
  server.on("/setup", HTTP_GET, handleWebIndexGet);
  server.begin();
  blink_led(100, 3);
}

bool connectWifi(const char *ssid, const char *password)
{
  WiFi.begin(ssid, password);

  while (!improvSerial.isConnected())
  {
    blink_led(500, 1);
  }

  return true;
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32, "ImprovWiFiLib", "1.0.0", "BasicWebServer", "http://{LOCAL_IPV4}/setup");
  improvSerial.onImprovError(onImprovWiFiErrorCb);
  improvSerial.onImprovConnected(onImprovWiFiConnectedCb);
  improvSerial.setCustomConnectWiFi(connectWifi); 

  blink_led(100, 5);
}

void loop()
{

  improvSerial.handleSerial();

  if (improvSerial.isConnected())
  {

  }
}

void blink_led(int d, int times)
{
  for (int j = 0; j < times; j++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}