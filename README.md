# Tactor

## Install Guide
* Download as ```.zip``` from GitHub
* Unzip and place unzipped file in your ```Arduino Libraries``` folder

## API
### Initialize Instance
```Arduino
#include <Servo.h>
#include <Tactor.h>

Servo myServo;
Tactor myTactor;
```

in ```setup()```
```Arduino
myServo.attach(pin);

myTactor.begin(Servo myServo, int frameTime, int initPos, int indicatorPin);
```

```myServo``` : Instance of Servo
* ```pin``` : pin to which servo motor is connected to

```frameTime``` : minimum update time between servo moves

```initPos``` : Initial position to start servo from

```indicatorPin``` : output pin for tactor activation status

### Activate/ Deactivate Tactor
```Arduino
myTactor.activate(); //activate the tactor

myTactor.deactivate(); //deactivate the tactor
```

### Position Control
```Arduino
myTactor.moveTo(int position, int duration);

myTactor.getCurrPos(); //returns current position (int)
```
```position``` : in degrees where tactor needs to move.

```duration``` : in what time tactor need to finish movement.

### Start/ Stop Tactor
```Arduino
myTactor.start(); //start the tactor

myTactor.stop(); //stop the tactor
```

### Direction Control
```Arduino
myTactor.setDirection(bool direction); //activate the tactor

myTactor.getDirection(); //returns direction (bool)
```

### Checkpoints
```Arduino
myTactor.hasArrived(); //has tactor arrived at its commaned move point?

myTactor.isRunning(); //is the tactor running (start/stop flag)
```

### Call this periodically in ```loop()```
```Arduino
myTactor.update()
```