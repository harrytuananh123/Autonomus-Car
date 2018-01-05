#include <SoftwareServo.h>
#include <TimerOne.h>
#include <Wire.h>
#include <LIDARLite.h>
 
#define DIR_PIN 8
#define STEP_PIN 6
 
LIDARLite myLidarLite;
SoftwareServo servo;
 
volatile int stepCount = 0;
volatile int roundCount = 0;
 
int height = 120;
 
void setup() {
    Serial.begin(115200);
 
    myLidarLite.begin(0, true);
    myLidarLite.configure(0);
 
    servo.attach(11);
    servo.write(height);
 
    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(7, OUTPUT);// Stepper driver Enable
    digitalWrite(7, LOW);
    digitalWrite(DIR_PIN, HIGH);
 
    Timer1.initialize();
    Timer1.attachInterrupt(tick, 4000000.0f / (400 * 1));
}
 
void loop() {
    receive(true);
 
    for(int i = 0; i < 99; i++)
        receive(false);
}
 
void receive(bool bias)
{
    float angle = ((float)stepCount / 400.0f) * 360.0f;
    Serial.print(height);
    Serial.print(",");
    Serial.print(angle);
    Serial.print(",");
    Serial.println(myLidarLite.distance(bias));
 
    if(roundCount >= 2)
    {
        height++;
 
        if(height >= 150)
            height = 120;
 
        servo.write(height);
        roundCount = 0;
    }
 
    servo.refresh();
}
 
void tick()
{
    digitalWrite(STEP_PIN, HIGH);
    //delay(1);
    digitalWrite(STEP_PIN, LOW);
 
    stepCount++;
    if(stepCount >= 400)
    {
        stepCount = 0;
        roundCount++;
    }
}
