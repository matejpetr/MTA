#include "Stepper.hpp"
#include <CheapStepper.h>

// vyhnout se 6..11 (flash), 34..39 (input-only), 12 (strap)
static inline bool validOutPin_(int p) {
  if (p < 0) return false;
  if (p >= 6 && p <= 11) return false;
  if (p == 12) return false;
  if (p >= 34 && p <= 39) return false;
  return (p <= 33);
}

void Stepper::releasePins_() {
  if (_pin1 >= 0) pinMode(_pin1, INPUT);
  if (_pin2 >= 0) pinMode(_pin2, INPUT);
  if (_pin3 >= 0) pinMode(_pin3, INPUT);
  if (_pin4 >= 0) pinMode(_pin4, INPUT);
}


void Stepper::ensureDriver_() {
  if (_stp) return;
  if (!validOutPin_(_pin1) || !validOutPin_(_pin2) ||
      !validOutPin_(_pin3) || !validOutPin_(_pin4)) {
    return; // nemáme validní piny → neinstancuj driver
  }

  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  pinMode(_pin3, OUTPUT);
  pinMode(_pin4, OUTPUT);

  _stp = new CheapStepper(_pin1, _pin2, _pin3, _pin4);
  _stp->setRpm(_rpm);
  _stp->stop(); // drž cívky vypnuté, dokud nepřijde příkaz
}

void Stepper::attach(const std::vector<int>& pins) {
  // vypnout a uvolnit starý driver
  if (_stp) { _stp->stop(); delete _stp; _stp = nullptr; }

  if (pins.size() > 0) _pin1 = pins[0];
  if (pins.size() > 1) _pin2 = pins[1];
  if (pins.size() > 2) _pin3 = pins[2];
  if (pins.size() > 3) _pin4 = pins[3];

  ensureDriver_();
}

void Stepper::detach() {
  _stp->stop();
  if (_stp) { delete _stp; _stp = nullptr; }
  releasePins_();
}

void Stepper::init() {
  // opakovaná ochrana – když attach nedodal všechny piny, nic se nestane
  if (!_stp) ensureDriver_();
}

void Stepper::config(Param* params, int count) {
  // Piny v CONFIGU NEMĚNÍME – jen logické parametry
  for (int i = 0; i < count; ++i) {
    if      (params[i].key == "angle") _angle = params[i].value.toInt();
    else if (params[i].key == "dir")   _dir   = (params[i].value == "true");
    else if (params[i].key == "rpm")   _rpm   = params[i].value.toInt();
  }

  ensureDriver_();
  if (_stp) _stp->setRpm(_rpm);

  int angleAbs = abs(_angle);
  if (_stp && angleAbs > 0) {
    _currentPos += _dir ? angleAbs : -angleAbs;
    _stp->moveDegrees(_dir, angleAbs);
    _stp->stop();
  }
}

void Stepper::reset() {
  if (_currentPos != 0) {
    ensureDriver_();
    if (_stp) {
      int backAngle = abs(_currentPos);
      bool backDir  = (_currentPos > 0) ? false : true;
      int oldRpm = _rpm;
      _stp->setRpm(16);
      _stp->moveDegrees(backDir, backAngle);
      _stp->setRpm(oldRpm);
      _stp->stop();
    }
    _currentPos = 0;
  }
}
