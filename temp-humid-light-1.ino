/* Built to run on ESP-01 for being small and powered by 3.3v directly. 
 *  Changed to work on wemos mini D1 by mk
 *  
 *  
 *  This file includes:
   SHT21 Temperature and humidity sensor
   GY49 LUX sensor
   OTA support * disabled
   MQTT support
   Made by Daniel Römer 2019 for homeautomation.
*/

#include <ESP8266WiFi.h>  //For ESP8266
#include <PubSubClient.h> //For MQTT
#include <MAX44009.h>
#include "SparkFun_Si7021_Breakout_Library.h"

 



//WIFI configuration

#define wifi_ssid "Yoyr_ssid"
#define wifi_password "Your_Password"
#define board_name "temp-humid-light-1"

//MQTT configuration
#define mqtt_server "SERVER_IP"
#define mqtt_user "MQTT_UserName"
#define mqtt_password "MQTT_Password"
String mqtt_client_id="ESP-";   //This text is concatenated with ChipId to get unique client_id
//MQTT Topic configuration



String mqtt_base_topic="sensor";
#define humidity_topic "/humidity"
#define temperature_topic "/temperature"
#define lux_topic "/lux"


//MQTT client
WiFiClient espClient;
PubSubClient mqtt_client(espClient);


MAX44009 light;
Weather sensor;


void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(wifi_ssid);
  WiFi.hostname(board_name);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  Serial.print("   IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() { 
  Serial.begin(115200);
  Serial.println("\r\nBooting...");
  
  setup_wifi();


  Serial.println("OK");

  Serial.println("Configuring MQTT server...");
  mqtt_client_id=board_name;
  mqtt_base_topic=mqtt_base_topic+"/" + mqtt_client_id;
  mqtt_client.setServer(mqtt_server, 1883);
  Serial.printf("   Server IP: %s\r\n",mqtt_server);  
  Serial.printf("   Username:  %s\r\n",mqtt_user);
  Serial.println("   Cliend Id: "+mqtt_client_id);  
  Serial.println("   MQTT configured!");

  //Wire.begin(0,2);
  sensor.begin();
  light.begin();

  Serial.println("Setup completed! Running app...");
}


void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {    
    if (mqtt_client.connect(mqtt_client_id.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return(true);
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}



long now =0; //in ms
long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;
int min_timeout=6000; //in ms


float lux = 0;

void loop() {
  
//  ArduinoOTA.handle();
  
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();

  now = millis();
  if (now - lastMsg > min_timeout) {
    lastMsg = now;
    now = millis();
    //getData();
    float newTemp = sensor.getTemp();
    float newHum = sensor.getRH();
    float lux = light.get_lux();


    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("Sent ");
      Serial.print(String(temp).c_str());
      Serial.println(" to "+mqtt_base_topic+temperature_topic);
      mqtt_client.publish((mqtt_base_topic+temperature_topic).c_str(), String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("Sent ");
      Serial.print(String(hum).c_str());
      Serial.println(" to "+mqtt_base_topic+humidity_topic);
         mqtt_client.publish((mqtt_base_topic+humidity_topic).c_str(), String(hum).c_str(), true);
    }

    mqtt_client.publish((mqtt_base_topic+lux_topic).c_str(), String(lux).c_str(), true);
    
  }
}
