#include "addsensorwindow.h"
#include "mainwindow.h"

AddSensorWindow::AddSensorWindow(MainWindow* mainWindow, QWidget* parent): QDialog(parent), mainWindow(mainWindow){
    setWindowTitle("Aggiungi Sensore");
    setFixedSize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* tipoLabel = new QLabel("Tipo Sensore:");
    tipoEdit = new QComboBox;
    tipoEdit->addItem("Scegli...");
    tipoEdit->setItemData(0, QVariant(), Qt::UserRole - 1);     //disabilito l'opzione "Scegli..."
    tipoEdit->addItem("Temperatura");
    tipoEdit->addItem("Pressione");
    tipoEdit->addItem("Umidità");
    tipoEdit->addItem("PH");
    tipoEdit->addItem("Livello");

    QLabel* unitaMisuraLabel = new QLabel("Unità di Misura:");
    unitaMisuraEdit = new QLineEdit;    
    unitaMisuraEdit->setReadOnly(true);

    QLabel* minLabel = new QLabel("Minimo:");
    minEdit = new QLineEdit;
    minEdit->setValidator(new QDoubleValidator(minEdit));

    QLabel* maxLabel = new QLabel("Massimo:");
    maxEdit = new QLineEdit;
    maxEdit->setValidator(new QDoubleValidator(maxEdit));

    QPushButton* confermaButton = new QPushButton("Conferma");

    mainLayout->addWidget(tipoLabel);
    mainLayout->addWidget(tipoEdit);
    mainLayout->addWidget(unitaMisuraLabel);
    mainLayout->addWidget(unitaMisuraEdit);
    mainLayout->addWidget(minLabel);
    mainLayout->addWidget(minEdit);
    mainLayout->addWidget(maxLabel);
    mainLayout->addWidget(maxEdit);


    if(mainWindow != nullptr){
        //ottengo l'autoclave selezionata
        try{
            autoclave = mainWindow->getAutoclaveSelected();
        }catch(AutoclaveException& e){
            qDebug() << e.what();
        }
    }else
        throw MainWindowException("Errore: mainWindow non settata");

    //connetto il segnale clicked del pulsante confermaButton al metodo onConfermaButtonClicked
    connect(confermaButton, &QPushButton::clicked, this, &AddSensorWindow::onConfermaButtonClicked);

    connect(tipoEdit, &QComboBox::currentTextChanged, this, &AddSensorWindow::onTipoChanged);

    mainLayout->addWidget(confermaButton);

    isEditing=false;

    //this->setAttribute(Qt::WA_DeleteOnClose);

    setLayout(mainLayout);

}

void AddSensorWindow::setModel(Model* model){
    if(this->model != nullptr &&this->autoclave != nullptr)
        disconnect(this->autoclave, &Autoclave::sensoreNonAggiunto, this, &AddSensorWindow::mostraErrore);

    this->model = model;

    if(this->model != nullptr)
        connect(this->autoclave, &Autoclave::sensoreNonAggiunto, this, &AddSensorWindow::mostraErrore);
}

void AddSensorWindow::onTipoChanged(const QString& text){
    if (text == "Temperatura") {
        unitaMisuraEdit->setText("°C");
    } else if (text == "Pressione") {
        unitaMisuraEdit->setText("bar");
    } else if (text == "Umidità") {
        unitaMisuraEdit->setText("%");
    } else if (text == "PH") {
        unitaMisuraEdit->setText("");
    } else if (text == "Livello") {
        unitaMisuraEdit->setText("l");
    }else{
        unitaMisuraEdit->setText("");
    }
}

void AddSensorWindow::setMainWindow(MainWindow* mainWindow) {
    this->mainWindow = mainWindow;
}

void AddSensorWindow::onConfermaButtonClicked(){
    //Raccolgo i dati dai widget
    QString tipo = tipoEdit->currentText();
    QString unitaMisura = unitaMisuraEdit->text();
    QString min = minEdit->text();
    QString max = maxEdit->text();
    

    //creo un nuovo sensore con i dati raccolti
    bool sensoreAggiunto;
    if(isEditing)
        sensoreAggiunto = model->aggiornaSensore(autoclave, tipo.toStdString(), min.toDouble(), max.toDouble());
    else
        sensoreAggiunto = model->aggiungiSensore(sensore, autoclave, tipo.toStdString(), min.toDouble(), max.toDouble());
        
    if(sensoreAggiunto){
        isEditing=false;
        tipoEdit->setDisabled(false);
        if(mainWindow)
            mainWindow->setUltimoTipoSensoreAggiunto(tipo.toStdString());
        else
            qDebug() << "Errore: mainWindow non settata";
        emit sensorAdded();

        this->close();
    }    
}

std::string AddSensorWindow::getUltimoTipoSensoreAggiunto() const{
    return mainWindow->getUltimoTipoSensoreAggiunto();
}

void AddSensorWindow::mostraErrore(const QString& motivo){
    QMessageBox::warning(this, "Errore", motivo);
}

void AddSensorWindow::setSensore(Sensore* sensore){
    if(sensore != nullptr){
        isEditing=true;
        tipoEdit->setDisabled(true);
        tipoEdit->setCurrentText(QString::fromStdString(sensore->getTipo()));
        minEdit->setText(QString::number(sensore->getSogliaMinima()));
        maxEdit->setText(QString::number(sensore->getSogliaMassima()));
    }
}

void AddSensorWindow::closeEvent(QCloseEvent* event){
    if(this->model != nullptr && this->autoclave != nullptr)
        disconnect(this->autoclave, &Autoclave::sensoreNonAggiunto, this, &AddSensorWindow::mostraErrore);
    
    //chiamo il metodo della classe base per eseguire il comportamento di default
    QWidget::closeEvent(event);
}

AddSensorWindow::~AddSensorWindow(){
    if(this->model != nullptr && this->autoclave != nullptr)
        disconnect(this->autoclave, &Autoclave::sensoreNonAggiunto, this, &AddSensorWindow::mostraErrore);
    delete tipoEdit;
    delete unitaMisuraEdit;
    delete minEdit;
    delete maxEdit;
}