#include <Servo.h>
#include <AccelStepper.h>

Servo flap;

// Ultrasonic Variables
#define trigPin 4
#define echoPin 5

// Stepper Variables
const int DIR = 6;
const int STEP = 7;
const int steps_per_rev = 1000;

int rain_pin = A3;
int inductive_pin = 10;
int flap_pin = 9;
const int buzzer = 12;
int homing_pin = 13;
int sound = 250;

#define FULLSTEP 4
AccelStepper myStepper(FULLSTEP, 8, 10, 9, 11);

unsigned long flapMoveTime = 0;
bool flapMoving = false;
int flapTargetPosition = 0;

void setup()
{
  Serial.begin(9600);
  initializePins();
  initializeStepper();
  homeStepper();

  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  // Test servo to ensure it works
  Serial.println("Testing servo...");
  testServo();
}

void loop()
{
  long distance = measureDistance();
  int inductive = digitalRead(inductive_pin);
  int rain = analogRead(rain_pin);
  printSensorData(distance, rain, inductive);

  if (inductive == 0)
  { // Metallic waste
    handleWaste("Metallic waste detected.", 1, 180);
  }
  else if (rain <= 800 && inductive == 1)
  { // Wet waste
    handleWaste("Wet waste detected.", 2, 180);
  }
  else if (rain > 920 && inductive == 1 && distance <= 20)
  { // Dry waste
    handleWaste("Dry waste detected.", 3, 180);
  }
  else
  { // No waste
    handleNoWaste();
  }
  // testServo();
  updateFlap();

  delay(1000); // Delay for visibility
}

void initializePins()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(rain_pin, INPUT);
  pinMode(inductive_pin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(homing_pin, INPUT);
  flap.attach(flap_pin);
  Serial.println("Pins initialized.");
}

void initializeStepper()
{
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(100.0);
  myStepper.setSpeed(1000);
  Serial.println("Stepper initialized.");
}

void homeStepper()
{
  int homing = digitalRead(homing_pin);
  for (int i = 0; homing == LOW; i++)
  {
    myStepper.moveTo(i);
    myStepper.runToPosition();
    homing = digitalRead(homing_pin);
  }
  myStepper.moveTo(myStepper.currentPosition() - 2);
  myStepper.runToPosition();
  myStepper.setCurrentPosition(0);
  Serial.println("Stepper homed.");
}

long measureDistance()
{
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  // Debug print for duration and distance
  Serial.print("Duration: ");
  Serial.print(duration);
  Serial.print(" us, Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

void printSensorData(long distance, int rain, bool inductive)
{
  Serial.print("Distance = ");
  Serial.println(distance);
  Serial.print("Moisture = ");
  Serial.println(rain);
  Serial.print("Inductive  = ");
  Serial.println(inductive);
}

void handleWaste(const char *message, int num_beeps, int position)
{
  Serial.println(message);
  beep(num_beeps);

  if (num_beeps == 1)
  {
    // select metal dustbin
  }
  else if (num_beeps == 2)
  {
    // select wet dustbin
  }
  else if (num_beeps == 3)
  {
    // select dry dustbin
  }

  moveAndOpenFlap(position);
}

void handleNoWaste()
{
  Serial.println("No waste detected.");
  moveAndOpenFlap(0);
}

void beep(int num_beeps)
{
  for (int i = 0; i < num_beeps; i++)
  {
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    delay(300);
  }
}

void moveAndOpenFlap(int position)
{
  // Debugging: print the position the servo is supposed to move to
  Serial.print("Moving flap to position: ");
  Serial.println(position);

  flap.write(position);
  flapMoveTime = millis();
  flapMoving = true;
  flapTargetPosition = position;
}

void updateFlap()
{
  if (flapMoving && (millis() - flapMoveTime >= 2000))
  {
    flap.write(0);
    flapMoving = false;
    Serial.println("Returning flap to position 0");
  }
}

void testServo()
{
  flap.write(90); // Move to 90 degrees
  delay(2000);
  flap.write(0); // Move back to 0 degrees
  delay(2000);
}