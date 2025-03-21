#include "linechart.h"

LineChart::LineChart(Model* model, Autoclave* autoclave, const QString& tiposensore) : GeneralChart(model, autoclave, tiposensore){
}

void LineChart::updateChart(QChart* chart, const QString& tipoSensore){

    series = new QLineSeries();
     // Variabili per tenere traccia del minimo e massimo timestamp
    qint64 minTimestamp = std::numeric_limits<qint64>::max();
    qint64 maxTimestamp = std::numeric_limits<qint64>::min();

    series->setName("Dati " + tipoSensore);

    //Itero sui sensori dell'autoclave per trovare quelli del tipo specificato
    for(Sensore* sensore : autoclave->getSensori()){
        if(sensore->getTipo() == tipoSensore.toStdString()){
            auto timestamps = sensore->getTimestamps();
            auto dati = sensore->getDati();
            auto itDati = dati.begin();
            for(auto timestamp : timestamps){
                // Converto std::chrono::time_point in millisecondi dal Unix epoch
                auto epoch = timestamp.time_since_epoch();
                auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(value);
                if(itDati != dati.end()){
                    series->append(dateTime.toMSecsSinceEpoch(), *itDati);
                    ++itDati;
                }
                // Aggiorno i valori minimo e massimo
                minTimestamp = std::min(minTimestamp, dateTime.toMSecsSinceEpoch());
                maxTimestamp = std::max(maxTimestamp, dateTime.toMSecsSinceEpoch());

            }
        }
    }

    chart->addSeries(series);

    // Configuro l'asse X
    axisX = new QDateTimeAxis;
    axisX->setFormat("dd/MM/yyyy hh:mm:ss");
    axisX->setTitleText("Data");
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(minTimestamp), QDateTime::fromMSecsSinceEpoch(maxTimestamp));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configuro l'asse Y
    axisY = new QValueAxis;
    QString unitaMisura;
    if(tipoSensore == "Temperatura")
        unitaMisura = "<span style='font-family: Noto Sans; font-size: 11pt;'>&deg;C</span>";
    else
        unitaMisura = QString::fromStdString(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getUnitaMisura());
    axisY->setLabelFormat("%.1f " + unitaMisura);
    axisY->setTitleText("Valore");
    axisY->setRange(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima()-8, model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima()+8);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    //Creo delle serie per le linee delle soglie
    QLineSeries *sogliaMin = new QLineSeries;
    sogliaMin->setName("Soglia minima: " + QString::number(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima()) + " " + unitaMisura);
    sogliaMin->append(axisX->min().toMSecsSinceEpoch(), model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima());
    sogliaMin->append(axisX->max().toMSecsSinceEpoch(), model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima());
    chart->addSeries(sogliaMin);
    sogliaMin->attachAxis(axisX);
    sogliaMin->attachAxis(axisY);

    QLineSeries *sogliaMax = new QLineSeries;
    sogliaMax->setName("Soglia massima: " + QString::number(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima()) + " " + unitaMisura);
    sogliaMax->append(axisX->min().toMSecsSinceEpoch(), model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima());
    sogliaMax->append(axisX->max().toMSecsSinceEpoch(), model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima());
    chart->addSeries(sogliaMax);
    sogliaMax->attachAxis(axisX);
    sogliaMax->attachAxis(axisY);


    marker = new QScatterSeries();
    marker->append(0, 0); // Inizializzo con un punto non visibile o fuori dal range
    marker->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    marker->setMarkerSize(10.0);
    chart->addSeries(marker); // Aggiungo il marker al grafico
    marker->attachAxis(axisX);
    marker->attachAxis(axisY);
    marker->setVisible(false); // Nascondo inizialmente il marker
    

    connect(series, &QLineSeries::hovered, this, &LineChart::showTooltip);

    chart->update();

}

QChart* LineChart::getChart() const{
    return chart;
}

void LineChart::showTooltip(const QPointF point, bool state){
    if(state){
        QString tooltipText = QString("Valore: %1\nData: %2")
            .arg(point.y())
            .arg(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x())).toString("dd/MM/yyyy hh:mm:ss"));

        QToolTip::showText(QCursor::pos(), tooltipText, nullptr, QRect(), 5000);

        //aggiorno la posizione del marker
        if(marker){
        marker->clear();
        marker->setName(tooltipText);
        marker->append(point.x(), point.y());
        marker->setVisible(true);
        }else
            qDebug() << "Marker non inizializzato";
    }else{
        QToolTip::hideText();
    }
}

LineChart::~LineChart(){
    delete series;
    delete chart;
    delete axisX;
    delete axisY;
    delete marker;
}