#include <Setup.hpp>
#include <Senzor_Ahall.hpp>


int val;
void Ahall_update(int pin, int lLimit, int hLimit,int res) {

  analogReadResolution(res);
  val = analogRead(pin);
  String polarity;


  if (val > hLimit) {     // porovnávání vstupní hodnoty vůči limitům
    polarity = "SOUTH";
  } else if (val < lLimit) {
    polarity = "NORTH";
  } else {
    polarity = "NO MAGNET";
  }

   String out = "?type=Ahall&id=3&val=" + String(val) + "&polarity=" + polarity;
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}


bool Ahall_init(int pin) {
  //return(analogRead(pin)>1800);
}


void Ahall_reset(){
val = 0;
}