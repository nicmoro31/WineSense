#include "barchart.h"

BarChart::BarChart(Model* model, Autoclave* autoclave, const QString& tiposensore) : GeneralChart(model, autoclave, tiposensore){
}

void BarChart::updateChart(QChart* chart, const QString& sensorType){
    barSeries = new QBarSeries();
    tipoSensore = sensorType;

    // Variabili per tenere traccia del minimo e massimo timestamp
    qint64 minTimestamp = std::numeric_limits<qint64>::max();
    qint64 maxTimestamp = std::numeric_limits<qint64>::min();

    //chart->removeAllSeries(); // Rimuove le serie precedenti

    barSeries->setName("Dati " + tipoSensore);

    QList<QBarSet*> barSets;
    QMap<QString, QBarSet*> dateMap;
    //Itero sui sensori dell'autoclave per trovare quelli del tipo specificato
    for(Sensore* sensore : autoclave->getSensori()){
        if(sensore->getTipo() == tipoSensore.toStdString()){
            auto timestamps = sensore->getTimestamps();
            auto dati = sensore->getDati();
            auto itDati = dati.rbegin();
            QBarSet *set = new QBarSet(tipoSensore);
            for(auto timestamp : timestamps){
                // Converto std::chrono::time_point in millisecondi dal Unix epoch
                auto epoch = timestamp.time_since_epoch();
                auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(value);
                QString dateString = dateTime.toString("dd/MM/yyyy hh:mm:ss");
                if (!categories.contains(dateString)) {
                    categories.append(dateString);
                }

                if(itDati != dati.rend()){
                    if(dateMap.contains(dateString)){
                        // Uso il QBarSet esistente
                        set = dateMap[dateString];
                    } else {
                        // Crea un nuovo QBarSet e aggiungilo alla mappa
                        dateMap[dateString] = set;
                        barSets.prepend(set);
                    }
                    *set << *itDati; // Aggiungo il valore al set
                    ++itDati;
                }
                // Aggiorno i valori minimo e massimo
                minTimestamp = std::min(minTimestamp, dateTime.toMSecsSinceEpoch());
                maxTimestamp = std::max(maxTimestamp, dateTime.toMSecsSinceEpoch());
            }

            for(int i=0; i<barSets.size(); i++){
                barSeries->append(barSets[i]);
            }
        }
    }


    std::reverse(categories.begin(), categories.end());


    // Aggiungo la serie al grafico
    chart->addSeries(barSeries);

    // Configuro l'asse X    
    axisX = new QBarCategoryAxis;
    axisX->append(categories);
    axisX->setTitleText("Data");
    axisX->setLabelsAngle(80);
    if(!chart->axes(Qt::Horizontal).contains(axisX)){
        chart->addAxis(axisX, Qt::AlignBottom);
        barSeries->attachAxis(axisX);
    }

    // Configuro l'asse Y
    axisY = new QValueAxis;
    QString unitaMisura;

    if (tipoSensore == "Temperatura")
        unitaMisura = "<span style='font-family: Noto Sans; font-size: 11pt;'>&deg;C</span>";
    else
        unitaMisura = QString::fromStdString(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getUnitaMisura());

    axisY->setTitleText("Valore " + unitaMisura);
    axisY->setRange(0, model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima()+8);
    if(!chart->axes(Qt::Vertical).contains(axisY)){
        chart->addAxis(axisY, Qt::AlignLeft);
        barSeries->attachAxis(axisY);
    }

    // Configuro il tooltip
    marker = new QScatterSeries;
    marker->setMarkerSize(10);
    marker->setColor(Qt::red);
    chart->addSeries(marker);
    marker->attachAxis(axisX);
    marker->attachAxis(axisY);
    marker->hide();
    connect(barSeries, &QBarSeries::hovered, this, &BarChart::handleBarClicked);


    //Creo delle serie per le linee delle soglie
    QLineSeries *sogliaMin = new QLineSeries;
    sogliaMin->setName("Soglia minima: " + QString::number(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima()) + " " + unitaMisura);
    sogliaMin->append(0, model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima());
    sogliaMin->append(categories.size()-1, model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMinima());
    chart->addSeries(sogliaMin);
    sogliaMin->attachAxis(axisX);
    sogliaMin->attachAxis(axisY);

    QLineSeries *sogliaMax = new QLineSeries;
    sogliaMax->setName("Soglia massima: " + QString::number(model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima()) + " " + unitaMisura);
    sogliaMax->append(0, model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima());
    sogliaMax->append(categories.size()-1, model->getSensorePerAutoclave(autoclave, tipoSensore.toStdString())->getSogliaMassima());
    chart->addSeries(sogliaMax);
    sogliaMax->attachAxis(axisX);
    sogliaMax->attachAxis(axisY);

    chart->update();
}

QChart* BarChart::getChart() const{
    return chart;
}

void BarChart::showTooltip(const QPointF point, bool state){
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

void BarChart::handleBarClicked(bool clicked, int index, QBarSet* set) {
    QPointF point; 
    bool state = clicked; 
    double data = set->at(index); 
    QString categoryLabel = categories.at(index);
    
    
    double datainMS = QDateTime::fromString(categoryLabel, "dd/MM/yyyy hh:mm:ss").toMSecsSinceEpoch();
    point = QPointF(datainMS, data);
    showTooltip(point, state);
}

BarChart::~BarChart(){
    delete barSeries;
    delete chart;
    delete axisX;
    delete axisY;
    delete marker;
}