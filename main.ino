#include <SoftwareSerial.h>
#include<String.h>
#include<TinyGPSPlus.h>
#define echoPin1 8
#define trigPin1 9 
#define echoPin2 2
#define trigPin2 3
#define echoPin3 4
#define trigPin3 5
#define buzzer 7
#define POWER_PIN 6 
#define SIGNAL_PIN A5
#define heartsensor A2
#define emergencybutton 12

SoftwareSerial mySerial(10,11);
SoftwareSerial gpsSerial(0,1);
TinyGPSPlus gps;

int reading = 0;
int reading_final;
int Heart_rate;
long duration, distance,distance1,distance2,distance3,diff1,diff2,diff3;
int z=0,k=1;
long int x=630051;
long int t1=0; 
long int t2=510000;
String CellNumtemp="";
String textMessage="";
String CellNum="";
String loct="";
char r;
int buttonstate=0;
float lat = 12.7494,lon = 80.1986;
void SendMessage();
void RecieveMessage();
void SendMessagenum();
int value = 0;
const int sensorMin = 0;    
const int sensorMax = 1024; 


void setup()
{
  Serial.begin (9600);
  pinMode(emergencybutton,OUTPUT);
  gpsSerial.begin(9600);
  mySerial.begin(9600);  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(A0,INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(POWER_PIN, OUTPUT);   
  digitalWrite(POWER_PIN, LOW); 
}

void loop() {
 
  flameDetection();
  waterDetection();
  SonarSensor(trigPin1, echoPin1);
  distance1 = distance;
  SonarSensor(trigPin2, echoPin2);
  distance2 = distance;
  SonarSensor(trigPin3, echoPin3);
  distance3 = distance;

  Serial.print("Distance1 = ");
  Serial.println(distance1);
  Serial.print("Distance2 = ");
  Serial.println(distance2);
  Serial.print("Distance3 = ");
  Serial.println(distance3);

  diffFunction();
  
  k=stairs(distance1,distance2,distance3);

  if(z==3 && k==1){
    
    if((diff1)<15&& (diff2)<15 &&(diff3)<15){
    Serial.println("Wall Detected");
    tone(buzzer,200);
    delay(500);
   
    noTone(buzzer);
    delay(500);
  } 
}

while(gpsSerial.available()){  
  if(gps.encode(gpsSerial.read())) 
  { if (gps.location.isValid()){
    Serial.print("Latitude: ");
    lat=gps.location.lat();
    lon=gps.location.lng();
    Serial.println(lat, 6);
    Serial.print("Longitude: ");
    Serial.println(lon, 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
   else
  { Serial.println("Location: Not Available"); }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else{
    Serial.println("Not Available");
  }
 }
}
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    //Serial.println("No GPS detected");
  }
  
  String latitude = String(lat,6); 
  String longitude = String(lon,6); 
  loct=latitude+','+longitude;
  buttonstate=digitalRead(emergencybutton);
  
  if (buttonstate==HIGH){
    Serial.println("Emergency Button Activated !");
    SendMessage();
    delay(2000);
 }
 t2=millis();
 RecieveMessage();
 
 reading=0;
 
 for(int i=0;i<5;i++){
     reading = reading + analogRead(heartsensor);
}
      reading_final = (reading)/5;
      Serial.print("Heat Beat : ");         
      Serial.println(reading_final);   
      if(reading_final <40){
        SendMessage();
        delay(2000);    
      }
  
  z=0;
  k=1;

}

void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if(distance<100)
       z+=1;
}

int stairs(int distance1,int distance2, int distance3){
  if (((diff1)>25 ||  (diff2)>25|| (diff3)>25) && distance1<100 && distance2<100 && distance3<100){
      Serial.println("Stairs Ahead !");
      tone(buzzer,200);
      delay(500);
      noTone(buzzer);
      delay(500);
      tone(buzzer,200);
      delay(500);
      noTone(buzzer);
      delay(500);
      return 0;
  }
  return 1;
}

void flameDetection(){
  
  int sensorReading = analogRead(A0);
  
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  switch (range) {
  case 0:    // A fire closer than 1.5 feet away.
    Serial.println("**  Fire **");
     tone(buzzer,1000,5000);
    break;
  case 1:    // A fire between 1-3 feet away.
    Serial.println("** No Fire **");
    //tone(buzzer,800,2000);
    break;
  case 2:    // No fire detected.
    Serial.println("No Fire");
    break;
  }
  delay(1); 
}

void waterDetection(){
  digitalWrite(POWER_PIN, HIGH);  
  delay(10);                      
  value = analogRead(SIGNAL_PIN); 
  digitalWrite(POWER_PIN, LOW);   
  


  if(value>350){
  Serial.print("Water Detected - Sensor value: ");
  Serial.println(value);
  tone(buzzer, 500); 
  delay(1000);
  noTone(buzzer);
  delay(1000);   
  }
}

int diffFunction(){
    
    if(distance1-distance2<0)
      diff1=distance2-distance1;
    
    else
      diff1=distance1-distance2;
    
    if(distance2-distance3<0)
      diff2=distance3-distance2;
    
    else
      diff2=distance2-distance3;
    
    if(distance3-distance1<0)
      diff3=distance1-distance3;
    
    else
      diff3=distance3-distance1;

    return diff1,diff2,diff3;
}

void SendMessage()
{ //emergency number 1
  mySerial.println("AT+CMGF=1");    
  delay(1000);
  mySerial.println("AT+CMGS=\"+916300517015\"\r");
  delay(1000);
  Serial.println("sending message ");
  mySerial.print("The latitude and longitude is ");
  mySerial.print(loct);
  mySerial.print("\n");
  mySerial.print("Check Map: \n");
  mySerial.print("http://www.google.com/maps/place/");
  mySerial.println(loct);
  mySerial.println((char)26);
  
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+918940899999\"\r");
  delay(1000);
 // Serial.println("sending message to number 2");
  mySerial.print("The latitude and longitude is ");
  mySerial.print(loct); 
  mySerial.print("\n");
  mySerial.print("Check Map: \n");
  mySerial.print("http://www.google.com/maps/place/");
  mySerial.println(loct);
  mySerial.println((char)26);
 
  mySerial.println("AT+CMGF=1");
     delay(1000);
  //emergency number 3
  mySerial.println("AT+CMGS=\"+919080681237\"\r"); 
  delay(1000);
  //Serial.println("sending message to number 3");
  mySerial.print("The latitude and longitude is ");
  mySerial.print(loct); 
  mySerial.print("\n");
  mySerial.print("Check Map: \n");
  mySerial.print("http://www.google.com/maps/place/");
  mySerial.println(loct);
  mySerial.println((char)26);
}

void SendMessagenum()
{ Serial.println("Send Message after receiving message");
  Serial.println(CellNum);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.print("AT+CMGS=\"");
  mySerial.print(CellNum);
  mySerial.print("\"\r");
  delay(1000);
  mySerial.print("The latitude and longitude is ");
  mySerial.print(loct);
  mySerial.print("\n");
  mySerial.print("Check Map: \n");
  mySerial.print("http://www.google.com/maps/place/");
  mySerial.println(loct);
  mySerial.println((char)26);
  Serial.println("the SendMessagenum function is completed");
  delay(1000);
  
}

void RecieveMessage()
{ CellNum="";
  mySerial.println("AT+CNMI=2,2,0,0,0"); 
  delay(1000);
  textMessage="";
  textMessage=mySerial.readString();
  Serial.println(textMessage);
  CellNumtemp = textMessage.substring(textMessage.indexOf("+91"));
  CellNum = CellNumtemp.substring(0,13);
  if (textMessage.length()>0)
   {if(textMessage.indexOf("location")>=0){
    Serial.println("inside location loop");
    SendMessagenum();
    textMessage="";
    }}
   textMessage="";
  delay(1000);
}
