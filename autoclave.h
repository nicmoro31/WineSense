#ifndef AUTOCLAVE_H
#define AUTOCLAVE_H

#include <QObject>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iomanip>

#include "sensore.h"

class Database;     //forward declaration

class Autoclave: public QObject{
    Q_OBJECT
private:
    unsigned int id;
    double capacita;
    std::string nome;
    std::vector<Sensore*> sensori;
    //std::set<std::string> tipiSensori;
    std::map<Autoclave*, std::set<std::string>> tipiSensoriPerAutoclave;
    static unsigned int nextId;

public:
    Autoclave();
    Autoclave(double capacita, std::string nome);
    Autoclave(const Autoclave&);
    ~Autoclave();
    std::string getNome() const;
    unsigned int getId() const;
    double getCapacita() const;
    std::vector<Sensore*> getSensori() const;
    Autoclave& operator=(const Autoclave&);
    bool aggiungiSensore(Sensore*&, Autoclave*, const std::string&, double, double);
    bool rimuoviSensore(Autoclave*, const std::string&);
    bool aggiornaSensore(Autoclave*, const std::string&, double, double);
    void impostaCapacita(float);
    void caricaDati(unsigned int, std::string, double);
    std::string checkLimiti(Sensore*);

signals:
    void sensoreNonAggiunto(const QString& motivo);
};

#endif // AUTOCLAVE_H
