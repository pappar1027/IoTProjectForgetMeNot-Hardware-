//get the find information (from the user end) from server 
//light up LED
#include <SoftwareSerial.h>
#include <stdlib.h>
#include<string.h>

// LED
int ledPin = 13;
// LM35 analog input
int lm35Pin = 0;
const int DrawerLED=5;
String inData="";




// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // RX, TX
 

void findValue(String inData){
   //the data string will be in the form of ....+IPD,1:0(or1)CLOSED, this information has length =60
  int i = ':';   
  int len=inData.length();
  //Serial.print(len);
  if (len==60){
    while (inData.charAt(i) != ':')
    i++;
    inData = inData.substring(i+1,i+2);  
    }else{return;}
    Serial.println("value");
    Serial.println(inData);
    if(inData=="0"){Serial.println("LED OFF");digitalWrite(DrawerLED,LOW); }
 else if(inData=="1"){Serial.println("LED ON");digitalWrite(DrawerLED,HIGH); }}

  
// this runs once
void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);  
  pinMode(DrawerLED, OUTPUT);  
  

  // enable debug serial
  Serial.begin(9600);
  // enable software serial
  ser.begin(9600);

  // reset ESP8266
  //ser.println("AT+RST");
  ser.println("AT+RST");

delay(1000);
//reset the esp8266 module
if(ser.find("OK") ) Serial.println("Module Reset");
else{Serial.println("cannot reset");}
}




// the loop
void loop() {
  
  if (Serial.available()){
    int value=Serial.read();
    Serial.print(value);
    ser.println(value);}
    
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  //Serial.print(ser.readString());  
  if(ser.find("Error")){
    Serial.println("AT+CIPStart error");
  
  }
  String snd="AT+CIPSEND=\"";
  ser.println("AT+CIPSEND=100");
  Serial.print(ser.readString());
  if(ser.find("Error")){
    Serial.println("AT+CIPsend error");}    
delay(500);
  ser.println("GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=WK4RHTZ9DZSOI42N"
 );

 delay(1500);

  
  delay(500);
  
  ser.println("AT+CIPCLOSE=\r\n");
  //Serial.print(ser.readString());
  
 inData=ser.readString();
 
 
   
   
    if(ser.find("Error")){
    Serial.println("get error");
    return;
  } 
Serial.println("value");
findValue(inData);
//Serial.print(inData);
    
 Serial.println("end"); 
 

  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);
  
}


