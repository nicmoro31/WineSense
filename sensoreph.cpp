#include "sensore.h"
#include "sensoreph.h"

#include <iostream>

SensorePH::SensorePH(const std::string& tipo, double sogliaMin, double sogliaMax):Sensore(tipo, sogliaMin, sogliaMax), ph(0.0){}

SensorePH* SensorePH::clone() const{
    return new SensorePH(*this);
}

SensorePH::~SensorePH(){}

std::vector<double> SensorePH::getDati() const{
    return ph;
}

void SensorePH::setDati(const std::vector<double>& ph, const std::vector<std::chrono::system_clock::time_point>& timestamps){
    this->ph = ph;
    this->timestamps = timestamps;
}


void SensorePH::controllaAnomalia(){
    for(double valore : ph){
        if(valore < sogliaMin){
            std::cout << "Anomalia rilevata: PH inferiore alla soglia minima" << std::endl;
            return;
        }
        if(valore > sogliaMax){
            std::cout << "Anomalia rilevata: PH superiore alla soglia massima" << std::endl;
            return;
        }
    }
    std::cout << "PH nella norma" << std::endl;
}

void SensorePH::calibra(double valoreCalibrato){
    ph.push_back(valoreCalibrato);
    std::cout << "Sensore PH calibrato con successo. Nuovo valore calibrato: " << valoreCalibrato << std::endl;
}

std::pair<double, double> SensorePH::getLimiti() const{
    return {1.0, 14.0};
}

std::string SensorePH::getUnitaMisura() const{
    return "";
}