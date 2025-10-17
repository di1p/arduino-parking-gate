#include <Servo.h>

Servo myservo;   

int pos = 0;

int cm = 0;
#define GREEN_LED_PIN 12
#define RED_LED_PIN 11

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}



void setup() {
  myservo.attach(9); 
  Serial.begin(9600);
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(RED_LED_PIN,OUTPUT);
}

void loop() {
  //digitalWrite(LED_PIN, HIGH);

  cm = 0.01723 * readUltrasonicDistance(6, 7);

  if(cm<30){
  Serial.print(cm);
  Serial.println("cm");

   //OPENING
  for (pos = 120; pos >= 0; pos -= 1) { 
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    myservo.write(pos);
    delay(15);                                     
  }
  delay(5000);
  digitalWrite(GREEN_LED_PIN, LOW);
  //CLOSING
  for (pos = 0; pos <= 120; pos += 1) { 
    digitalWrite(RED_LED_PIN, HIGH);
    myservo.write(pos);
    delay(15);                                     
  }
  delay(5000);
  
  }                          

else{
    myservo.write(120);
    digitalWrite(RED_LED_PIN, HIGH);
  }
}