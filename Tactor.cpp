#include "Tactor.h"
#include <math.h>

#define DEBUG 1

#if (DEBUG > 0)
#define debug_update(){                                 \
        Serial.print("update: currPos:");               \
        Serial.print(currPos);                          \
        Serial.print(" startPos:");                     \
        Serial.print(startPos);                         \
        Serial.print(" deltaPos:");                     \
        Serial.print(deltaPos);                         \
        Serial.print(" startMillis:");                  \
        Serial.print(startMillis);                      \
        Serial.print(" arrived:");                      \
        Serial.println(((arrived) ? "true" : "false")); \
    }
#define debug_reset() {                                 \
        Serial.print("reset: currPos:");                \
        Serial.print(currPos);                          \
        Serial.print(" deltaPos:");                     \
        Serial.print(deltaPos);                         \
        Serial.print(" durMillis:");                    \
        Serial.print(durMillis);                        \
        Serial.print(" startMillis:");                  \
        Serial.print(startMillis);                      \
        Serial.print(" active:");                       \
        Serial.println(((active) ? "true" : "false"));  \
    }
#else
#define debug_update() {}
#define debug_reset() {}
#endif

// default movement function
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be in frames or seconds/milliseconds
float basicMoveFunction(float t, float b, float c, float d)
{
    return b+(c/d*t);
}

void Tactor::begin(Servo &s, int frameTime, float initPos, int indicator)
{
    servo = &s;
    frameMillis = frameTime;
    startPos = initPos;

    direction = false;
    arrived = true;

    active = false;

    servoFunc = basicMoveFunction;

    statusPin = indicator;
    pinMode(statusPin, OUTPUT);
    digitalWrite(statusPin, LOW);
    
    reset();
}

void Tactor::activate()
{
    active = true;
    digitalWrite(statusPin, HIGH);
}

void Tactor::deactivate()
{
    active = false;
    digitalWrite(statusPin, LOW);
}

void Tactor::reset()
{
    if (servo)
    {
        servo->write(startPos);
        currPos = servo->read();
    }

    startPos = currPos; // get everyone in sync;
    deltaPos = 0;

    startMillis = millis();

    debug_reset();
}

void Tactor::moveTo(int pos, int dur)
{
    if (active)
    {
        startPos = currPos;
        deltaPos = pos - startPos;
        durMillis = dur;
        startMillis = millis();
        arrived = false;
    }
}

void Tactor::update()
{
    if (active)
    {
        unsigned long currentMillis = millis();

        if (((currentMillis - lastMillis) < frameMillis) || !running)
        { //time yet?
            return;
        }

        lastMillis = currentMillis;

        if (currentMillis > (startMillis + durMillis))
        {
            currPos = startPos + deltaPos;
            arrived = true;
        }
        else
        {
            currPos = servoFunc(currentMillis - startMillis, startPos, deltaPos, durMillis);
            arrived = (currPos == startPos + deltaPos) ? true : false;
        }

        debug_update();

        float p = (direction) ? 180.0 - currPos : currPos;
        if (servo)
            if (useMicros)
                servo->writeMicroseconds(angleToMicros(p));
            else
                servo->write(p);
    }
}

void Tactor::setMinMaxMicroseconds(int mi, int ma)
{
    min = mi;
    max = ma;
    useMicros = true;
}

// from Servo.cpp
#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4) // min value in uS
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4) // max value in uS

#define in_min 0
#define in_max 180
#define out_min SERVO_MIN()
#define out_max SERVO_MAX()
int Tactor::angleToMicros(float angle)
{
    //                x   in_min,in_max, out_min, out_max
    //int value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
    return (angle - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Tactor::useMicroseconds(boolean t)
{
    useMicros = t;
}

boolean Tactor::usingMicroseconds()
{
    return useMicros;
}

void Tactor::setDirection(boolean dir)
{
    direction = dir;
}

boolean Tactor::getDirection()
{
    return direction;
}

void Tactor::start()
{
    if (!running)
    {
        startMillis += millis() - lastMillis;
        running = true;
    }
}

void Tactor::stop()
{
    running = false;
}

boolean Tactor::isRunning()
{
    return running;
}

boolean Tactor::hasArrived()
{
    return arrived;
}

float Tactor::getCurrPos()
{
    return currPos;
}