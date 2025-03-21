#ifndef MODEL_H
#define MODEL_H

#include "sensore.h"
#include "autoclave.h"
#include "database.h"
#include <string>
#include <vector>

class Model{
private:
    Autoclave* autoclave;
    Database* database;

    std::vector<Autoclave*> autoclavi;
public:
    Model();
    Model(double capacitaAutoclave, std::string nomeAutoclave);
    ~Model();
    Model& operator=(const Model&);
    std::vector<Sensore*> getSensori() const;
    void addAutoclave(double, std::string);
    void setAutoclave(Autoclave*);
    Autoclave* getAutoclave(unsigned int) const;
    void rimuoviAutoclave(Autoclave*);
    bool aggiungiSensore(Sensore*&, Autoclave*, const std::string&, double, double);
    bool rimuoviSensore(Autoclave*, const std::string&);
    bool aggiornaSensore(Autoclave*, const std::string&, double, double);
    std::vector<Sensore*> getSensori();
    Sensore* getSensorePerAutoclave(Autoclave*, std::string);
    std::vector<Autoclave*> getAutoclavi() const;
    void impostaCapacita(double);

    void salvaAutoclave(Autoclave*);
    void caricaAutoclave(const std::string&);

    unsigned int caricaIdAutoclave(const std::string&);
};

#endif // MODEL_H
