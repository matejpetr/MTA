#ifndef SENSOR_HPP
#define SENSOR_HPP
#include <vector>
#include <Arduino.h>

struct Param {
  String key;
  String value;
};

struct KV { String k; String v; };

//Třída Sensor
class Sensor {
  public:
    virtual std::vector<KV>  update() = 0;
    virtual void reset() {}
    virtual bool init()=0; 
    virtual void config(Param* params, int paramCount) {}  // prázdná výchozí implementace
    virtual ~Sensor() {}
    virtual const char* getType() = 0;
};

#endif