#ifndef MWINDOW_H
#define MWINDOW_H

#include "common.h"
#include "buttons.h"
#include "table.h"

#include <QLabel>
#include <QStatusBar>
#include <QValidator>
#include <QTableWidget>
#include <QMainWindow>

class NNCGMainWindow: public QMainWindow
{

private:
    QWidget *bigWidget;
    QStatusBar *statusBar;
    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *commentLabel;
    NNCGTable *table {nullptr};
    QFont fntCons10 {"Consolas", 10, 0}; // фонт для колонки #
    QFont fntCons11 {"Consolas", 11, 0}; // фонт для колонки Description
    QFont fntCons12bold {"Consolas", 12, 100}; // фонт для колонки Value
    void closeEvent(QCloseEvent*);
    void resizeEvent(QResizeEvent *);
    QValidator *vldtrs[varType_t::MAX];

public:
    NNCGMainWindow(QWidget* = nullptr, Qt::WindowFlags = Qt::WindowFlags());
    void refreshTable();
    void dumpTableToHash();
    void clearTable();
    NNCGButtonLoad *btnTemplLoad;
    NNCGButtonCreate *btnCfgCreate;
    NNCGBtnCsvLoad *btnCsvLoad;
    NNCGBtnCsvSave *btnCsvSave;
    NNCGBtnClearAll *btnClearAll;
    NNCGBtnSetLang *btnEngLang;
    NNCGBtnSetLang *btnRusLang;
};

#endif // MWINDOW_H
