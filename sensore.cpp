#include "sensore.h"
#include "sensorelivello.h"
#include "sensorepressione.h"
#include "sensoretemperatura.h"
#include "sensoreumidita.h"
#include "sensoreph.h"

unsigned int Sensore::id = 0;

Sensore::Sensore(){
    id++;
    tipo = "";
    sogliaMin = 0.0;
    sogliaMax = 0.0;
}

Sensore::Sensore(std::string tipo, double sogliaMin, double sogliaMax){
    id++;
    this->tipo = tipo;
    this->sogliaMin = sogliaMin;
    this->sogliaMax = sogliaMax;
}

Sensore::~Sensore() {
    id--;
}

Sensore* Sensore::create(std::string tipo, double sogliaMin, double sogliaMax, Autoclave* autoclave){
    if(tipo=="Temperatura"){
        return new SensoreTemperatura(tipo, sogliaMin, sogliaMax);
    }else if (tipo=="Pressione"){
        return new SensorePressione(tipo, sogliaMin, sogliaMax);
    }else if (tipo=="Umidità"){
        return new SensoreUmidita(tipo, sogliaMin, sogliaMax);
    }else if (tipo=="PH"){
        return new SensorePH(tipo, sogliaMin, sogliaMax);
    }else if (tipo=="Livello"){
        return new SensoreLivello(tipo, sogliaMin, sogliaMax, autoclave);
    }else{
        return nullptr;
    }
}

bool Sensore::operator==(const Sensore& other) const {
    return id == other.id && tipo == other.tipo && sogliaMin == other.sogliaMin && sogliaMax == other.sogliaMax;
}

bool Sensore::operator!=(const Sensore& other) const {
    return !(*this == other);
}

int Sensore::getId(){
    return id;
}

std::string Sensore::getTipo() const{
    return tipo;
}

double Sensore::getSogliaMinima() const{
    return sogliaMin;
}

double Sensore::getSogliaMassima() const{
    return sogliaMax;
}

void Sensore::impostaSoglia(double nuovaSogliaMin, double nuovaSogliaMax){
    sogliaMin = nuovaSogliaMin;
    sogliaMax = nuovaSogliaMax;
}

std::vector<std::chrono::system_clock::time_point> Sensore::getTimestamps() const{
    return timestamps;
}

// Path: sensore.cpp
