#include "generalchart.h"

GeneralChart::GeneralChart(Model* model, Autoclave* autoclave, const QString& tipoSensore): model(model), autoclave(autoclave), tipoSensore(tipoSensore){}

GeneralChart::~GeneralChart(){
    delete model;
    delete autoclave;
}

void GeneralChart::setModel(Model* newModel){
    this->model = newModel;
}

void GeneralChart::setAutoclave(Autoclave* newAutoclave){
    this->autoclave = newAutoclave;
}