#include "mwindow.h"
#include "template.h"
#include "settings.h"
#include "csv.h"
#include "updater.h"

#include <QThread>
#include <QApplication>
#include <QWindowStateChangeEvent>

using namespace std;

QApplication *app;

NNCGMainWindow *mainWindow;
NNCGTemplate *objTempl;
NNCGSettings objSett;
NNCG_csv *objCSV {nullptr};
QThread *upd_wrk_thr;

QString upgradeFilePath {};
QString lastVerStr {};

extern array<QString, int(varType_t::MAX)> QS_VARTYPES;

// из bool в QString
QString b2s(bool b) {
    return (b) ? "true" : "false";
}

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

void updater_work(NNCGMainWindow *mw_ptr) {
    Updater().make_request(mw_ptr);
    QThread::currentThread()->deleteLater();
}

///////////////////////////////////////////
int main(int argc, char *argv[]) {
    app = new QApplication(argc, argv);
    QApplication::setApplicationName("Network Node Configuration Generator");
    QApplication::setWindowIcon(QIcon(":/rtr.png"));

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

    mainWindow->repaintWithTheme(); // раскраска дочерних элементов в соотв. с текущей темой
    QApplication::postEvent(mainWindow, new QEvent(QEvent::LanguageChange)); // перевод надписей на текущий язык

    if (objSett.autoUpdate) {
        upd_wrk_thr = QThread::create(updater_work, mainWindow); // порождаем отдельный процесс проверки и скачивания новой версии
        upd_wrk_thr->start();
    }

    mainWindow->refreshTable(); // обновление таблицы в соотв. с текущим шаблоном и показ главного окна

    QApplication::exec();

    return 0;
}
