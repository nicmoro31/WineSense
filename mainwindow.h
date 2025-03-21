#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QPixmap>
#include <QIcon>
#include <QCoreApplication>
#include <QListWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QVariant> 
#include <iomanip>
#include <QScrollArea>
#include <QGroupBox>

#include "model.h"
#include "addsensorwindow.h"
#include "datagenerator.h"
#include "datawindow.h"
#include "chartwindow.h"
#include "exceptions.h"


class MainWindow : public QWidget{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    QMenuBar* menuBar;
    QAction* saveAction;
    QAction* closeAction;
    QAction* nuovaAutoclaveAction;
    QAction* caricaAutoclaveAction;
    QMenu* visualizza;
    
    QVBoxLayout* detailLayout;

    QPushButton* nuovaAutoclave;
    QPushButton* caricaAutoclave;

    QMap<int, QListWidget*> autoclaveLists;
    QListWidget* autoclaveList;
    QWidget* detailWidget;

    QLabel* sensorMessageLabel;

    QListWidget* sensorList;

    QPushButton* addSensorButton;

    QLineEdit* searchBox;

    QPushButton* generateDataButton;

    QPushButton* showChartButton;

    std::string ultimoTipoSensoreAggiunto;

    bool isEditing;

    std::map<unsigned int, std::map<std::string, bool>> isEditingMap;


    Model* model;

    AddSensorWindow* addsensorwindow;

    Autoclave* autoclaveSelezionata;

    DataWindow* dataWindow;

    bool isClicked;

    void addMenus();

    void mostraDatiGenerati(const std::string&, Autoclave*);

    void sensoreHaDati(const std::string&, Autoclave*);
    
protected:
    void resizeEvent(QResizeEvent* event) override;
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setModel(Model* model);

    void setAddSensorWindow(AddSensorWindow* addsensorwindow);

    bool isViewManaged = false;

    void setIsViewManaged(bool);

    void setUltimoTipoSensoreAggiunto(const std::string&);
    std::string getUltimoTipoSensoreAggiunto() const;

    Autoclave* getAutoclaveSelected() const;

    void showAutoclaveDetails(std::string tipoSensoreCercato = "x");

    void onAutoClaveSelected();

    Autoclave* getAutoclaveById(int);

    void visualizzaGraficoDallaBar(QString, Autoclave*);

    void chiudiAutoclave(Autoclave*);

public slots:
    void nuovaAutoclaveView();
    void caricaAutoclaveView();

    void manageView();
    
    void addSensorView();

    void dataView(std::string);

    void searchSensore();

    void sensorAddedHandler();

    
};
#endif // MAINWINDOW_H
