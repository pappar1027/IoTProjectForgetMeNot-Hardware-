//door magnet: door open/close -->post 
//weight change: record a positive/negative weight change and post to the server (set an tolerance)
//test whether can have a few server to post to when necessary
//wait for a few cycles for the connected information before placing the item
#include <SoftwareSerial.h>
#include <stdlib.h>
#include "HX711.h"


// HX711.DOUT  - pin #A1
// HX711.PD_SCK - pin #A0
SoftwareSerial client(10, 11); // RX, TX
HX711 scale(A1, A0);    


// LED
int ledPin = 13;
// LM35 analog input
int lm35Pin = 0;
const int DrawerLED=5;
int doorStatus=0;
int pastDoorStatus=0;
float weight=0;
float pastWeight=0;
const int switchPin = 3;
String inData="";




// this runs once
void setup() 
{
  pinMode(ledPin, OUTPUT);  
  pinMode(DrawerLED, OUTPUT);  
   Serial.begin(9600);
   scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; 
  scale.tare(); 
  client.begin(9600);
   client.println("AT+RST");
delay(500);
//reset the esp8266 module
if(client.find("OK") ) Serial.println("Module Reset");


   Serial.println("System Ready..");
}
void loop() 
{
  delay(500);

  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // connect to api.thingspeak.com where we set the connection with the server
  cmd += "\",80";
  client.println(cmd);
  Serial.println(client.readString());

      delay(1000);
     Serial.println("weight difference is:");
      Serial.println(String(weight-pastWeight));
//send only when there is significant weight change or door open/close
 if(abs(weight-pastWeight)>5|abs(pastDoorStatus-doorStatus)==1){ 
 String postRequest="POST https://api.thingspeak.com/apps/thinghttp/send_request?api_key=S9KZPOIUA3T046HD&weight="+String(weight-pastWeight)+"&status="+String(doorStatus)+ " HTTP/1.0\r\n\r\n";
client.print("AT+CIPSEND=");
client.println(postRequest.length() );

delay(1000);

if(client.find(">")) { 
  
  Serial.println("Sending.."); client.println(postRequest);

//Serial.println(client.readString());
//if(client.find("SEND OK")) { Serial.println("Packet sent");
//
//while (client.available()) {
//
//Serial.println(client.readString());
//}}


     client.println("AT+CIPCLOSE=0"); 
     Serial.println("AT+CIPCLOSE=0");
     //Serial.println(client.readString());
}}
     delay(1000);
  //read the door magnet sensor data     
    if(digitalRead(switchPin) == HIGH){
    digitalWrite(ledPin, HIGH);
    Serial.println("The door is closed");
    pastDoorStatus=doorStatus;
    doorStatus=0;
  }  
  else{
    digitalWrite(ledPin, LOW);
    Serial.println("The door is open");
    pastDoorStatus=doorStatus;
    doorStatus=1;
 
  }
  //update information 
  Serial.print(String(doorStatus));
  Serial.println(inData);
  Serial.print("average:\t");

  pastWeight=weight; 
  weight=scale.get_units(10);
  Serial.print(String(weight));
  
  
  
  


  

}
