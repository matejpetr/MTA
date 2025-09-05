#ifndef SENSOR_HPP
#define SENSOR_HPP
#include "Parser.hpp"   // ← přidat


//Třída Sensor
class Sensor {
  public:
    virtual void update() = 0;
    virtual void reset() {}
    virtual bool init()=0; 
    virtual void config(Param* params, int paramCount) {}  // prázdná výchozí implementace
    virtual ~Sensor() {}
    virtual const char* getType() = 0;
};

#endif