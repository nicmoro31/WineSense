#include "autoclave.h"
#include "database.h"

#include <iostream>
#include <QDebug>

Database* db;

unsigned int Autoclave::nextId = db->caricaIdAutoclave("ultimo_id_autoclave.csv") + 1;

Autoclave::Autoclave() : id(nextId), capacita(0.0), nome("") {}

Autoclave::Autoclave(double capacita, std::string nome): id(nextId++), capacita(capacita), nome(nome) {
}

Autoclave::Autoclave(const Autoclave& other) : QObject(), id(other.id), capacita(other.capacita), nome(other.nome), sensori(other.sensori){
    //copia profonda dei sensori
    for (Sensore* sensore : other.sensori){
        sensori.push_back(sensore->clone());
    }
}

Autoclave::~Autoclave() {
    for (auto sensore : sensori) {
        delete sensore;
    }
}

std::string Autoclave::getNome() const {
    return nome;
}

unsigned int Autoclave::getId() const {
    return id;
}

double Autoclave::getCapacita() const {
    return capacita;
}

std::vector<Sensore*> Autoclave::getSensori() const {
    return sensori;
}

bool Autoclave::aggiungiSensore(Sensore*& sensore, Autoclave* autoclave, const std::string& tipo, double min, double max) {
    if(tipiSensoriPerAutoclave[autoclave].find(tipo) != tipiSensoriPerAutoclave[autoclave].end()){
        emit sensoreNonAggiunto(QString::fromStdString("Sensore di tipo " + tipo + " già presente. Selezionare un tipo diverso."));
        return false;
    }
    if(min >= max){
        emit sensoreNonAggiunto(QString::fromStdString("La soglia minima deve essere inferiore alla soglia massima"));
        return false;
    }

    sensore = Sensore::create(tipo, min, max, autoclave);
    if(sensore!=nullptr){
        std::string errore = checkLimiti(sensore);
        if(!errore.empty()){
            emit sensoreNonAggiunto(QString::fromStdString(errore));
            delete sensore;
            return false;
        }
        sensori.push_back(sensore);
        tipiSensoriPerAutoclave[autoclave].insert(tipo);
        return true;
    }else{
        std::cout << "Tipo sensore non valido" << std::endl;
        return false;
    }
    
}

std::string Autoclave::checkLimiti(Sensore* sensore){
    std::string unitadimisura = sensore->getUnitaMisura();
    double min, max;
    std::tie(min, max) = sensore->getLimiti();
    double sogliaMin = sensore->getSogliaMinima();
    double sogliaMax = sensore->getSogliaMassima();
    if(sogliaMin < min || sogliaMax > max){
        std::ostringstream minStream, maxStream;
        minStream << std::fixed << std::setprecision(1) << min;
        maxStream << std::fixed << std::setprecision(1) << max;
        return "Le soglie per il sensore " + sensore->getTipo() + " devono essere comprese tra " + minStream.str() + " e " + maxStream.str()+ " " + unitadimisura;   
    }
    return "";
}

bool Autoclave::rimuoviSensore(Autoclave* autoclave, const std::string& tipo) {
    for (auto it = autoclave->sensori.begin(); it != autoclave->sensori.end(); ) {
        if ((*it)->getTipo() == tipo){
            autoclave->tipiSensoriPerAutoclave[this].erase(tipo);
            delete *it;
            it = autoclave->sensori.erase(it);
            return true;
        }else
            ++it;
    }
    return false;
}

bool Autoclave::aggiornaSensore(Autoclave* autoclave, const std::string& tipo, double min, double max) {
    if(min >= max){
        emit sensoreNonAggiunto(QString::fromStdString("La soglia minima deve essere inferiore alla soglia massima"));
        return false;
    }
    for (auto sensore : autoclave->sensori){
        if (sensore->getTipo() == tipo){
            double oldMin, oldMax;
            oldMin = sensore->getSogliaMinima();
            oldMax = sensore->getSogliaMassima();
            sensore->impostaSoglia(min, max);
            std::string check = autoclave->checkLimiti(sensore);
            if (!check.empty()) {
                sensore->impostaSoglia(oldMin, oldMax);
                emit sensoreNonAggiunto(QString::fromStdString(check));
                return false;
            }
            return true;
        }
    }
    return false;
}

void Autoclave::impostaCapacita(float capacita) { 
    this->capacita = capacita;
}

void Autoclave::caricaDati(unsigned int id, std::string nome, double capacita) {
    this->id = id;
    this->nome = nome;
    this->capacita = capacita;
}