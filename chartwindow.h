#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QBarSet>
#include <QDateTime>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QToolTip>

#include <QMap>

#include "autoclave.h"
#include "model.h"
#include "generalchart.h"
#include "linechart.h"
#include "barchart.h"


struct DatiSensore{
    QDateTime timestamp;
    double dato;
};


class ChartWindow: public QDialog{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;

    QChart* chart;

    QMap<QString, GeneralChart*> charts;

    GeneralChart* generalChart;

    Autoclave* autoclave;

    Model* model;

    QListWidget* chartlist;

    QChartView* chartView;

    QString sensorType;

    void updateChartType(const QString &chartType, QString& sensorType);

public:
    ChartWindow(QWidget* parent=nullptr, const QString& tipoSensore = QString(), const QString& nomeAutoclave = QString());
    
    void showGraph(QString tipoSensore, Autoclave* autoclave);
    void setModel(Model* model);

    ~ChartWindow();

public slots:
    void handleItemChanged(QListWidgetItem* previous, QListWidgetItem* current);
};

#endif // CHARTWINDOW_H
