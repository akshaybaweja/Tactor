#include <Servo.h>
#include <Tactor.h>

const int ledPin = 13;
const int servoPin = 7;

int servoFrameMillis = 20; //minimum time between servo updates

Servo myServo;
Tactor myTactor;

void setup(){
    
    //first get the servo ready
    myServo.attach(servoPin);
    
    //initialize tactor
    myTactor.begin(myServo, servoFrameMillis, ledPin);

    //Telling tactor to move 20 degrees over 2 seconds
    myTactor.moveTo(20, 2000);
}

void loop(){
    myTactor.update();

    if(myTactor.hasArrived()){
        // Telling tactor to move 160 degrees in 4 seconds
        myTactor.moveTo(160, 4000);
    }
}
