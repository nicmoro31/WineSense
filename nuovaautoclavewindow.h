#ifndef NUOVAAUTOCLAVEWINDOW_H
#define NUOVAAUTOCLAVEWINDOW_H

#include "model.h"
#include "mainwindow.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>

class nuovaAutoclaveWindow : public QDialog{
    Q_OBJECT
private:
    QLineEdit* nomeEdit;
    QDoubleSpinBox* capacitaSpin;

    Model* model;
    MainWindow* mainWindow;
public:
    explicit nuovaAutoclaveWindow(QWidget *parent = nullptr);
    ~nuovaAutoclaveWindow();

    void setModel(Model* model);

public slots:
    void onConfermaButtonClicked();

signals:
    void autoclaveAdded();
};

#endif // NUOVAAUTOCLAVEWINDOW_H
