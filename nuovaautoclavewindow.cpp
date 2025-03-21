#include "nuovaautoclavewindow.h"

nuovaAutoclaveWindow::nuovaAutoclaveWindow(QWidget *parent) : QDialog(parent){
    setWindowTitle("Nuova Autoclave");

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* nomeLabel = new QLabel("Nome Autoclave:");
    nomeEdit = new QLineEdit;

    QLabel* capacitaLabel = new QLabel("Capacità Autoclave:");
    capacitaSpin = new QDoubleSpinBox;  // Usa QDoubleSpinBox invece di QLineEdit
    capacitaSpin->setSingleStep(0.5);  // Imposta l'incremento a 0.5
    capacitaSpin->setDecimals(1);  // Mostra sempre una cifra decimale
    capacitaSpin->setMaximum(9999.0);
    capacitaSpin->clear();


    QPushButton* confermaButton = new QPushButton("Conferma");

    mainLayout->addWidget(nomeLabel);
    mainLayout->addWidget(nomeEdit);
    mainLayout->addWidget(capacitaLabel);
    mainLayout->addWidget(capacitaSpin);

    //connetto il segnale clicked del pulsante confermaButton al metodo onConfermaButtonClicked
    connect(confermaButton, &QPushButton::clicked, this, &nuovaAutoclaveWindow::onConfermaButtonClicked);

    mainLayout->addWidget(confermaButton);

    setLayout(mainLayout);
}

nuovaAutoclaveWindow::~nuovaAutoclaveWindow(){
    delete nomeEdit;
    delete capacitaSpin;
}

void nuovaAutoclaveWindow::setModel(Model* model){
    this->model = model;
}

void nuovaAutoclaveWindow::onConfermaButtonClicked(){
    //Raccolgo i dati dai widget
    QString nome = nomeEdit->text();
    double capacita = capacitaSpin->value();

    //creo una nuova autoclave con i dati raccolti
    model->addAutoclave(capacita, nome.toStdString());

    emit autoclaveAdded();
    
    //Chiudo la finestra
    this->close();
}