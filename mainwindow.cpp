#include "mainwindow.h"
#include "nuovaautoclavewindow.h"
#include "addsensorwindow.h"

void MainWindow::addMenus()
{
    menuBar = new QMenuBar();

    QMenu* file = new QMenu("File", menuBar);
    menuBar->addMenu(file);

    file->addAction(new QAction("Nuova Autoclave", file));
    file->addAction(new QAction("Carica Autoclave", file));

    saveAction = new QAction("Salva", file);
    saveAction->setEnabled(false);      //Disabilito il tasto salva: riabilitarlo chiamando saveAction->setEnabled(true); quando apro un'autoclave
    file->addAction(saveAction);

    closeAction = new QAction("Chiudi Autoclave", file);
    closeAction->setEnabled(false);      //Disabilito il tasto chiudi: riabilitarlo chiamando closeAction->setEnabled(true); quando apro un'autoclave
    file->addAction(closeAction);

    file->addAction(new QAction("Chiudi", file));


    visualizza = new QMenu("Visualizza", menuBar);
    visualizza->setEnabled(false);      //Disabilito il tasto visualizza: riabilitarlo chiamando visualizza->setEnabled(true); quando apro un'autoclave
    menuBar->addMenu(visualizza);

    visualizza->addAction(new QAction("Grafico Temperatura", visualizza));
    visualizza->addAction(new QAction("Grafico Pressione", visualizza));
    visualizza->addAction(new QAction("Grafico Umidità", visualizza));
    visualizza->addAction(new QAction("Grafico PH", visualizza));
    visualizza->addAction(new QAction("Grafico Livello", visualizza));


    connect(file->actions().at(0), &QAction::triggered, this, &MainWindow::nuovaAutoclaveView);
    //connetto bottone di salvataggio dell'autoclave con la funzione di salvataggio
    connect(file->actions().at(2), &QAction::triggered, this, [this](){
        Autoclave* autoclave = nullptr;
        try{
            autoclave = getAutoclaveSelected();
        }catch(AutoclaveException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }
        if(autoclave != nullptr){
            model->salvaAutoclave(autoclave);
        }else{
            QMessageBox::critical(this, "Errore", "Errore: Autoclave non settata");
        }
    });

    //connetto bottone di caricamento dell'autoclave con la funzione di caricamento
    connect(file->actions().at(1), &QAction::triggered, this, &MainWindow::caricaAutoclaveView);

    
    //connetto il bottone di chiusura dell'autoclave
    connect(file->actions().at(3), &QAction::triggered, this, [this](){
        Autoclave* autoclaveSelected = getAutoclaveSelected();
        try{
            chiudiAutoclave(autoclaveSelected);
        }catch(AutoclaveException& e){
            QMessageBox::critical(this, "Errore", e.what());
        }
    });

    //connetto il bottone di visualizzazione del grafico temperatura
    connect(visualizza->actions().at(0), &QAction::triggered, this, [this](){
        Autoclave* autoclaveSelected = getAutoclaveSelected();
        try{
            visualizzaGraficoDallaBar("Temperatura", autoclaveSelected);
        }catch(ChartException& e){
            QMessageBox::critical(this, "Errore", e.what());
        }
    });

    //connetto il bottone di chiusura del programma
    connect(file->actions().at(4), &QAction::triggered, this, &MainWindow::close);


    //connetto il bottone di visualizzazione del grafico pressione
    connect(visualizza->actions().at(1), &QAction::triggered, this, [this](){
        Autoclave* autoclaveSelected = getAutoclaveSelected();
        try{
            visualizzaGraficoDallaBar("Pressione", autoclaveSelected);
        }catch(ChartException& e){
            QMessageBox::critical(this, "Errore", e.what());
        }
    });

    //connetto il bottone di visualizzazione del grafico umidità
    connect(visualizza->actions().at(2), &QAction::triggered, this, [this](){
        Autoclave* autoclaveSelected = getAutoclaveSelected();
        try{
            visualizzaGraficoDallaBar("Umidità", autoclaveSelected);
        }catch(ChartException& e){
            QMessageBox::critical(this, "Errore", e.what());
        }
    });

    //connetto il bottone di visualizzazione del grafico ph
    connect(visualizza->actions().at(3), &QAction::triggered, this, [this](){
        Autoclave* autoclaveSelected = getAutoclaveSelected();
        try{
            visualizzaGraficoDallaBar("PH", autoclaveSelected);
        }catch(ChartException& e){
            QMessageBox::critical(this, "Errore", e.what());
        }
    });

    //connetto il bottone di visualizzazione del grafico livello
    connect(visualizza->actions().at(4), &QAction::triggered, this, [this](){
        Autoclave* autoclaveSelected = getAutoclaveSelected();
        try{
            visualizzaGraficoDallaBar("Livello", autoclaveSelected);
        }catch(ChartException& e){
            QMessageBox::critical(this, "Errore", e.what());
        }
    });


    mainLayout->setMenuBar(menuBar);
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent){

    model = new Model();    // Creo il model

    mainLayout = new QVBoxLayout;

    //imposto grandezza minima finestra
    setMinimumSize(1000, 600);

    //Aggiungo il menu
    addMenus();
    

    //Aggiungo il layout principale (lo schermo)

    mainLayout->addStretch(); //Aggiungo uno stretch per centrare il logo
    //Carico il logo
    QPixmap logo("img/WS.png");

    QLabel* logoLabel = new QLabel(this);
    logoLabel->setPixmap(logo);
    logoLabel->setAlignment(Qt::AlignCenter); //Centro l'immagine

    mainLayout->addWidget(logoLabel);

    mainLayout->addStretch(); //Aggiungo uno stretch per centrare l'immagine

    //Aggiungo il layout per i pulsanti
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    nuovaAutoclave = new QPushButton("Nuova Autoclave", this);
    caricaAutoclave = new QPushButton("Carica Autoclave", this);

    // Imposta la grandezza dei pulsanti
    nuovaAutoclave->setFixedSize(QSize(200, 50));
    caricaAutoclave->setFixedSize(QSize(200, 50));

    // Imposta lo stile dei pulsanti
    QString buttonStyle = "QPushButton {"
                        "background-color: #A3C1DA; "
                        "border-style: outset; "
                        "border-width: 2px; "
                        "border-radius: 10px; "
                        "border-color: beige; "
                        "font: bold 14px; "
                        "min-width: 10em; "
                        "padding: 6px; "
                        "}"
                        "QPushButton:hover {"
                        "background-color: #BBD2E9; "
                        "}"
                        "QPushButton:pressed {"
                        "background-color: #879DB4; "
                        "}";
    nuovaAutoclave->setStyleSheet(buttonStyle);
    caricaAutoclave->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(nuovaAutoclave);
    buttonLayout->addWidget(caricaAutoclave);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch(); //Aggiungo uno stretch per centrare i pulsanti

    //interfaccia nuova autoclave
    connect(nuovaAutoclave, &QPushButton::clicked, this, &MainWindow::nuovaAutoclaveView);
    //interfaccia carica autoclave
    connect(caricaAutoclave, &QPushButton::clicked, this, &MainWindow::caricaAutoclaveView);

    QIcon icon("img/WS_icon.png");
    this->setWindowIcon(icon);

    isClicked=false;
    autoclaveSelezionata=nullptr;

    addSensorButton = nullptr;

    addsensorwindow = new AddSensorWindow(this);

    //creo la lista delle autoclavi
    autoclaveList = new QListWidget;

    sensorList = new QListWidget;
    
    // Crea il pulsante di aggiunta sensore
    addSensorButton = new QPushButton("Aggiungi sensore");

    //Creo la barra di ricerca
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Cerca un sensore");
    searchBox->setVisible(false);

    // Creo una QLabel per mostrare un messaggio se non ci sono sensori
    sensorMessageLabel = new QLabel(this);

    setLayout(mainLayout);
}

void MainWindow::setModel(Model* model){
    this->model = model;
}

void MainWindow::setAddSensorWindow(AddSensorWindow* addsensorwindow){
    try{
        this->addsensorwindow = addsensorwindow;
    }catch(MainWindowException& e){
        QMessageBox::warning(this, "Errore", e.what());
    }
}


void MainWindow::nuovaAutoclaveView(){
    //Creo la finestra per la nuova autoclave
    nuovaAutoclaveWindow* nuovaAutoclave = new nuovaAutoclaveWindow();
    nuovaAutoclave->setModel(model);    // Passo il model alla finestra

    connect(nuovaAutoclave, &nuovaAutoclaveWindow::autoclaveAdded, this, &MainWindow::manageView);

    //La mostro
    nuovaAutoclave->show();
}

void MainWindow::caricaAutoclaveView(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Carica Autoclave"), QDir::currentPath(), tr("CSV files (*.csv)"));
    if(sensorList != nullptr)
        sensorList->clear();
    if (!fileName.isEmpty()){
        model->caricaAutoclave(fileName.toStdString());
        try{
            manageView();
        }catch(AutoclaveException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }
    }
}

MainWindow::~MainWindow(){
    delete model;
    delete mainLayout;
    delete menuBar;
    delete nuovaAutoclave;
    delete caricaAutoclave;
    delete autoclaveList;
    delete detailWidget;
    delete sensorMessageLabel;
    delete sensorList;
    delete addSensorButton;
    delete generateDataButton;
    delete showChartButton;
    delete addsensorwindow;
    delete dataWindow;
    delete saveAction;
    delete closeAction;
    delete visualizza;
    delete detailLayout;
    delete searchBox;
}


void MainWindow::manageView(){
    std::vector<Autoclave*> autoclavi = model->getAutoclavi();

    for(const auto& autoclave : autoclavi){
        for(const auto& sensore : autoclave->getSensori()){
            isEditingMap[autoclave->getId()][sensore->getTipo()] = false;
        }
    }

    while(autoclaveList->count() > 0){
        QListWidgetItem* item = autoclaveList->takeItem(0);
        delete item;
    }
    // Aggiungi tutte le autoclavi alla lista
    for(Autoclave* autoclave : autoclavi){
        if (autoclave != nullptr) {
            autoclaveLists[autoclave->getId()] = autoclaveList;
            std::string id = std::to_string(autoclave->getId());    // Converte l'id in stringa
            std::string nome = autoclave->getNome();
            double capacita = autoclave->getCapacita();

            std::stringstream stream;
            stream<<std::fixed<<std::setprecision(1)<<capacita; // Imposta la precisione a 1 cifra decimale
            std::string capacitaStr=stream.str();  // Converte il valore in stringa

            std::string autoclaveInfo = "ID: " + id + "\nCapacità: " + capacitaStr + " L" + "\nNome: " + nome;
            QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(autoclaveInfo));
            item->setSizeHint(QSize(item->sizeHint().width(), 60));     // Imposta l'altezza dell'elemento della lista a 60

            // Imposta l'autoclave come data dell'elemento della lista
            item->setData(Qt::UserRole, QVariant::fromValue(autoclave));

            //item->setBackground(Qt::lightGray);    // Imposta il colore di sfondo dell'elemento della lista
            autoclaveList->addItem(item);
        }
        else{
            throw AutoclaveException("Errore: autoclave non trovata");
        }
    }

    if(autoclaveList->count() > 1){
        autoclaveList->setCurrentRow(autoclaveList->count()-1);
        sensorList->update();
        try{
            showAutoclaveDetails();
        }catch(AutoclaveException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }catch(SensorException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }
    }


    nuovaAutoclave->hide();
    caricaAutoclave->hide();

    //Abilito il tasto salva
    saveAction->setEnabled(true);
    //Abilito il tasto chiudi
    closeAction->setEnabled(true);
    //Abilito il tasto visualizza
    visualizza->setEnabled(true);

    // Remove the existing layout if it exists
    if (this->layout() != nullptr) {
        QLayoutItem* item;
        while ((item = this->layout()->takeAt(0)) != nullptr) {
            if(dynamic_cast<QMenuBar*>(item->widget()) == nullptr){
                delete item->widget();
            }
            delete item;
        }
    }

    QHBoxLayout* manageLayout = new QHBoxLayout;

    autoclaveList->setStyleSheet(
        "QListWidget::item {"
        "border: 1px solid black;"
        "background-color: #A3C1DA;"
        "}"
        "QListWidget::item:hover{"
        "background-color: #BBD2E9;"
        "}"
        "QListWidget::item:selected{"
        "background-color: #879DB4;"
        "}"
    );

    manageLayout->addWidget(autoclaveList,2); //aggiungo la lista delle autoclavi al layout principale e la faccio espandere in orizzontale (2)


    //creo la finestra per la visualizzazione delle autoclavi
    detailWidget = new QWidget;
    detailWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    detailLayout = new QVBoxLayout;

    addSensorButton->setEnabled(false);    // Disabilito il pulsante all'avvio
    
    // Aggiungi i pulsanti al layout
    detailLayout->addWidget(addSensorButton, 0, Qt::AlignTop|Qt::AlignRight);

    detailLayout->addWidget(searchBox, 0, Qt::AlignTop|Qt::AlignLeft);


    sensorMessageLabel->setObjectName("sensorMessageLabel");
    sensorMessageLabel->setStyleSheet("QLabel { "
                                        "font: bold 14px; "
                                        "text-align: center; }");
    

   
    detailWidget->setLayout(detailLayout);
    detailLayout->addWidget(sensorMessageLabel, 0, Qt::AlignTop|Qt::AlignCenter);
    
    // Connetti il segnale itemClicked() del QListWidget al metodo setEnabled() del pulsante
    connect(autoclaveList, &QListWidget::itemClicked, [this](QListWidgetItem*) {
        addSensorButton->setEnabled(true);
    });


    manageLayout->addWidget(detailWidget,4); //aggiungo la finestra di visualizzazione delle autoclavi al layout principale e la faccio espandere in orizzontale (4)

    //isViewManaged = true;
    mainLayout->addLayout(manageLayout);
    setLayout(mainLayout);

    connect(autoclaveList, &QListWidget::currentItemChanged, this, [this](QListWidgetItem* current, QListWidgetItem* previous) {
        if (current != nullptr && current != previous) {
            try{
                showAutoclaveDetails();
            }catch(AutoclaveException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }catch(SensorException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }
            onAutoClaveSelected();
        }
    });
    
    //connetto il segnale di click sul pulsante di aggiunta sensore con la funzione di aggiunta sensore
    connect(addSensorButton, &QPushButton::clicked, this, &MainWindow::addSensorView);

    //connetto il segnale di scrittura sul box di ricerca con la funzione di ricerca
    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::searchSensore);
}

void MainWindow::searchSensore(){
    sensorList->clear();
    std::vector<Sensore*> sensori;
    try{
        sensori = getAutoclaveSelected()->getSensori();
    }catch(AutoclaveException& e){
        qDebug() << e.what();
        QMessageBox::critical(this, "Errore", e.what());
    }
    QString searchText = searchBox->text();
    for(Sensore* sensore : sensori){
        QString nomeSensore = QString::fromStdString(model->getSensorePerAutoclave(getAutoclaveSelected(), sensore->getTipo())->getTipo());
        
        if(nomeSensore.contains(searchText, Qt::CaseInsensitive)){
            try{
                showAutoclaveDetails(nomeSensore.toStdString());
            }catch(AutoclaveException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }catch(SensorException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }
        }
    }

    if(searchText.isEmpty()){
        try{
            showAutoclaveDetails();
        }catch(AutoclaveException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }catch(SensorException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }
    }else if(sensorList->count() == 0){
         QListWidgetItem* noResultsLabel = new QListWidgetItem("Nessun risultato trovato");
         sensorList->addItem(noResultsLabel);
    }
}


void MainWindow::setIsViewManaged(bool newIsViewManaged){
    isViewManaged = newIsViewManaged;
}


void MainWindow::mostraDatiGenerati(const std::string& tipoSensore, Autoclave* selectedAutoclave){
    const std::vector<Sensore*> sensori = selectedAutoclave->getSensori();

    //ottengo il layout dalla datawindow
    QVBoxLayout* datiGeneratiLayout = static_cast<QVBoxLayout*>(dataWindow->layout());

    //Pulisco il layout dei dati generati prima di aggiungere i nuovi dati
    QLayoutItem* item;
    while((item = datiGeneratiLayout->takeAt(0)) != nullptr){
        delete item->widget();
        delete item;
    }

    bool datiGenerati = false;
    for(Sensore* sensore : sensori){
        if(sensore != nullptr && sensore->getTipo() == tipoSensore){
            QLabel* sensoreLabel = new QLabel(QString::fromStdString(sensore->getTipo()));
            datiGeneratiLayout->addWidget(sensoreLabel);
            std::vector<double> dati = sensore->getDati();
            std::vector<std::chrono::system_clock::time_point> timestamps = sensore->getTimestamps();
            for(size_t i=0; i<dati.size(); ++i){
                std::time_t timestamp = std::chrono::system_clock::to_time_t(timestamps[i]);
                QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                QString formattedTimeStamp = dateTime.toString("dd/MM/yyyy hh:mm:ss");
                QLabel* datoLabel = new QLabel(QString::number(dati[i]) + " " + QString::fromStdString(sensore->getUnitaMisura()) + " - " + formattedTimeStamp);
                if(sensore->getSogliaMinima() > dati[i] || sensore->getSogliaMassima() < dati[i])
                    datoLabel->setStyleSheet("color: red;");
                datiGeneratiLayout->addWidget(datoLabel);
                datiGenerati = true;
            }
        }else{
            qDebug() << "Sensore non trovato";
        }
    }

    if(!datiGenerati){
        QLabel* datoLabel = new QLabel("Nessun dato generato");
        datiGeneratiLayout->addWidget(datoLabel);
    }
}

void MainWindow::showAutoclaveDetails(std::string tipoSensoreCercato){
    // disconnect per evitare che la funzione venga chiamata per ogni autoclave ma solo per quella selezionata
    disconnect(autoclaveList, &QListWidget::currentItemChanged, this, nullptr);

    //aggiorna il widget di dettaglio in base all'autoclave selezionata
    QListWidgetItem* selectedItem = autoclaveList->currentItem();

    //ottengo l'autoclave selezionata
    Autoclave* selectedAutoclave = nullptr;
    if(selectedItem)
        selectedAutoclave = selectedItem->data(Qt::UserRole).value<Autoclave*>();

    if (selectedAutoclave){
        // Aggiorna i widget figli di detailWidget con i dettagli dell'autoclave
        QLabel* nameLabel = detailWidget->findChild<QLabel*>("nameLabel");
	
        if (nameLabel) {
            nameLabel->setText(QString::fromStdString(selectedAutoclave->getNome()));
        }
	
        if(sensorMessageLabel != nullptr){
            sensorMessageLabel->hide(); // Nasconde la QLabel
            searchBox->show();  // Mostra la QLineEdit
        }
        	
        if (sensorList != nullptr){
            sensorList->clear();
    	}
	
        if(selectedAutoclave->getSensori().empty()){
            //se non ci sono sensori
            if(sensorList != nullptr)
                sensorList->hide(); // Nasconde la QListWidget
            sensorMessageLabel = findChild<QLabel*>("sensorMessageLabel");
            if (sensorMessageLabel) {
                sensorMessageLabel->setText("Non ci sono sensori per questa autoclave");
                sensorMessageLabel->show();  // Mostra la QLabel
                searchBox->hide();  // Nasconde la QLineEdit
            }
            else{
                qDebug () << "sensorMessageLabel è nullptr";
            }
        }else{
            if(sensorList != nullptr)
                sensorList->show(); // Mostra la QListWidget
            if(sensorMessageLabel != nullptr)
                sensorMessageLabel->hide();     // Nasconde la QLabel
            if(sensorList == nullptr){
                sensorList = new QListWidget(this);
                sensorList->setStyleSheet(
                    "background-color: transparent;"
                );
            }
	
            const std::vector<Sensore*> sensori = selectedAutoclave->getSensori();
            std::vector<std::string> tipi = {"Temperatura", "Pressione", "Umidità", "PH", "Livello"};

            for(Sensore* sensore : sensori){
            if(tipoSensoreCercato != "x" && sensore->getTipo() != tipoSensoreCercato)
                continue;
            if(sensore != nullptr){
                // Crea un nuovo frame per l'elemento della lista
                QListWidget *listItemWidget = new QListWidget();
                listItemWidget->setFrameShape(QFrame::StyledPanel);
                listItemWidget->setFrameShadow(QFrame::Raised);
                listItemWidget->setStyleSheet("background-color: #FFFFFF;");
                listItemWidget->setMinimumSize(50, 100);
                listItemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


                // Crea un layout per il widget
                QVBoxLayout *listItemLayout = new QVBoxLayout();

                int sogliaMinima = sensore->getSogliaMinima();
                int sogliaMassima = sensore->getSogliaMassima();
                std::string tipodaestrarre = sensore->getTipo();

                std::string unitaDiMisura = sensore->getUnitaMisura();
                //mostro a video i dati del sensore
                std::string sensoreDetails = "Tipo: " + tipodaestrarre + "\nSoglia minima: " + std::to_string(sogliaMinima) + " " + unitaDiMisura + "\nSoglia massima: " + std::to_string(sogliaMassima) + " " + unitaDiMisura;
                
                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(sensoreDetails), sensorList);
                item->setData(Qt::UserRole, QVariant::fromValue(sensore));

                QLabel* sensoreDetailsLabel = new QLabel(QString::fromStdString(sensoreDetails));
                listItemLayout->addWidget(sensoreDetailsLabel);
                

                // Aggiungo i bottoni di modifica e rimozione del sensore
                QPushButton* editButton = new QPushButton();
                editButton->setIcon(QIcon("img/edit_icon.png"));
                editButton->setIconSize(QSize(30, 30));
                editButton->setFixedSize(30, 30);
                editButton->setToolTip("Modifica sensore");

                QPushButton* removeButton = new QPushButton();
                removeButton->setIcon(QIcon("img/delete_icon.png"));
                removeButton->setIconSize(QSize(30, 30));
                removeButton->setFixedSize(30, 30);
                removeButton->setToolTip("Rimuovi sensore");

                // Aggiungo il bottone di generazione dei dati
                generateDataButton = new QPushButton();
                generateDataButton->setIcon(QIcon("img/genera_dati_icon.png"));
                generateDataButton->setIconSize(QSize(30, 30));
                generateDataButton->setText("Genera dati");

                //creo un pulsante per visualizzare i dati generati
                QPushButton* viewDataButton = new QPushButton;
                viewDataButton->setIcon(QIcon("img/view_list_icon.png"));
                viewDataButton->setIconSize(QSize(30, 30));
                viewDataButton->setText("Visualizza dati");

                //creo il pulsante per la visualizzazione del grafico
                showChartButton = new QPushButton;
                showChartButton->setIcon(QIcon("img/show_chart_icon.png"));
                showChartButton->setIconSize(QSize(30, 30));
                showChartButton->setText("Visualizza grafico");

                //creo un layout per i bottoni
                QHBoxLayout* buttonLayout = new QHBoxLayout;
                buttonLayout->addStretch(1); // Add stretch to push buttons to the right
                buttonLayout->addWidget(generateDataButton);
                buttonLayout->addWidget(viewDataButton);
                buttonLayout->addWidget(showChartButton);
                buttonLayout->addStretch(1);
                buttonLayout->addWidget(editButton);
                buttonLayout->addWidget(removeButton);
                
                listItemLayout->addLayout(buttonLayout);

                // Imposta il layout sul widget
                listItemWidget->setLayout(listItemLayout);

                // Imposta il widget personalizzato come widget dell'elemento della lista
                sensorList->setItemWidget(item, listItemWidget);

                // Imposta la dimensione dell'elemento della lista per adattarsi al widget
                item->setSizeHint(listItemWidget->sizeHint());

                //connetto il click sul pulsante di modifica con la funzione di modifica del sensore
                connect(editButton, &QPushButton::clicked, this, [this, sensore](){
                    addsensorwindow = new AddSensorWindow(this);
                    addsensorwindow->setModel(model);    // Passo il model alla finestra
                    addsensorwindow->setMainWindow(this); // Passo il mainwindow alla finestra
                    addsensorwindow->setSensore(sensore); // Passo il sensore alla finestra
                    addsensorwindow->show();
                    connect(addsensorwindow, &AddSensorWindow::sensorAdded, this, [this, sensore](){  // Connetto il segnale di aggiunta di un sensore con la funzione di aggiornamento della vista
                        this->generateDataButton->setEnabled(true);
                        Autoclave* autoclave = nullptr;
                        try{
                            autoclave = this->getAutoclaveSelected();
                        }catch(AutoclaveException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }
                        if(autoclave)
                            this->isEditingMap[autoclave->getId()][sensore->getTipo()] = true;

                        try{
                            this->showAutoclaveDetails();
                        }catch(AutoclaveException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }catch(SensorException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }
                    });
                
                });


                //connetto il click sul pulsante di rimozione con la funzione di rimozione del sensore
                connect(removeButton, &QPushButton::clicked, this, [this, sensore](){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Rimuovi sensore", "Sei sicuro di voler rimuovere il sensore?",
                                                    QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        Autoclave* autoclave = nullptr;
                        try{
                            autoclave = getAutoclaveSelected();
                        }catch(AutoclaveException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }
                        if(autoclave != nullptr){
                            qDebug() << "Autoclave rimuovi sensore: " << autoclave;
                            std::string tipo = sensore->getTipo();
                            if(model->rimuoviSensore(autoclave, tipo)){
                                qDebug() << "Sensore rimosso";
                                try{
                                    showAutoclaveDetails();
                                }catch(AutoclaveException& e){
                                    qDebug() << e.what();
                                    QMessageBox::critical(this, "Errore", e.what());
                                }catch(SensorException& e){
                                    qDebug() << e.what();
                                    QMessageBox::critical(this, "Errore", e.what());
                                }
                            }else{
                                qDebug() << "Sensore non rimosso";
                            }
                        }else{
                            throw AutoclaveException("Errore: Autoclave non settata");
                        }
                    }else{
                        qDebug() << "Rimozione sensore annullata";
                    }
                });



                //connetto il click sul pulsante di generazione dati con la funzione di generazione dati
                connect(generateDataButton, &QPushButton::clicked, this, [this, sensore](){
                    if(sensore != nullptr){
                        DataGenerator dataGenerator;
                        dataGenerator.generaDati(*sensore, 12, 1);
                        Autoclave* autoclave = nullptr;
                        try{
                            autoclave = this->getAutoclaveSelected();
                        }catch(AutoclaveException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }
                        if(autoclave)
                            this->isEditingMap[autoclave->getId()][sensore->getTipo()] = false;
                            
                        try{
                            this->showAutoclaveDetails();
                        }catch(AutoclaveException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }catch(SensorException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }
                        
                    }else{
                        qDebug() << "Sensore è nullptr";
                    }
                });
                
                if(!isEditingMap[selectedAutoclave->getId()][sensore->getTipo()])
                    sensoreHaDati(sensore->getTipo(), selectedAutoclave);

                //connetto il click sul pulsante di mostrare i dati generati
                connect(viewDataButton, &QPushButton::clicked, this, [this, sensore](){
                    if(sensore)
                        this->dataView(sensore->getTipo());
                    else
                        qDebug() << "Sensore è nullptr";
                });


                //connetto il click sul pulsante di visualizzazione del grafico
                connect(showChartButton, &QPushButton::clicked, this, [this, sensore, selectedAutoclave](){
                    if(sensore){
                        ChartWindow* chartWindow = new ChartWindow(this, QString::fromStdString(sensore->getTipo()), QString::fromStdString(selectedAutoclave->getNome()));
                        chartWindow->setModel(model);
                        try{
                            chartWindow->showGraph(QString::fromStdString(sensore->getTipo()), selectedAutoclave);
                        }catch(AutoclaveException& e){
                            qDebug() << e.what();
                            QMessageBox::critical(this, "Errore", e.what());
                        }
                        chartWindow->show();
                    }else{
                        throw SensorException("Errore: sensore non trovato");
                    }
                });                           
            
            }else   
                throw SensorException("Errore: sensore non trovato");
            }

            QVBoxLayout* detailLayout = detailWidget->findChild<QVBoxLayout*>();
            if(detailLayout->indexOf(sensorList) == -1)
                detailLayout->addWidget(sensorList);         

        }
    }else
        qDebug() << "Autoclave non selezionata";

    connect(autoclaveList, &QListWidget::currentItemChanged, this, [this](QListWidgetItem* current, QListWidgetItem* previous) {
        if (current != nullptr && current != previous) {
            try{
                showAutoclaveDetails();
            }catch(AutoclaveException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }catch(SensorException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }
            onAutoClaveSelected();
        }
    });
}

void MainWindow::addSensorView(){
    if(addsensorwindow != nullptr){
        delete addsensorwindow;
        addsensorwindow=nullptr;
    }
    qDebug() << "dopo if addsensorwindow";
    //creo la finestra per l'aggiunta di un sensore
    AddSensorWindow* addSensor = nullptr;
    try{
        addSensor = new AddSensorWindow(this);
    }catch(MainWindowException& e){
        QMessageBox::warning(this, "Errore", e.what());
    }

    if(addSensor != nullptr){
        this->setAddSensorWindow(addSensor);

        addSensor->setModel(model);    // Passo il model alla finestra
        addSensor->setMainWindow(this); // Passo il mainwindow alla finestra

        connect(addSensor, &AddSensorWindow::sensorAdded, this, &MainWindow::sensorAddedHandler);   // Connetto il segnale di aggiunta di un sensore con la funzione di aggiornamento della vista

        //La mostro
        addSensor->show();
    }else{
        qDebug() << "Errore: finestra di aggiunta sensore non creata";
    }
}

void MainWindow::sensorAddedHandler(){
    try{
        showAutoclaveDetails();
    }catch(AutoclaveException& e){
        qDebug() << e.what();
        QMessageBox::critical(this, "Errore", e.what());
    }catch(SensorException& e){
        qDebug() << e.what();
        QMessageBox::critical(this, "Errore", e.what());
    }
}

void MainWindow::setUltimoTipoSensoreAggiunto(const std::string& tipo){
    ultimoTipoSensoreAggiunto = tipo;

}
std::string MainWindow::getUltimoTipoSensoreAggiunto() const{
    return ultimoTipoSensoreAggiunto;
}

void MainWindow::onAutoClaveSelected(){
    Autoclave* autoclave = autoclaveList->currentItem()->data(Qt::UserRole).value<Autoclave*>();
    autoclaveSelezionata = autoclave;
}

Autoclave* MainWindow::getAutoclaveSelected() const{
    if(autoclaveSelezionata != nullptr){
        return autoclaveSelezionata;
    }else{
        throw AutoclaveException("Autoclave non selezionata");
    }
}

Autoclave* MainWindow::getAutoclaveById(int id){
    return model->getAutoclave(id);
}


void MainWindow::resizeEvent(QResizeEvent* event){
    QWidget::resizeEvent(event);
    try{
        showAutoclaveDetails();
    }catch(AutoclaveException& e){
        qDebug() << e.what();
        QMessageBox::critical(this, "Errore", e.what());
    }catch(SensorException& e){
        qDebug() << e.what();
        QMessageBox::critical(this, "Errore", e.what());
    }
}

void MainWindow::dataView(std::string tipo){
    dataWindow = new DataWindow(this);
    dataWindow->show();
    try{
        this->mostraDatiGenerati(tipo, getAutoclaveSelected());
    }catch(AutoclaveException& e){
        qDebug() << e.what();
        QMessageBox::critical(this, "Errore", e.what());
    }
}


void MainWindow::sensoreHaDati(const std::string& tipoSensore, Autoclave* autoclaveSelected){
    const std::vector<Sensore*> sensori = autoclaveSelected->getSensori();

    for(Sensore* sensore : sensori){
        if(sensore != nullptr && sensore->getTipo() == tipoSensore){
            std::vector<double> dati = sensore->getDati();
            if(!dati.empty()){
                generateDataButton->setEnabled(false);
                showChartButton->setEnabled(true);
            }else{
                generateDataButton->setEnabled(true);
                showChartButton->setEnabled(false);
            }
        }
    }
}

void MainWindow::visualizzaGraficoDallaBar(QString tipoSensore, Autoclave* selectedAutoclave){
    const std::vector<Sensore*> sensori = selectedAutoclave->getSensori();
    Sensore* sensoreTrovato = nullptr;

    for(Sensore* sensore : sensori){
        if(sensore->getTipo() == (tipoSensore).toStdString()){
            sensoreTrovato = sensore;
            break;
        }else{ 
            sensoreTrovato = nullptr;
        }
    }

    if(!sensoreTrovato || sensoreTrovato->getDati().empty()){
        throw ChartException("Sensore non esistente o senza dati. \nGenera i dati prima di visualizzare il grafico");
    }else{
        if(sensoreTrovato){
            ChartWindow* chartWindow = new ChartWindow(this, QString::fromStdString(sensoreTrovato->getTipo()), QString::fromStdString(selectedAutoclave->getNome()));
            chartWindow->setModel(model);
            try{
                chartWindow->showGraph(QString::fromStdString(sensoreTrovato->getTipo()), selectedAutoclave);
            }catch(AutoclaveException& e){
                qDebug() << e.what();
                QMessageBox::critical(this, "Errore", e.what());
            }
            chartWindow->show();
        }else{
            qDebug() << "Sensore è nullptr";
        }
    }
}

void MainWindow::chiudiAutoclave(Autoclave* autoclave){
    if(autoclave != nullptr){
        model->rimuoviAutoclave(autoclave);
        sensorList->clear();
        try{
            manageView();
        }catch(AutoclaveException& e){
            qDebug() << e.what();
            QMessageBox::critical(this, "Errore", e.what());
        }
    }else{
        throw AutoclaveException("Errore: Autoclave non selezionata");
    }
}
