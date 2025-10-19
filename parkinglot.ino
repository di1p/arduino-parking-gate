#include <Servo.h>
#include <LiquidCrystal.h>

// === Pin setup ===
#define TRIG_PIN       6
#define ECHO_PIN       7
#define SERVO_PIN      9
#define RED_LED_PIN    11
#define GREEN_LED_PIN  12
#define BUZZER_PIN     8

// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 3, 4, 5, 10, A0);

Servo myservo;
int pos = 0;
float cm = 0;

// --- Ultrasonic function ---
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH, 30000UL);  // timeout for safety
}

void setup() {
  myservo.attach(SERVO_PIN);
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Parking Gate");
  lcd.setCursor(0,1);
  delay(1500);
  lcd.clear();

  digitalWrite(RED_LED_PIN, HIGH); // Gate starts closed
  myservo.write(120);              // closed position
}

void loop() {
  cm = 0.01723 * readUltrasonicDistance(TRIG_PIN, ECHO_PIN);

  // --- Display distance on LCD ---
  lcd.setCursor(0,0);
  lcd.print("Dist:");
  lcd.setCursor(6,0);
  lcd.print("     ");      // clear old number
  lcd.setCursor(6,0);
  lcd.print(cm);
  lcd.print("cm");

  if (cm < 30) {
    Serial.print(cm); Serial.println(" cm");

    // Gentle confirmation beep before opening
    tone(BUZZER_PIN, 880, 450);
    delay(480);
    noTone(BUZZER_PIN);

    // LCD feedback
    lcd.setCursor(0,1);
    lcd.print("OPENING");

    // OPENING movement
    for (pos = 120; pos >= 0; pos--) {
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, HIGH);
      myservo.write(pos);
      delay(15);
    }

    delay(5000);
    digitalWrite(GREEN_LED_PIN, LOW);

    // CLOSING
    lcd.setCursor(0,1);
    lcd.print("CLOSING");
    for (pos = 0; pos <= 120; pos++) {
      digitalWrite(RED_LED_PIN, HIGH);
      myservo.write(pos);

      // Fast warning beeps
      if (pos % 10 == 0) {
        tone(BUZZER_PIN, 2000, 100);
        delay(80);
      }
      delay(15);
    }
    noTone(BUZZER_PIN);

    lcd.setCursor(0,1);
    lcd.print("CLOSED");
    delay(5000);

  } else {
    // Idle
    myservo.write(120);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    noTone(BUZZER_PIN);

    lcd.setCursor(0,1);
    lcd.print("Idle            ");
    delay(200);
  }
}