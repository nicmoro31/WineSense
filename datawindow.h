#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QDialog>
#include <QVBoxLayout>

class DataWindow:public QDialog{
    Q_OBJECT
public:
    DataWindow(QWidget* parent = nullptr);
};

#endif // DATAWINDOW_H
