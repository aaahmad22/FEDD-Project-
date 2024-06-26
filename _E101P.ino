#include <Servo.h>

#define trigPin 8
#define echoPin 9
#define red1 6
#define red2 5
#define yellow1 4
#define yellow2 3
#define green1 2

Servo mainServo;
Servo secondaryServo;

// Global variables
double oldDistance = 0;

// Global constants
const double MAX_TRIGGER_DISTANCE = 10; // Farthest distance servo and green LED will activate at
const double MAX_READ_DISTANCE = 60; // Max distance the servo will calculate (if object is very far away), should be > 5 * MAX_TRIGGER_DISTANCE
const double NEW_AVERAGE_WEIGHT = 0.3; // distance = oldDistance * (1-NEW_AVERAGE_WEIGHT) + newDistance * NEW_AVERAGE_WEIGHT
const int HAND_CLOSE = 180; // Number corresponding to opening hand
const int HAND_OPEN = 0; // Number corresponding to closing hand

double getSmoothedDistance(long newDuration, double oldDistance);
void lightUpLEDs(double distance);
void checkMoveMotors(double distance);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green1, OUTPUT);
  
  // mainServo.attach(10); 
  // secondaryServo.attach(11); // Attach second motor to secondaryHand

  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  Serial.println("Rest");
  //rest
  analogWrite(10, 113);
  delay(1000);
  //to tighten
  Serial.println("tighten");
  analogWrite(10, 116); 
  delay(1000);
  
  Serial.println("open");
  analogWrite(10, 111); 
  delay(1000);

  // for (int i = 0; i < 1000; i+=10) {
  //   analogWrite(10,i);
  //   Serial.print("Speed: "); Serial.println(i);
  //   delay(500);
  // }
}


/*
* Desription:
*   - Closes or opens hand based on distance and max trigger distance
* Arguments:
*   - Distance from ultrasonic
* Return values:
*   - None
*/
void checkMoveMotors(double distance) {
  if (distance < MAX_TRIGGER_DISTANCE) {
    mainServo.write(HAND_OPEN);
    secondaryServo.write(HAND_CLOSE);
  } else {
    mainServo.write(HAND_CLOSE);
    secondaryServo.write(HAND_OPEN);
  }
}


/*
* Desription:
*   - Lights up LEDs based on distance, green if close enough for servo actuation
* Arguments:
*   - Distance from ultrasonic
* Return values:
*   - None
*/
void lightUpLEDs(double distance) {
  if (distance < MAX_TRIGGER_DISTANCE) {
    digitalWrite(red1, HIGH);
    digitalWrite(red2, HIGH);
    digitalWrite(yellow1, HIGH);
    digitalWrite(yellow2, HIGH);
    digitalWrite(green1, HIGH);
  } else if (distance < MAX_TRIGGER_DISTANCE * 2) {
    digitalWrite(red1, HIGH);
    digitalWrite(red2, HIGH);
    digitalWrite(yellow1, HIGH);
    digitalWrite(yellow2, HIGH);
    digitalWrite(green1, LOW);
  } else if (distance < MAX_TRIGGER_DISTANCE * 3) {
    digitalWrite(red1, HIGH);
    digitalWrite(red2, HIGH);
    digitalWrite(yellow1, HIGH);
    digitalWrite(yellow2, LOW);
    digitalWrite(green1, LOW);
  } else if (distance < MAX_TRIGGER_DISTANCE * 4) {
    digitalWrite(red1, HIGH);
    digitalWrite(red2, HIGH);
    digitalWrite(yellow1, LOW);
    digitalWrite(yellow2, LOW);
    digitalWrite(green1, LOW);
  } else if (distance < MAX_TRIGGER_DISTANCE * 5) {
    digitalWrite(red1, HIGH);
    digitalWrite(red2, LOW);
    digitalWrite(yellow1, LOW);
    digitalWrite(yellow2, LOW);
    digitalWrite(green1, LOW);
  } else {
    digitalWrite(red1,LOW);
    digitalWrite(red2,LOW);
    digitalWrite(yellow1,LOW);
    digitalWrite(yellow2,LOW);
    digitalWrite(green1,LOW);
  }
}


/*
* Desription:
*   - Calculates weighted distance
* Arguments:
*   - Most recent duration, old distance calculation
* Return values:
*   - New distance value
*/
double getSmoothedDistance(long newDuration, double oldDistance) {
  double distance;
  //double distance1,distance2;
  double newDistance;

  newDistance = newDuration * 0.0344 / 2; // half time it takes to reach object multiplied by speed of sound in air (0.0344 cm/s)

  // early return if distance calculated is too short
  if (newDistance < 0) {
    return 0;
  }
  // removes outlier
  if (newDistance > MAX_READ_DISTANCE) {
    newDistance = MAX_READ_DISTANCE;
  }

  //distance = (9.0/10.0)*oldDistance + (1.0/10.0)*newDistance; // takes weighted average of new distance value
  distance = oldDistance * (1 - NEW_AVERAGE_WEIGHT) + newDistance * NEW_AVERAGE_WEIGHT;

  return distance;
}