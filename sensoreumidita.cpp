#include "sensore.h"
#include "sensoreumidita.h"

#include <iostream>

SensoreUmidita::SensoreUmidita(const std::string& tipo, double sogliaMin, double sogliaMax):Sensore(tipo, sogliaMin, sogliaMax), umidita(0.0){}

SensoreUmidita* SensoreUmidita::clone() const{
    return new SensoreUmidita(*this);
}

SensoreUmidita::~SensoreUmidita(){}

std::vector<double> SensoreUmidita::getDati() const{
    return umidita;
}

void SensoreUmidita::setDati(const std::vector<double>& umidita, const std::vector<std::chrono::system_clock::time_point>& timestamps){
    this->umidita = umidita;
    this->timestamps = timestamps;
}


void SensoreUmidita::controllaAnomalia(){
    for(double valore : umidita){
        if(valore < sogliaMin){
            std::cout << "Anomalia rilevata: umidita' inferiore alla soglia minima" << std::endl;
            return;
        }
        if(valore > sogliaMax){
            std::cout << "Anomalia rilevata: umidita' superiore alla soglia massima" << std::endl;
            return;
        }
    }
    std::cout << "Umidita' nella norma" << std::endl;
}


std::pair<double, double> SensoreUmidita::getLimiti() const{
    return {40.0, 85.0};
}

std::string SensoreUmidita::getUnitaMisura() const{
    return "%";
}
