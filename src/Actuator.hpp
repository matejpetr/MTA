#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP

#include "Parser.hpp"

// Třída Actuator
class Actuator {
  public:
    virtual void config(Param* params = nullptr, int count = 0) {}  
    virtual void reset() {} 
    virtual ~Actuator() {}
};

#endif
