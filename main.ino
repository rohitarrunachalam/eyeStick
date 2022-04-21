#include <PulseSensorPlayground.h> 
#define USE_ARDUINO_INTERRUPTS true   
#define echoPin1 12 
#define trigPin1 13 
#define echoPin2 2
#define trigPin2 3
#define echoPin3 7
#define trigPin3 8
#define motorPin 9
#define buzzer 10 
#define buttonPin 1

//Declare all the variables
long duration;
long distance; 
int distance1;
int distance2;
int distance3;
int z=0,k=0;
int const PULSE_SENSOR_PIN = 0; 
const int PulseWire = 0;       
int Threshold = 550;           
                       
PulseSensorPlayground pulseSensor;

void setup() {

  //Setup all the Pins as INPUT or OUTPUT
  
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT);  
  pinMode(trigPin2, OUTPUT);  
  pinMode(echoPin2, INPUT);   
  pinMode(trigPin3, OUTPUT);  
  pinMode(echoPin3, INPUT);   
  pinMode(buzzer,OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);        
  
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold); 
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");   
  }  

  Serial.println("Ultrasonic Sensor HC-SR04 Test"); 
  Serial.println("with Arduino UN R3");

}

void loop() {

  //Function Calls
  
  pulse()
  emergencyButton()
  distance1=ultra1();
  distance2=ultra2();
  distance3=ultra3();
  
  k=stairs(distance1,distance2,distance3); 

  //Stairs Part - Beep and Vibrate
  
  if(z==3 and k==0){
    
    tone(buzzer,200);
    delay(500);
    digitalWrite(motorPin, HIGH); 
    delay(1000); 
  
    noTone(buzzer);
    delay(500);

    digitalWrite(motorPin, LOW);  
    delay(1000); 
   }

  delay(1000);
  z=0;

}

  //Buzzer On Press - Emergency Button
  
  void emergencyButtonn(){
  int buttonState = digitalRead(buttonPin);
  if (buttonState==LOW)
    tone(buzzer, 200);
  else
    noTone(buzzer);
}

//Function for Measuring Pulse
void pulse(){
  int myBPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {           
  Serial.println("  A HeartBeat Happened ! "); 
  Serial.print("BPM: ");                       
  Serial.println(myBPM);                       
  }
}

//Function for Ultrasonic Sensor 1
int ultra1(){
  long duration,distance;
  digitalWrite(trigPin1,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin1,LOW);
  duration=pulseIn(echoPin1,HIGH);
  distance=duration*0.034/2;
  if(distance<30){
    z+=1;
  }
  Serial.print("Distance1 is: ");
  Serial.print(distance);
  Serial.println("cm");
  return distance;
}

//Function for Ultrasonic Sensor 2
int ultra2(){
  long duration,distance;
  digitalWrite(trigPin2,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin2,LOW);
  duration=pulseIn(echoPin2,HIGH);
  distance=duration*0.034/2;
  if(distance<30){
     z+=1;
  }
  Serial.print("Distance2 is: ");
  Serial.print(distance);
  Serial.println("cm");
  return distance;
}
  
//Function for Ultrasonic Sensor 3
 int ultra3(){
  long duration,distance;
  digitalWrite(trigPin3,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin3,LOW);
  duration=pulseIn(echoPin3,HIGH);
  distance=duration*0.034/2;
  if(distance<30){
    z+=1;
  }
  Serial.print("Distance3 is: ");
  Serial.print(distance);
  Serial.println("cm");
  return distance;
}

//Function for Strais Detection
int stairs(int distance1,int distance2, int distance3){
  if ((distance1-distance2)>7 and  (distance2-distance3)>7 and distance1<30 and distance2<30 and distance3<30){
      tone(buzzer,200);
      delay(200);
      noTone(buzzer);
      delay(200);
      tone(buzzer,200);
      delay(200);
      noTone(buzzer);
      delay(200);
      return 0;
    }
  return 1;
}
