#ifndef SENSOR_HPP
#define SENSOR_HPP



//Třída Sensor
class Sensor {
  public:
    virtual void update() = 0;
    virtual void reset() {}
    virtual bool init()=0; 
    virtual void config(){}
    virtual ~Sensor() {}
    virtual const char* getType() = 0;
};

#endif