#include <Setup.hpp>
#include "Senzor_Antc.hpp"


void Antc_update(int pin, int filter,int res) {
  const int samples = (filter == 2) ? 10 : (filter == 1 ? 3 : 1);
  int sum = 0;

  for (int i = 0; i < samples; ++i) {
    sum += analogRead(pin);
    delay(3);  
  }

  int Res = sum / samples;

  double Vo = (double)Res * (3.3 / (pow(2, res) - 1));
  double R2 = 10000 * (Vo / (3.3 - Vo));

  float Temp = log(R2);
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 276.8;  // Kalibrace

  String out = "?type=Antc&id=14&temp=" + String(Temp,1);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

bool Antc_init(int pin){
  const int samples = 5;
  int sum = 0;
  for (int i = 0; i < samples; ++i) {
    sum += analogRead(pin);
    delay(5);
  }
  int avg = sum / samples;
  return (avg > 500 && avg < 2500);  
}

void Antc_reset() {

}
