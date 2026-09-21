#include <Servo.h>

const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;
const byte SERVO_PIN = 6;

const int OPEN_DISTANCE_CM = 20;
const int CLOSED_ANGLE = 10;
const int OPEN_ANGLE = 100;
const unsigned long OPEN_TIME_MS = 2500;

Servo lidServo;

long readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (duration == 0) {
    return -1;
  }

  return (long)(duration / 58.0);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lidServo.attach(SERVO_PIN);
  lidServo.write(CLOSED_ANGLE);

  Serial.begin(9600);
}

void loop() {
  long distance = readDistanceCm();

  if (distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  if (distance > 0 && distance <= OPEN_DISTANCE_CM) {
    lidServo.write(OPEN_ANGLE);
    delay(OPEN_TIME_MS);
    lidServo.write(CLOSED_ANGLE);
    delay(300);
  }

  delay(100);
}
