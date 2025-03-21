#ifndef ADDSENSORWINDOW_H
#define ADDSENSORWINDOW_H

#include "model.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCloseEvent>

class MainWindow;


class AddSensorWindow:public QDialog{
    Q_OBJECT
private:
    QLineEdit *nomeEdit;
    QComboBox *tipoEdit;
    QLineEdit *unitaMisuraEdit;
    QLineEdit *minEdit;
    QLineEdit *maxEdit;

    Model* model;

    MainWindow* mainWindow;

    Autoclave* autoclave;

    Sensore* sensore;

    std::string ultimoTipoSensoreAdded;

    bool isEditing;
public:
    explicit AddSensorWindow(MainWindow*, QWidget* parent = nullptr);
    ~AddSensorWindow();

    void setModel(Model* model);

    void setMainWindow(MainWindow* mainWindow);

    std::string getUltimoTipoSensoreAggiunto() const;

    void setSensore(Sensore* sensore);

    void closeEvent(QCloseEvent* event);

public slots:
    void onConfermaButtonClicked();
    void onTipoChanged(const QString&);
    void mostraErrore(const QString&);
signals:
    void sensorAdded();
};

#endif // ADDSENSORWINDOW_H
