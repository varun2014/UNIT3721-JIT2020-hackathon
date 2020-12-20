#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
 
String apiKey = "EBT32OWJNXGBX8IK";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Redmi Note 5 Pro";     // replace with your wifi ssid and wpa2 key
const char *pass =  "";
const char* server = "api.thingspeak.com";
const int sensor_pin = A0;
const int relay_pin = 4;
int sensorData;
char auth[] = "-zvqcVSf2CVuAyVXO1XM8abqemQznDDZ";

WiFiClient client;

BLYNK_READ(V5) //Blynk app has something on V5
{
  sensorData = analogRead(A0); //reading the sensor on A0
  Blynk.virtualWrite(V5, sensorData); //sending to Blynk
}


void setup() 
{
       Serial.begin(115200); 
       Blynk.begin(auth, ssid, pass);
       pinMode(relay_pin, OUTPUT);
       delay(10);
       
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
      Blynk.run();
      float t ;
      t = (100 - ( (analogRead(sensor_pin)/1023.00)* 100.00) );
      
                 if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                                                           
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("moisture: ");
                             Serial.print(t);
                             Serial.println("%. Send to Thingspeak.");
                             if (t<10){
                              digitalWrite(relay_pin, HIGH);
                              delay(1000);
                             }
                             if (t>10){
                              digitalWrite(relay_pin, LOW);
                              delay(1000);
                              
                             }
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(1000);
}
