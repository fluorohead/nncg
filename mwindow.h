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
    QFont fntCons10 {"Consolas", 10, 0}; // фонт для колонки #
    QFont fntCons11 {"Consolas", 11, 0}; // фонт для колонки Description
    QFont fntCons12bold {"Consolas", 12, 100}; // фонт для колонки Value
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
