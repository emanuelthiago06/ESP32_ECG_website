#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

void setup() {
 
  Serial.begin(9600);                 //Serial connection
  pinMode(4, INPUT); // Setup for leads off detection LO +
  pinMode(5, INPUT); // Setup for leads off detection LO -
  //WiFi.begin("brisa-2983954", "dtiu2ujf");   //WiFi connection
  WiFi.begin("AndroidAP", "ewcs4018"); 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
  
  String input2;
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    if((digitalRead(4) == 1)||(digitalRead(5) == 1)){
      Serial.println('!');
      input2 = 0;
      
      }
     else{
      // send the value of analog input 0:
      // int in = analogRead(A0)
      input2 = String(analogRead(A0));
      Serial.println(analogRead(A0));
      }
 
    HTTPClient http;    //Declare object of class HTTPClient
    WiFiClient wifiClient;
    http.begin(wifiClient,"http://192.168.183.135:8000/snippets/");      //Specify request destination
    http.addHeader("Content-Type", "aplication/json");  //Specify content-type header
    String input = "{\"amp\":";
    input= input + input2;
    input+= "}";
    int httpCode = http.POST(input);   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(3000);  //Send a request every 30 seconds
 
}
