#ifndef GENERALCHART_H
#define GENERALCHART_H

#include <QtCharts/QChart>
#include <vector>
#include <QObject>
#include <QString>

#include "model.h"
#include "autoclave.h"


struct DatiSensore;

class GeneralChart: public QObject{
    Q_OBJECT
protected:
    Model* model;
    Autoclave* autoclave;
    const QString& tipoSensore;
public:
    GeneralChart(Model*, Autoclave*, const QString&);
    virtual ~GeneralChart();
    virtual void updateChart(QChart* chart, const QString& sensorType) = 0;
    void setModel(Model*);
    void setAutoclave(Autoclave*);
};

#endif // GENERALCHART_H
