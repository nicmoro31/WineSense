#include "datawindow.h"

DataWindow::DataWindow(QWidget* parent):QDialog(parent){
    setWindowTitle("Dati raccolti");
    
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    setLayout(mainLayout);

}
