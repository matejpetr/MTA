#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP
#include "sensor.hpp"

// Třída Actuator
class Actuator {
  public:
    virtual void config(Param* params = nullptr, int count = 0) {}  
    virtual void reset() {} 
    virtual ~Actuator() {}
    virtual void init() {}

    virtual void attach(const std::vector<int>& pins) { (void)pins; }
    virtual void detach() {}                        // default no-op
};

#endif
