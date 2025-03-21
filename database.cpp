#include "database.h"

Database::Database(): autoclave(nullptr){
    nomeFile="";
    autoclave = new Autoclave();
}

void Database::setFile(const std::string& nomeFile){
    this->nomeFile = nomeFile;
}

void Database::salvaDatiAutoclaveCSV(const Autoclave& autoclave){
    QString filters = "CSV files (*.csv)";
    QString defaultFilter = "CSV files (*.csv)";
    QString qnomeFile = QFileDialog::getSaveFileName(nullptr, "Salva file", QDir::currentPath(), filters, &defaultFilter);

    if(qnomeFile.isEmpty()){
        throw DatabaseException("Salvataggio annullato");
        return;
    }

    // Aggiungi l'estensione .csv se non è già presente
    if (!qnomeFile.endsWith(".csv", Qt::CaseInsensitive)) {
        qnomeFile += ".csv";
    }
    
    std::string nomeFile = qnomeFile.toStdString();
    std::ofstream file(nomeFile);
    if(file.is_open()){
        //Salvare informazioni sull'autoclave
        file<<"ID,Nome Autoclave,Capacità Autoclave\n";
        file<<autoclave.getId()<<","<<autoclave.getNome()<<","<<autoclave.getCapacita()<<"\n";

        //Salvare informazioni sui sensori
        file<<"Tipo Sensore,Soglia Minima,Soglia Massima,Dati\n";
        for(const auto& sensore : autoclave.getSensori()){
            file<<sensore->getTipo()<<","<<sensore->getSogliaMinima()<<","<<sensore->getSogliaMassima()<<",";

            const auto& datiSensori = sensore->getDati();
            const auto& timestampSensori = sensore->getTimestamps();
            if (datiSensori.empty()){
                throw DatabaseException("Errore: dati del sensore vuoti");
            }
            else{
                for(size_t i=0; i<datiSensori.size(); ++i){
                    file<<datiSensori[i] << " ";

                    //converto il time_point in time_t
                    std::time_t time = std::chrono::system_clock::to_time_t(timestampSensori[i]);

                    //converto il time_t in stringa
                    std::string timeStr = std::ctime(&time);

                    //rimuovo il carattere di newline
                    timeStr.erase(std::remove(timeStr.end()-1, timeStr.end(), '\n'), timeStr.end());

                    file << timeStr;

                    if(i!=datiSensori.size()-1){
                        file<<"|";
                    }
                }
            }
            file<<"\n";
        }
        file.close();
    }else{
        throw FileopenException("Errore nell'apertura del file per il salvataggio");
    }
}


Autoclave* Database::caricaDatiAutoclaveCSV(const std::string& nomeFile){
    std::ifstream file(nomeFile);
    std::string linea;
    autoclave = nullptr;

    if(file.is_open()){
        //Leggere informazioni sull'autoclave
        std::getline(file, linea);      //salto la prima riga perché contiene i nomi delle colonne
        if(std::getline(file, linea)){
            std::stringstream ss(linea);
            std::string id, nome, capacita;
            std::getline(ss, id, ',');
            std::getline(ss, nome, ',');
            std::getline(ss, capacita, ',');
            unsigned int idInt = std::stoi(id);     //converto da stringa a intero
            double capacitaDouble = std::stod(capacita);    //converto da stringa a double
            delete autoclave;   //elimino l'oggetto autoclave se esiste già
            autoclave = new Autoclave();
            autoclave->caricaDati(idInt, nome, capacitaDouble);
        }
        

        //Leggere informazioni sui sensori
        std::getline(file, linea);      //salto la prima riga perché contiene i nomi delle colonne
        while(std::getline(file, linea)){
            std::stringstream ss(linea);
            std::string tipo, sogliaMin, sogliaMax, dati;
            std::getline(ss, tipo, ',');
            std::getline(ss, sogliaMin, ',');
            std::getline(ss, sogliaMax, ',');
            std::getline(ss, dati, ',');

            double min = std::stod(sogliaMin);
            double max = std::stod(sogliaMax);
            std::vector<double> datiSensori;
            std::vector<std::chrono::system_clock::time_point> timestampSensori;
            std::stringstream ssDati(dati);
            std::string dato, timestamp;
            while(std::getline(ssDati, dato, '|')){
                std::locale::global(std::locale("C")); //imposto la locale in modo che come decimale venga usato il punto
                std::stringstream ssDato(dato);
                std::getline(ssDato, dato, ' ');
                std::getline(ssDato, timestamp);
                datiSensori.push_back(std::stod(dato));

                std::stringstream ssTimestamp(timestamp);
                std::tm tm = {};
                ssTimestamp >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");
                auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                timestampSensori.push_back(tp);
            }
            Sensore* sensore = nullptr;
            autoclave->aggiungiSensore(sensore, autoclave, tipo, min, max); 
            if(sensore!=nullptr){                
                sensore->setDati(datiSensori, timestampSensori);
            }
        }
    }else{
        throw FileopenException("Errore nell'apertura del file per il caricamento");
    }
    return autoclave;
}

Autoclave* Database::getAutoclave() const{
    return autoclave;
}

void Database::salvaIdAutoclave(unsigned int id, const std::string& nomeFile){
    std::ofstream file(nomeFile);
    if(file.is_open()){
        file<<id;
        file.close();
    }else{
        throw FileopenException("Errore nell'apertura del file per il salvataggio dell'id");
    }
}

unsigned int Database::caricaIdAutoclave(const std::string& nomeFile){
    std::ifstream file(nomeFile);
    if(file.fail()){
        return -1;
    }
    std::string linea;
    if(file.is_open()){
        std::getline(file, linea);
        return std::stoi(linea);
    }else{
        throw FileopenException("Errore nell'apertura del file per il caricamento dell'id");
        return 0;
    }
}

Database::~Database(){
    delete autoclave;
}