#include "sensore.h"
#include "sensorepressione.h"

#include <iostream>

SensorePressione::SensorePressione(const std::string& tipo, double sogliaMin, double sogliaMax):Sensore(tipo, sogliaMin, sogliaMax), pressione(0.0){}

SensorePressione* SensorePressione::clone() const{
    return new SensorePressione(*this);
}

SensorePressione::~SensorePressione(){}

std::vector<double> SensorePressione::getDati() const{
    return pressione;
}

void SensorePressione::setDati(const std::vector<double>& pressione, const std::vector<std::chrono::system_clock::time_point>& timestamps){
    this->pressione = pressione;
    this->timestamps = timestamps;
}


void SensorePressione::controllaAnomalia(){
    for(double valore : pressione){
        if(valore < sogliaMin){
            std::cout << "Anomalia rilevata: pressione inferiore alla soglia minima" << std::endl;
            return;
        }
        if(valore > sogliaMax){
            std::cout << "Anomalia rilevata: pressione superiore alla soglia massima" << std::endl;
            return;
        }
    }
    std::cout << "Pressione nella norma" << std::endl;
}


std::pair<double, double> SensorePressione::getLimiti() const{
    return {0.5, 4.5};
}

std::string SensorePressione::getUnitaMisura() const{
    return "bar";
}