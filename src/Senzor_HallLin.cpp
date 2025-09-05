#include <Setup.hpp>
#include "Senzor_HallLin.hpp"

void HallLin_update(int pin, int res, String unit) {
  analogReadResolution(res);
  int raw = analogRead(pin);

  float result;
  String label;

  if (unit == "ADC") {
    result = raw;
    label = "ADC";
  } else {
    float Vref = 3.3;
    int maxADC = pow(2, res) - 1;
    float voltage = (raw / (float)maxADC) * Vref;

    if (unit == "Induction") {
      float zeroOffset = Vref / 2.0;        // napětí bez pole (1.65 V)
      float sensitivity = 0.0025;           // 2.5 mV/G = 0.0025 V/G
      result = ((voltage - zeroOffset) / sensitivity) * 0.1;  // V → G → mT
      label = "Induction";
    } else {
      result = voltage;
      label = "Voltage";
    }
  }

  String out = "?type=HallLin&id=17&" + label + "=" + String(result, 2);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
}

bool HallLin_init(int pin) {
  int raw1 = analogRead(pin);
  delay(10);
  int raw2 = analogRead(pin);

  float voltage1 = raw1 * (3.3 / 4095.0);
  float voltage2 = raw2 * (3.3 / 4095.0);

  float diff = abs(voltage1 - voltage2);
  return (voltage1 >= 0.9 && voltage1 <= 2.0) && (diff < 0.3);
}

void HallLin_reset() {
  
}
