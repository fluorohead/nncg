#include "mwindow.h"
#include "template.h"
#include "settings.h"
#include "csv.h"

#include <QApplication>
#include <QWindowStateChangeEvent>

using namespace std;

QApplication *app;

NNCGMainWindow *mainWindow;
NNCGTemplate *objTempl;
NNCGSettings objSett;
NNCG_csv *objCSV {nullptr};

extern array<QString, int(varType_t::MAX)> QS_VARTYPES;

// из bool в QString
QString b2s(bool b) { return (b) ? "true" : "false"; }

// из varType_t в строку
QString t2s(varType_t type) {
    return QS_VARTYPES.at(int(type));
}

// из строки в varType_t
varType_t s2t(const QString &str) {
    QString toLow = str.toLower();
    for (int vt = 0; vt < varType_t::MAX; vt++){
        if (toLow == QS_VARTYPES.at(vt)) return (varType_t) vt;
    }
    return varType_t::Text; // значение по умолчанию
}

///////////////////////////////////////////
int main(int argc, char *argv[]) {
    app = new QApplication(argc, argv);
    QApplication::setApplicationName("Network Node Configuration Generator");

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

    mainWindow->repaintWithTheme(); // раскраска дочерних элеметов в соотв. в текущей темой
    QApplication::postEvent(mainWindow, new QEvent(QEvent::LanguageChange)); // перевод надписей на текущий язык
    mainWindow->refreshTable(); // обновление таблицы в соотв. с текущим шаблоном и показ главного окна

    QApplication::exec();

    return 0;
}
