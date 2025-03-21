#include "sensore.h"
#include "sensorelivello.h"

#include<iostream>
SensoreLivello::SensoreLivello(const std::string& tipo, double sogliaMin, double sogliaMax, Autoclave* autoclave):Sensore(tipo, sogliaMin, sogliaMax), livello(0.0), autoclave(autoclave){}

SensoreLivello* SensoreLivello::clone() const{
    return new SensoreLivello(*this);
}



std::vector<double> SensoreLivello::getDati() const{
    return livello;
}


void SensoreLivello::setDati(const std::vector<double>& livello, const std::vector<std::chrono::system_clock::time_point>& timestamps){
    this->livello = livello;
    this->timestamps = timestamps;
}


void SensoreLivello::controllaAnomalia() {
    for(double valore : livello){
        if(valore < sogliaMin){
            std::cout << "Anomalia rilevata: livello inferiore alla soglia minima" << std::endl;
            return;
        }
        if(valore > sogliaMax){
            std::cout << "Anomalia rilevata: livello superiore alla soglia massima" << std::endl;
            return;
        }
    }
    std::cout << "Livello nella norma" << std::endl;
}


std::pair<double, double> SensoreLivello::getLimiti() const{
    return {0.0, autoclave->getCapacita()};
}

std::string SensoreLivello::getUnitaMisura() const{
    return "l";
}

void SensoreLivello::setAutoclave(Autoclave* autoclave){
    this->autoclave = autoclave;
}
