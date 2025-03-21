#include "sensore.h"
#include "sensoretemperatura.h"

#include <iostream>

SensoreTemperatura::SensoreTemperatura(const std::string& tipo, double sogliaMin, double sogliaMax):Sensore(tipo, sogliaMin, sogliaMax), temperatura(0.0){}

SensoreTemperatura* SensoreTemperatura::clone() const{
    return new SensoreTemperatura(*this);
}


SensoreTemperatura::~SensoreTemperatura(){}


std::vector<double> SensoreTemperatura::getDati() const{
    return temperatura;
}

void SensoreTemperatura::setDati(const std::vector<double>& temperatura, const std::vector<std::chrono::system_clock::time_point>& timestamps){
    this->temperatura = temperatura;
    this->timestamps = timestamps;
}


void SensoreTemperatura::controllaAnomalia(){
    for(double valore : temperatura){
        if(valore < sogliaMin){
            std::cout << "Anomalia rilevata: temperatura inferiore alla soglia minima" << std::endl;
            return;
        }
        if(valore > sogliaMax){
            std::cout << "Anomalia rilevata: temperatura superiore alla soglia massima" << std::endl;
            return;
        }
    }
    std::cout << "Temperatura nella norma" << std::endl;
}


std::pair<double, double> SensoreTemperatura::getLimiti() const{
    return { 3.0, 35.0 };
}

std::string SensoreTemperatura::getUnitaMisura() const{
    return "°C";
}