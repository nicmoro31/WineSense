#ifndef SENSORELIVELLO_H
#define SENSORELIVELLO_H

#include<string>
#include<vector>
#include<chrono>

#include "autoclave.h"

class Autoclave;        //forward declaration per risolvere dipendenza circolare

class SensoreLivello: public Sensore{
private: 
    std::vector<double> livello;
    Autoclave* autoclave;
public:
    SensoreLivello(const std::string& tipo, double sogliaMin, double sogliaMax, Autoclave* autoclave);
    ~SensoreLivello(){}
    SensoreLivello* clone() const;
    std::vector<double> getDati() const;
    void setDati(const std::vector<double>&, const std::vector<std::chrono::system_clock::time_point>&) override;
    void controllaAnomalia();
    std::pair<double, double> getLimiti() const;
    std::string getUnitaMisura() const;
    void setAutoclave(Autoclave*);
};

#endif // SENSORELIVELLO_H
