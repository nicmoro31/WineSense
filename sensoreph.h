#ifndef SENSOREPH_H
#define SENSOREPH_H

#include <string>
#include <vector>

class SensorePH :public Sensore{
private:
    std::vector<double> ph;
public:
    SensorePH(const std::string& tipo, double sogliaMin, double sogliaMax);
    ~SensorePH();
    SensorePH* clone() const;
    std::vector<double> getDati() const;
    void setDati(const std::vector<double>&, const std::vector<std::chrono::system_clock::time_point>&) override;
    void calibra(double);
    void controllaAnomalia();
    std::pair<double, double> getLimiti() const;
    std::string getUnitaMisura() const;
};

#endif // SENSOREPH_H
