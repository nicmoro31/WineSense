#ifndef LINECHART_H
#define LINECHART_H

#include "generalchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QCategoryAxis>
#include <QDateTime>
#include <QPointF>
#include <QToolTip>


class LineChart : public GeneralChart{
    Q_OBJECT
private:
    QLineSeries* series;
    QChart* chart;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QScatterSeries *marker;
public:
    LineChart(Model*, Autoclave*, const QString&);

    void updateChart(QChart* chart, const QString&) override;
    QChart* getChart() const;

    void showTooltip(QPointF point, bool state);

    virtual ~LineChart();
};

#endif // LINECHART_H
