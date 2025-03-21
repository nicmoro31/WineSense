#ifndef DATABASE_H
#define DATABASE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <QDebug>
#include <QFileDialog>

#include "autoclave.h"
#include "exceptions.h"


class Database{
private:
    std::string nomeFile;
    Autoclave* autoclave;
public:
    Database();
    void setFile(const std::string&);
    void salvaDatiAutoclaveCSV(const Autoclave&);
    Autoclave* caricaDatiAutoclaveCSV(const std::string&);

    Autoclave* getAutoclave() const;

    void salvaIdAutoclave(unsigned int, const std::string&);

    unsigned int caricaIdAutoclave(const std::string&);

    ~Database();
};

#endif // DATABASE_H
