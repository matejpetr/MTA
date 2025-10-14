#include "Senzor_DS18B20.hpp"
#include <cmath>

// Pevná adresa jako v původním kódu
static DeviceAddress DS18B20_ADDR = {0x28, 0xF8, 0xE4, 0x80, 0xE3, 0xE1, 0x3D, 0x00};

void DS18B20::applyConfig() {
  if (!_sensors) return;

  // Rozlišení (validace jako dřív)
  if (_res >= 9 && _res <= 12) {
    _sensors->setResolution(DS18B20_ADDR, _res);
  }

  // Alarmové meze (validace jako dřív)
  if (_LAlarm >= -55 && _LAlarm <= 125) _sensors->setLowAlarmTemp(DS18B20_ADDR,  _LAlarm);
  if (_HAlarm >= -55 && _HAlarm <= 125) _sensors->setHighAlarmTemp(DS18B20_ADDR, _HAlarm);

  // Ulož do scratchpadu
  _sensors->saveScratchPad(DS18B20_ADDR);
}

std::vector<KV> DS18B20::update() {
  std::vector<KV> kv;
  if (!_sensors) return kv;

  // Měření
  _sensors->requestTemperatures();
  float temperature = _sensors->getTempC(DS18B20_ADDR);

  // Přečti hranice z čipu (stejně jako dřív)
  const int lowAlarm  = _sensors->getLowAlarmTemp(DS18B20_ADDR);
  const int highAlarm = _sensors->getHighAlarmTemp(DS18B20_ADDR);

  String alarm = "OK";
  if (temperature <= lowAlarm)       alarm = "LOW";
  else if (temperature >= highAlarm) alarm = "HIGH";

  kv.push_back({"temp",  String(temperature, 1)});
  kv.push_back({"alarm", alarm});
  return kv;
}
