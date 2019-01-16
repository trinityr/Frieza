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

#include <DS18B20Events.h>
#include <DallasTemperature.h>
#include <Config.h>



// Replace with your network details
const char* ssid = "Enter your SSID";     //  your network SSID (name) 
const char* pass = "SSID Password";    // your network password

// the IP address of your InfluxDB host
byte host[] = {127, 0, 0, 1};

// the port that the InfluxDB UDP plugin is listening on
int port = 8089;


#define PiezoPin 4 //D2 on NodeMCU
#define ONE_WIRE_BUS 5 //D1 on NodeMCU
// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 9
// Index of sensors connected to data pin, default: 0
#define SENSOR_INDEX 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

WiFiServer server(80);

WiFiUDP udp;



void setup () {

  Serial.begin(9600);

   // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());

  sensors.begin();
  sensors.getAddress(sensorDeviceAddress, 0);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
}

void loop() {
   // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  // Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  // Serial.print(sensors.getTempFByIndex(0)); 
  float TempF = sensors.getTempFByIndex(SENSOR_INDEX);

  String line, temperature;
  temperature = String(TempF,4);

  if (TempF > 75.0) {
    //Buzzer
    tone(PiezoPin, 1000, 1000);
    //Log
    line = String("temperature value=" + temperature);
    Serial.println(line);
    Serial.print("Over Temp! ");
    Serial.print(TempF, 4);
    Serial.println(" Fahrenheit");
      // send the packet
       Serial.println("Sending UDP packet...");
       udp.beginPacket(host, port);
       udp.print(line);
       udp.endPacket();
    //Email
  }
  else if (TempF < 50.0) {
    //Buzzer
    tone(PiezoPin,500,1000);
   //Log
    line = String("temperature value=" + temperature);
    Serial.println(line);
    Serial.print("Under Temp! ");
    Serial.print(TempF, 4);
    Serial.println(" Fahrenheit");
      // send the packet
       Serial.println("Sending UDP packet...");
       udp.beginPacket(host, port);
       udp.print(line);
       udp.endPacket();
    //Email
  }
  else {
    //Log and move on
    //Log
    line = String("temperature value=" + temperature);
    Serial.println(line);
    Serial.print(TempF, 4);
    Serial.println(" Fahrenheit");
      // send the packet
       Serial.println("Sending UDP packet...");
       udp.beginPacket(host, port);
       udp.print(line);
       udp.endPacket();
  }
  
  delay(300000);
}
