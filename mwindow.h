#ifndef NNCGMAINWINDOW_H
#define NNCGMAINWINDOW_H

#include "common.h"
#include "buttons.h"

class NNCGMainWindow: public QMainWindow
{

    Q_OBJECT

private:
    QWidget *bigWidget;
    QStatusBar *statusBar;
    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *commentLabel;
    QTableWidget *table;
    QFont twiTah {"Tahoma", 10, 0}; // фонт для колонки с номером строки
    QFont twiTahBold {"Tahoma", 10, 100}; // фонт для колонки Description
    QFont twiCourNew {"Courier new", 12, 100}; // фонт для колонки Value
    void closeEvent(QCloseEvent* event);

public:
    NNCGMainWindow(QWidget* = nullptr, Qt::WindowFlags = Qt::WindowFlags());
    void refreshTable();
    void dumpTableToHash();
    NNCGButtonLoad *btnTemplLoad;
    NNCGButtonCreate *btnCfgCreate;
    NNCGBtnCsvLoad *btnCsvLoad;
    NNCGBtnCsvSave *btnCsvSave;
};

#endif // NNCGMAINWINDOW_H
