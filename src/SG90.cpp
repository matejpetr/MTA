#include "SG90.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>


extern Servo myServo;
void control(Servo &s, int start, int end, int speedMs);


static int lastAngle = 0;  // začíná ve středu
void SG90_config(int pin, int angle, int speed){
  angle = constrain(angle, 0, 180);
  int speedMs = map(speed, 0, 100, 100, 0);

  myServo.attach(pin);

  if (angle != lastAngle) {
    control(myServo, lastAngle, angle, speedMs);
    lastAngle = angle;
  }

}


void SG90_reset(int pin){
  myServo.attach(pin); 
  control(myServo, lastAngle, 0, 0);
  lastAngle = 0;

}



void control(Servo &s, int start, int end, int speedMs) {
  int step = (start < end) ? 1 : -1;
  for (int pos = start; pos != end; pos += step) {
    s.write(pos);
    delay(speedMs);  
  }
  s.write(end); //pro jistotu znovu zápis stejného úhlu
}