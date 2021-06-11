#ifndef Tactor_h
#define Tactor_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Servo.h>

typedef struct _servoMove
{
  int pos;
  int dur;
} ServoMove;

// t:current time, b:beginning value, c:change in value, d: duration
// t and d can be in frames or seconds/milliseconds
typedef float (*ServoFunc)(float t, float b, float c, float d);

class Tactor
{
private:
  Servo *servo;
  int frameMillis;

  float startPos;
  float currPos;
  float deltaPos;
  float initPos;

  unsigned long startMillis;
  unsigned long durMillis;

  unsigned long lastMillis;

  ServoFunc servoFunc;

  boolean running;
  boolean arrived;
  boolean direction;

  boolean useMicros;

  int statusPin;
  boolean active;

  int angleToMicros(float angle);

  uint8_t min;
  uint8_t max;

public:
  void begin(Servo &s, int frameTime, float initPos, int indicator);
  void reset();

  void activate();
  void deactivate();

  void moveTo(int pos, int durMillis);

  float getCurrPos();

  void update();

  void setServoFunc(ServoFunc);

  boolean hasArrived();
  boolean isRunning();

  void start();
  void stop();

  void setDirection(boolean dir);
  boolean getDirection();

  void setMinMaxMicroseconds(int mi, int ma);

  void useMicroseconds(boolean t);
  boolean usingMicroseconds();
};

#endif
