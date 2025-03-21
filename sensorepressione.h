#ifndef SENSOREPRESSIONE_H
#define SENSOREPRESSIONE_H

#include <string>
#include <vector>

class SensorePressione: public Sensore{
private:
    std::vector<double> pressione;
public:
    SensorePressione(const std::string& tipo, double sogliaMin, double sogliaMax);
    ~SensorePressione();
    SensorePressione* clone() const;
    std::vector<double> getDati() const;
    void setDati(const std::vector<double>&, const std::vector<std::chrono::system_clock::time_point>&) override;
    void controllaAnomalia();
    std::pair<double, double> getLimiti() const;
    std::string getUnitaMisura() const;
};

#endif // SENSOREPRESSIONE_H
