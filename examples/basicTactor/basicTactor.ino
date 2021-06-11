#include <Servo.h>
#include <Tactor.h>

const int ledPin = 13;
const int servoPin = 6;

int servoFrameMillis = 20; //minimum time between servo updates
float startPos = 0;

Servo myServo;
Tactor myTactor;

void setup() {

  Serial.begin(9600);

  //first get the servo ready
  myServo.attach(servoPin);

  //initialize tactor
  myTactor.begin(myServo, servoFrameMillis, startPos, ledPin);
  
  //wait 1 sec after initialization
  delay(1000);
  
  //activate tactor
  myTactor.activate();

  //Telling tactor to move 45 degrees over 2 seconds
  myTactor.moveTo(45, 2000);

  //start the tactor
  myTactor.start();
}

void loop() {
  myTactor.update();

  if (myTactor.hasArrived()) {
    // Telling tactor to move 160 degrees in 4 seconds
    myTactor.moveTo(160, 4000);
  }
}