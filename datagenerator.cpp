#include "datagenerator.h"

DataGenerator::DataGenerator(){}

void DataGenerator::generaDati(Sensore& sensore, int numDati, int intervalloOre){
    std::vector<double> dati;
    std::vector<std::chrono::system_clock::time_point> timestamps;
    double sogliaMin=sensore.getSogliaMinima();
    double sogliaMax=sensore.getSogliaMassima();
    double mean = (sogliaMin+sogliaMax)/2;  // media
    double stdDev = (sogliaMax-sogliaMin)/4; // deviazione standard: 4 deviazioni std coprono il 95% dei dati

    std::default_random_engine generator(static_cast<unsigned>(time(0)));
    std::normal_distribution<double> distribution(mean, stdDev);

    auto now = std::chrono::system_clock::now();
    auto intervallo = std::chrono::hours(intervalloOre);

    for(int i=0; i<numDati; i++){
        double dato;
        do{
            dato = distribution(generator);
            dato = std::round(dato*10.0)/10.0;
        }while(dato<sogliaMin - 3.0 || dato>sogliaMax + 3.0);
        dati.push_back(dato);
        timestamps.push_back(now - i*intervallo);
    }

    sensore.setDati(dati, timestamps);


    //Mostra messaggio di popoup
    QMessageBox msgBox;
    msgBox.setText("Dati generati con successo");
    msgBox.exec();
}
