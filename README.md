# Arduino Smart Dustbin Project

Build an automatic dustbin lid that opens when a hand comes close. This project uses an Arduino Uno, an HC-SR04 ultrasonic sensor, and a servo motor.

The sensor measures the distance to an object. When the measured distance falls below the chosen threshold, the Arduino moves the servo to open the lid. After a short delay, the lid closes again.

## What you will learn

- How an HC-SR04 ultrasonic sensor measures distance
- How to read the sensor with an Arduino Uno
- How to control a servo motor
- How to combine sensor input with motor output
- How to tune distance thresholds and delays
- How to troubleshoot common wiring and power problems

## Components

| Component | Quantity | Purpose |
|---|---:|---|
| Arduino Uno or compatible board | 1 | Main controller |
| HC-SR04 ultrasonic sensor | 1 | Detects a nearby hand or object |
| SG90 micro servo | 1 | Opens and closes the lid |
| Jumper wires | As needed | Connections |
| Breadboard | 1 | Optional prototyping board |
| USB cable | 1 | Programming and power |
| Lightweight dustbin lid | 1 | Mechanical part moved by the servo |

A separate 5 V supply can be useful if the servo causes the Arduino to reset. Connect the external supply ground to Arduino GND so the control signal has a common reference.

## How it works

The HC-SR04 has four pins: VCC, TRIG, ECHO, and GND.

1. The Arduino sends a short trigger pulse to the TRIG pin.
2. The sensor sends an ultrasonic burst.
3. The ECHO pin stays HIGH for the time taken by the sound to travel to the object and back.
4. The Arduino converts that time into distance.
5. If the distance is below the threshold, the servo moves to the open position.
6. After the configured delay, the servo returns to the closed position.

The approximate distance calculation is:

`distance_cm = echo_time_us / 58.0`

The value is based on the speed of sound and the round trip of the ultrasonic pulse.

## Wiring

### HC-SR04 to Arduino Uno

| HC-SR04 | Arduino Uno |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | D9 |
| ECHO | D10 |

### Servo to Arduino Uno

| Servo wire/function | Arduino Uno |
|---|---|
| Signal | D6 |
| VCC | 5V or suitable external 5 V supply |
| GND | GND |

Servo wire colors vary by manufacturer, so check the label or datasheet before connecting power.

> **Power note:** A servo can draw a short burst of current when it starts or when the lid is under load. If the Arduino resets or the servo jitters, power the servo from a suitable 5 V supply and connect its ground to Arduino GND. Do not exceed the servo's voltage rating.

## Arduino sketch

The example below keeps the code simple so it is easy to modify.

```cpp
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

  // No echo within the timeout.
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
```

## Upload and test

1. Install the Arduino IDE.
2. Connect the Arduino Uno with USB.
3. Select the correct board and COM/serial port.
4. Connect the HC-SR04 and servo according to the wiring tables.
5. Upload the sketch.
6. Open **Serial Monitor** at 9600 baud.
7. Move your hand toward the sensor and watch the distance readings.
8. If the hand is within 20 cm, the lid should open.
9. Adjust `OPEN_DISTANCE_CM`, `OPEN_ANGLE`, `CLOSED_ANGLE`, and `OPEN_TIME_MS` for your mechanism.

Test the electronics with the servo disconnected from the lid mechanism first. This makes it easier to verify the direction and travel before the servo has to move the physical lid.

## Mechanical setup

The electronics are only half of the project. A practical lid mechanism should move freely and stay within the servo's usable range.

- Use a lightweight lid for a small SG90 servo.
- Mount the servo firmly so its body does not rotate with the horn.
- Keep the linkage short and avoid forcing the servo against a hard stop.
- Start with small servo angles and increase them gradually.
- Place the ultrasonic sensor where the lid or bin wall does not block its field of view.
- Avoid positioning the sensor so the open lid is directly in front of it during normal operation.

## Tuning the sensor

`OPEN_DISTANCE_CM` controls how close an object must be before the lid opens. A value of 20 cm is a starting point, not a universal setting.

If the lid opens too easily, reduce the threshold. If it requires your hand to be very close, increase it.

Ultrasonic readings can change because of the target's shape, angle, surface, and surrounding objects. Test the sensor with the actual bin and lid installed.

## Common problems

### Servo does not move

- Check that the signal wire is connected to D6.
- Confirm the servo has the correct supply voltage.
- Make sure the servo ground and Arduino ground are connected.
- Test the servo separately with a simple Servo library example.

### Arduino resets when the lid moves

The servo may be drawing more current than the Arduino's 5 V supply can provide reliably. Use a suitable separate 5 V supply for the servo and connect the grounds together.

### Distance always shows `-1`

The code uses a timeout when no echo is received. Check the TRIG and ECHO connections, sensor power, and the sensor's orientation.

### Distance readings are unstable

Keep the sensor away from vibrating surfaces and test with a reasonably flat target. Make sure the sensor is mounted securely.

### Lid opens but does not close correctly

Adjust `OPEN_ANGLE`, `CLOSED_ANGLE`, and the linkage. Do not force the servo against the lid's mechanical limits.

### Lid repeatedly opens and closes

Increase the delay after closing, reduce the sensor threshold, or change the control logic so a new hand detection is required before another opening cycle.

## Possible improvements

Once the basic version works, you can improve it without changing the core idea:

- Add an LED to show when the lid is open.
- Add a buzzer for an audible indication.
- Use a better power supply and separate servo power wiring.
- Add a second distance sensor to estimate the bin fill level.
- Replace the fixed delay with a non-blocking `millis()`-based state machine.
- Add a manual open button.
- Add a rechargeable power system for portable use.

For a detailed companion explanation of the same project, see the Vayuyaan guide: https://vayuyaan.com/blog/smart-dustbin-using-arduino-and-ultrasonic-sensor/

## Project structure

```text
arduino-smart-dustbin-project/
├── README.md
└── smart_dustbin/
    └── smart_dustbin.ino
```

## Safety and reliability notes

This project is intended for a small demonstration dustbin and lightweight lid. Do not use an undersized servo to lift a heavy lid, and keep fingers clear of moving linkages during testing.

For a permanent installation, protect the electronics from moisture and waste, provide strain relief for wires, and use a regulated supply suitable for the actual servo load.

## License

This project is released under the MIT License. See `LICENSE` for details.
