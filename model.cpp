#include "model.h"

#include <iostream>
#include <QDebug>

Model::Model(){
    autoclave = nullptr;
    database = new Database();
}

Model::Model(double capacitaAutoclave, std::string nomeAutoclave){
    autoclave = new Autoclave(capacitaAutoclave, nomeAutoclave);
}

Model::~Model(){
    delete autoclave;
    delete database;
}

Model& Model::operator=(const Model& model){
    if (this != &model) {
        delete autoclave;
        autoclave = new Autoclave(*model.autoclave);
    }

    return *this;
}

void Model::addAutoclave(double capacita, std::string nome){
    autoclave = new Autoclave(capacita, nome);
    autoclavi.push_back(autoclave);
    std::cout << "Autoclave aggiunta" << std::endl;

    //salva l'id dell'autoclave appena creata
    try{
	qDebug() << "dentro try addautoclave";
        database->salvaIdAutoclave(autoclave->getId(), "ultimo_id_autoclave.csv");
    }catch(FileopenException& e){
	qDebug() << "dentro catch addautoclave";
        qCritical() << e.what();
    }
    qDebug() << "fuori try-catch";
}

void Model::setAutoclave(Autoclave* autoclave){ 
    this->autoclave = autoclave;
}

std::vector<Sensore*> Model::getSensori() const{
    if(autoclave)
        return autoclave->getSensori();
    else
        return std::vector<Sensore*>();
}

Autoclave* Model::getAutoclave(unsigned int id) const{
    for (Autoclave* autoclave : autoclavi){
        if (autoclave->getId() == id){
            return autoclave;
        }
    }
    return nullptr;
}

void Model::rimuoviAutoclave(Autoclave* autoclave){
    for (unsigned int i = 0; i < autoclavi.size(); i++){
        if (autoclavi[i] == autoclave){
            autoclavi.erase(autoclavi.begin() + i);
            delete autoclave;
            break;
        }
    }
}

std::vector<Autoclave*> Model::getAutoclavi() const{
    return autoclavi;
}

bool Model::aggiungiSensore(Sensore*& sensore, Autoclave* autoclave, const std::string& tipo, double min, double max){
    if(autoclave->aggiungiSensore(sensore, autoclave, tipo, min, max)){
        return true;
    }
    return false;
}

bool Model::rimuoviSensore(Autoclave* autoclave, const std::string& tipo){
    if(autoclave->rimuoviSensore(autoclave, tipo)){
        return true;
    }
    return false;
}

bool Model::aggiornaSensore(Autoclave* autoclave, const std::string& tipo, double min, double max){
    if(autoclave->aggiornaSensore(autoclave, tipo, min, max)){
        return true;
    }
    return false;
}

std::vector<Sensore*> Model::getSensori(){
    return autoclave->getSensori();
}

Sensore* Model::getSensorePerAutoclave(Autoclave* autoclave, std::string tipo){
    for (auto sensore : autoclave->getSensori()){
        if (sensore->getTipo() == tipo){
            return sensore;
        }
    }
    return nullptr;
}

void Model::impostaCapacita(double capacita){
    autoclave->impostaCapacita(capacita);
}

void Model::salvaAutoclave(Autoclave* autoclave){
    try{
        database->salvaDatiAutoclaveCSV(*autoclave);
    }catch(FileopenException& e){
        qCritical() << e.what();
    }catch(DatabaseException& e){
        qCritical() << e.what();
    }
} 

void Model::caricaAutoclave(const std::string& nomeFile){
    database->setFile(nomeFile);
    Autoclave* newAutoclave = nullptr;
    try{
        newAutoclave = database->caricaDatiAutoclaveCSV(nomeFile);
    }catch(FileopenException& e){
        qCritical() << e.what();
    }

    if(newAutoclave != nullptr){
        setAutoclave(newAutoclave);
        autoclavi.push_back(newAutoclave);
    }
    
}

unsigned int Model::caricaIdAutoclave(const std::string& nomeFile){
    try{
        database->setFile(nomeFile);
        return database->caricaIdAutoclave(nomeFile);
    }catch(FileopenException& e){
        qCritical() << e.what();
        return 0;
    }
}
