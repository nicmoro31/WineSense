#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <vector>
#include <ctime>
#include <random>
#include <cmath>
#include <chrono>
#include <QDebug>
#include <QMessageBox>

#include "sensore.h"

class DataGenerator{
public:
    DataGenerator();

    void generaDati(Sensore&, int, int);
};

#endif // DATAGENERATOR_H
