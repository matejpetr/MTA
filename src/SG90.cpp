#include "SG90.hpp"
#include <Arduino.h>
#include <ESP32Servo.h>


extern Servo myServo;
void control(Servo &s, int start, int end, int speedMs);

void SG90_config(int pin,int angle, int speed){
 myServo.attach(pin);
 int pos=myServo.read();

 int delay = map(speed, 0, 100, 100, 0);
 control(myServo, pos, angle, delay);
}


void SG90_reset(){
 myServo.write(0);
 delay(500);

}



void control(Servo &s, int start, int end, int speedMs) {
  int step = (start < end) ? 1 : -1;
  for (int pos = start; pos != end; pos += step) {
    s.write(pos);
    delay(speedMs);  
  }
  s.write(end); //pro jistotu znovu zápis stejného úhlu
}