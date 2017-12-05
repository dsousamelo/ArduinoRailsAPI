#include <EventManager.h>
//#include <MsTimer2.h>
#include <DHT_U.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
//////////////////////////////////////////////////////////
#define DHTPIN A0
#define DHTTYPE DHT11
//////////////////////////////////////////////////////////
DHT dht(DHTPIN, DHTTYPE);
EventManager gEM;
int ldrPin = 1; //LDR no pino analígico 8
int ldrValor = 0; //Valor lido do LDR
byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };  

IPAddress  ip(172,18,9,100);
char b[2];
int temperature;
String contentI = "{\"value\":";
String contentI2 = "{\"state\":";
const char* ssid     = "TP-LINK_8DDDAE";
const char* password = "labiotm06";
WiFiClient client;
//////////////////////////////////////////////////////////
void interruptHandler(){
  Serial.println("INTERRUPÇAO");
}

void setup() {
 Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
 /* MsTimer2::set(300000, updateTemp ); // 1 sec period
  MsTimer2::start();  */
  
}

void loop(){
  temperature = dht.readTemperature();
  updateLu();
  updateDoor();
  
  gEM.processEvent();
  
  delay(500);
  
}

void updateTemp(){  
    
  Serial.println(temperature);    
  String str = String(temperature);  
  String contentTemp = contentI+str+"}";  
  
  if(client.connect(ip,3000)) {
    Serial.println("CONECTOU");
    Serial.println(contentTemp);    
    
    client.println("PUT /temperatures/1 HTTP/1.1");
    client.println("Host:172.18.9.100:3000");    
    client.println("User-Agent: Arduino/1.0");
    client.println("Accept: application/json");
    client.print("Content-Length: ");
    client.println(contentTemp.length());    
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(contentTemp); 
 /////////////////////////////////////////////////////////////     
   
  } 
  else {
    Serial.println("connection failed");
  }  

  client.stop();

}

void updateLu(){
  
  ldrValor = analogRead(ldrPin);

  String contentLu;
  
  if(ldrValor>=1020){  
     contentLu = contentI2+"false}";
  
  }
  else{
    contentLu = contentI2+"true}";
  }
  if(client.connect(ip,3000)) {
    Serial.println("CONECTOU LU");
    Serial.println(contentLu);
    client.println("PUT /lights/1 HTTP/1.1");
    client.println("Host:172.18.9.100:3000");    
    client.println("User-Agent: Arduino/1.0");
    client.println("Accept: application/json");
    client.print("Content-Length: ");
    client.println(contentLu.length());    
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(contentLu); 
  
  }
  else {
    Serial.println("connection failed");
 }  
  
   client.stop();
   
}


void updateDoor(){
  
   int doorState = analogRead(2);
   String contentDoor;
  
  if(doorState==0){  
     contentDoor = contentI2+"true}";
  
  }
  else{
    contentDoor = contentI2+"false}";
  }
  if(client.connect(ip,3000)) {
    Serial.println("CONECTOU DOOR");
    Serial.println(contentDoor);
    client.println("PUT /doors/1 HTTP/1.1");
    client.println("Host:172.18.9.100:3000");    
    client.println("User-Agent: Arduino/1.0");
    client.println("Accept: application/json");
    client.print("Content-Length: ");
    client.println(contentDoor.length());    
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(contentDoor); 
  
  }
  else {
    Serial.println("connection failed");
 }  
  
   client.stop();
    
  
  
  
  
  
}
