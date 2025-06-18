#ifndef SENSOR_HPP
#define SENSOR_HPP



//Třída Sensor
class Sensor {
  public:
    virtual void update() = 0;
    virtual void reset() {}
    virtual void init() {}
    virtual void config(){}
    virtual ~Sensor() {}
};

#endif