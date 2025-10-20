#pragma once
#include <Arduino.h>
#include "actuator.hpp"

// forward deklarace, aby se CheapStepper netahal do .hpp
class CheapStepper;

class Stepper : public Actuator {
public:
  Stepper(int pin1, int pin2, int pin3, int pin4, int angle, bool dir, int rpm)
    : _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4),
      _angle(angle), _dir(dir), _rpm(rpm) {}

  // CONNECT: očekává pins=pin1,pin2,pin3,pin4 (může přijít i méně; použijí se dostupné)
  void attach(const std::vector<int>& pins) override;

  // DISCONNECT: vypne cívky a uvolní piny
  void detach() override;

  // init() bez návratové hodnoty (ladí s Actuator)
  void init() override;

  // CONFIG: mění jen logické parametry (piny NEMĚNÍ) a případně provede krok
  void config(Param* params = nullptr, int count = 0) override;

  // RESET: návrat do „0“ a vypnutí cívek
  void reset() override;


private:
  // HW
  int _pin1 = -1, _pin2 = -1, _pin3 = -1, _pin4 = -1;
  CheapStepper* _stp = nullptr;

  // Stav/parametry
  int  _angle = 0;     // DEG – kolik pohnout při CONFIG
  bool _dir   = true;  // true=fwd, false=back
  int  _rpm   = 16;    // rychlost
  int  _currentPos = 0; // akumulace v ° (pro reset)

  void ensureDriver_();       // vytvoří _stp, nastaví piny
  void releasePins_();        // piny do INPUT
  void coilsOff_();           // vypnout vinutí (šetří proud)
};
