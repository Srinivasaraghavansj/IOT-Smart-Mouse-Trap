#include<Servo.h>
const int trigpin1 = D0;
const int echopin1 = D1, echopin2 =D4, echopin3 =D5; 
const int doorpin = D2; 
const int servopin = D3;
long duration1, duration2 = 0, duration3 = 0;
int distance1, distance2 = 0, distance3 = 0;
void runUsensor();
void mousehandler();
Servo door;


void setup() {
pinMode(trigpin1, OUTPUT); 
pinMode(echopin1, INPUT);
pinMode(doorpin, INPUT_PULLUP); 
Serial.begin(115200);
door.detach(); 
}

void loop() {
runUsensor();
Serial.println("Distances: " + (String)distance1 + " , " + (String)distance2 + " , " + (String)distance3);
Serial.println("Duration: " + (String)duration1 + " , " + (String)duration2 + " , " + (String)distance3);

}

void runUsensor() {
digitalWrite(trigpin1, LOW);
delayMicroseconds(2);
digitalWrite(trigpin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigpin1, LOW);
duration1 = pulseIn(echopin1, HIGH);
duration2 = pulseIn(echopin2, HIGH);
duration3 = pulseIn(echopin3, HIGH);
distance1= duration1*0.034/2;
distance2= duration2*0.034/2;
distance3= duration3*0.034/2;
}

void mousehandler() {
  if(distance1 < 18 &&  digitalRead(doorpin) == HIGH)
  door.attach(servopin);
  door.write(90);
  delay(1000);
  door.detach(); 
}

