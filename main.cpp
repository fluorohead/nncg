#include "mwindow.h"
#include "template.h"
#include "settings.h"
#include "csv.h"

#include <QApplication>
#include <QWindowStateChangeEvent>

//#include <iostream>

QApplication *app;

NNCGMainWindow *mainWindow;
NNCGTemplate *objTempl;
NNCGSettings objSett;
NNCG_csv *objCSV {nullptr};

extern QString QS_VARTYPES[];

theme_t themeDark {
    {37, 37, 37, 255}, // big widget background
    {169, 169, 169, 255} // big widget foreground
};

theme_t themeCurrent {
    {37, 37, 37, 255}, // big widget background
    {169, 169, 169, 255} // big widget foreground
};

// из bool в QString
QString b2s(bool b) { return (b) ? "true" : "false"; }

// из varType_t в строку
QString t2s(varType_t type) {
    return QS_VARTYPES[type];
}

// из строки в varType_t
varType_t s2t(const QString &str) {
    QString toLow = str.toLower();
    for (int vt = 0; vt < varType_t::MAX; vt++){
        if (toLow == QS_VARTYPES[vt]) return (varType_t) vt;
    }
    return varType_t::Text; // значение по умолчанию
}

///////////////////////////////////////////
int main(int argc, char *argv[]) {
    app = new QApplication(argc, argv);
    app->setApplicationName("Network Node Configuration Generator");

    if (!objSett.templFpFn.isEmpty()) { // проверка пути к шаблону
        objTempl = new NNCGTemplate(objSett.templFpFn);
        if (!objTempl->noOpenErr) { // ошибка загрузки шаблона из файла -> грузим демо-шаблон
            delete objTempl;
            objTempl = new NNCGTemplate();
        }
    } else { // пустой путь к шаблону -> грузим демо-шаблон
        objTempl = new NNCGTemplate();
    }

    mainWindow = new NNCGMainWindow(nullptr, Qt::Window);
    if (objTempl->isDemo) {
        mainWindow->btnCfgCreate->setDisabled(true);
        mainWindow->btnCsvLoad->setDisabled(true);
        mainWindow->btnCsvSave->setDisabled(true);
        mainWindow->btnClearAll->setDisabled(true);
    }

    mainWindow->refreshTable();
    objSett.curLang = 1;
    app->postEvent(mainWindow, new QEvent(QEvent::LanguageChange));

    QApplication::exec();

    return 0;
}
