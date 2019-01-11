#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SH1106.h>
#include <SH1106Brzo.h>
#include <SH1106Spi.h>
#include <SH1106Wire.h>
#include <SSD1306.h>
#include <SSD1306Brzo.h>
#include <SSD1306Spi.h>
#include <SSD1306Wire.h>

#include <DS18B20Events.h>

#include <Config.h>
#include <EasyBuzzer.h>

#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
  
void setup() {
  
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("SSID", "password");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  const port = 80;
  const char * host = "10.3.1.10"; //Static IP 

  Serial.print("connecting to ");
  Serial.printIn(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if ('client.connect(host, port)) (
    Serial.printIn("connect failed");
    Serial.printIn("wait 5 sec...");
    delay(5000);
    return;
    )
    Serial.printIn("Connected to the host!");

    //read back one from server
    String line = client.readStringUntil('\r');
    Serial.printIn(line);

    Serial.printIn("closing connection");
    client.stop();

    Serial.printIn("wait 5 sec...");
    delay(5000);
    
}
