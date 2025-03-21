#ifndef BARCHART_H
#define BARCHART_H

#include "generalchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QPointF>
#include <QToolTip>

#include <QCategoryAxis>

#include <QDebug>


class BarChart: public GeneralChart{
    Q_OBJECT
private:
    QBarSeries* barSeries;
    QChart* chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QScatterSeries *marker;
    QStringList categories;

    QString tipoSensore;
public:
    BarChart(Model*, Autoclave*, const QString&);

    void updateChart(QChart* chart, const QString&) override;

    QChart* getChart() const;
    void showTooltip(const QPointF point, bool state);
    virtual ~BarChart();

public slots:
    void handleBarClicked(bool clicked, int index, QBarSet* set);
};

#endif // BARCHART_H
