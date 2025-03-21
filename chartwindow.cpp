#include "chartwindow.h"

ChartWindow::ChartWindow(QWidget *parent, const QString& tipoSensore, const QString& nomeAutoclave) : QDialog(parent){
    //inizializzo il model
    model = new Model();

    setWindowTitle("Grafico " + tipoSensore + " - " + nomeAutoclave);
    mainLayout = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout;

    setMinimumSize(1400, 900);

    //lista per scegliere il tipo di grafico
    chartlist = new QListWidget;
    chartlist->addItem("Lineare");
    chartlist->addItem("Barre");

    chartlist->setStyleSheet("QListWidget::item {"
        "border: 1px solid black;"
        "background-color: #A3C1DA;"
        "height: 300px;"
        "}"
        "QListWidget::item:hover{"
        "background-color: #BBD2E9;"
        "}"
        "QListWidget::item:selected{"
        "background-color: #879DB4;"
        "}");

    chartlist->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(chartlist, 1);
    
    //view del grafico
    chart = new QChart;
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //inizializzo charts
    charts = QMap<QString, GeneralChart*>();


    layout->addWidget(chartView, 4);

    mainLayout->addLayout(layout);

    connect(chartlist, &QListWidget::currentItemChanged, this, &ChartWindow::handleItemChanged);

    setLayout(mainLayout);
}

void ChartWindow::handleItemChanged(QListWidgetItem* previous, QListWidgetItem* current) {
    if(previous && previous != current){
        if(current){
            updateChartType(previous->text(), sensorType);

        }else{
            qDebug() << "nessun elemento selezionato";
        }
    }
}

void ChartWindow::updateChartType(const QString &chartType, QString& sensorType) {
    disconnect(chartlist, &QListWidget::currentItemChanged, this, &ChartWindow::handleItemChanged);

    if (charts.contains(chartType)) {
        chart->removeAllSeries();
        if(!chart->axes(Qt::Horizontal).empty())
            chart->removeAxis(chart->axes(Qt::Horizontal).first());
        if(!chart->axes(Qt::Vertical).empty())
            chart->removeAxis(chart->axes(Qt::Vertical).first());
        charts[chartType]->updateChart(chart, sensorType);        //aggiorna il grafico sfruttando il polimorfismo
        chart->update();
    } else {
        qDebug() << "Tipo di grafico non riconosciuto";
    }
    connect(chartlist, &QListWidget::currentItemChanged, this, &ChartWindow::handleItemChanged);
}

void ChartWindow::showGraph(QString tipoSensore, Autoclave* autoclave){
    chart->removeAllSeries();

    if(!chart->axes(Qt::Horizontal).empty())
        chart->removeAxis(chart->axes(Qt::Horizontal).first());
    if(!chart->axes(Qt::Vertical).empty())
        chart->removeAxis(chart->axes(Qt::Vertical).first());
    // Carica i dati
    if(autoclave != nullptr)
        qDebug() << "autoclave: " << autoclave->getNome().c_str();
    else{
        throw AutoclaveException("Autoclave non inizializzata");
    }
    qDebug() << "tipo sensore: " << tipoSensore;
    for (Sensore* sensore : autoclave->getSensori()) {
        if (sensore->getTipo() == tipoSensore.toStdString()) {
            auto timestamps = sensore->getTimestamps();
            auto dati = sensore->getDati();
            auto itDati = dati.begin();

            for (auto timestamp : timestamps) {
                auto epoch = timestamp.time_since_epoch();
                auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
                DatiSensore nuovoDato;
                nuovoDato.timestamp = QDateTime::fromMSecsSinceEpoch(value);
                nuovoDato.dato = *itDati;
                ++itDati;
            }
        }
    }

    //inizializzo la visualizzazione del grafico
    if(!charts.contains("Lineare"))
        charts["Lineare"] = new LineChart(model, autoclave, tipoSensore);
    if(!charts.contains("Barre"))
        charts["Barre"] = new BarChart(model, autoclave, tipoSensore);
    
    sensorType = tipoSensore;
    
    //grafico predefinito
    updateChartType("Lineare", sensorType);
}


void ChartWindow::setModel(Model* model){
    this->model = model;
}

ChartWindow::~ChartWindow(){
    delete model;
    delete mainLayout;
    delete chart;
    if(generalChart)
        delete generalChart;
    delete chartlist;
    for(auto& chart : charts){
        delete chart;
    }
    charts.clear();
}

