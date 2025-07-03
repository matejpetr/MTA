#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP



//Třída Actuator
class Actuator {
  public:
    virtual void config(){}  
    virtual void reset() {} 
    virtual ~Actuator() {}
};

#endif