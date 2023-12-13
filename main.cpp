#include "common.h"
#include "mwindow.h"
#include "template.h"
#include "settings.h"
#include "csv.h"

NNCGMainWindow *mainWindow;
NNCGTemplate *objTempl;
NNCGSettings objSett;
NNCG_csv *objCSV {nullptr};

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
    switch(type) {
    case varType_t::IPv4 :
        return QS_IPV4;
    case varType_t::MASKv4 :
        return QS_MASKV4;
    case varType_t::Sysname :
        return QS_SYSNAME;
    case varType_t::Description :
        return QS_DESCR;
    case varType_t::Unsigned :
        return QS_UNSIGNED;
    case varType_t::Password :
        return QS_PASSWD;
    case varType_t::IPv6 :
        return QS_IPV6;
    case varType_t::MASKv6 :
        return QS_MASKV6;
    default :
        return QS_DESCR;
    }
}

// из строки в varType_t
varType_t s2t(const QString &str) {
    QString toLow = str.toLower();
    if (toLow == QS_SYSNAME) return varType_t::Sysname;
    if (toLow == QS_DESCR) return varType_t::Description;
    if (toLow == QS_IPV4) return varType_t::IPv4;
    if (toLow == QS_UNSIGNED) return varType_t::Unsigned;
    if (toLow == QS_PASSWD) return varType_t::Password;
    if (toLow == QS_MASKV4) return varType_t::MASKv4;
    if (toLow == QS_IPV6) return varType_t::IPv6;
    if (toLow == QS_MASKV6) return varType_t::MASKv6;
    return varType_t::Description; // значение по умолчанию
}

///////////////////////////////////////////
int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

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
    }

    mainWindow->refreshTable();

    QApplication::exec();

    return 0;
}
