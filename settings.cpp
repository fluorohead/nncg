#include "common.h"
#include "settings.h"

#include <QDir>

using namespace std;

const QString QS_TEMPLATE {"template"};
const QString QS_THEME {"theme"};
const QString QS_MXMZD {"maximized"};
const QString QS_WIDTH {"width"};
const QString QS_HEIGHT {"height"};
const QString QS_COLWIDTH {"col_width"};
const QString QS_DARK {"dark"};
const QString QS_LIGHT {"light"};
const QString QS_LANG {"lang"};

const array<array<QString, LANGS_AMOUNT>, int(themeId_t::UnknownTheme)> QS_THEMESNAMES {{
                                                                                        {"Dark", "Тёмная"},
                                                                                        {"Light", "Светлая"}
                                                                                      }};

//"color: rgb(190, 190, 190); gridline-color: rgb(50, 50, 50); background-color: rgb(60, 60, 60)");

extern const array<theme_t, themeId_t::UnknownTheme> gamma {
    //       background           table fg (text)        table gridline        table bg               vert. handle    title and comment fg
    theme_t{{37,   37,  37, 255}, {190, 190, 190, 255}, { 50,  50,  50, 255}, { 60,  60,  60, 255}, { 60,  60,  60}, {169, 169, 169, 255}}, // dark theme gamma
    theme_t{{255, 255, 255, 255}, {  0,   0,   0, 255}, {150, 150, 150, 255}, {255, 255, 255, 255}, {160, 160, 160}, {0  ,   0,   0, 255}} // light theme gamma
};

NNCGSettings::NNCGSettings() {
    QDir qDir;
    QString strDir {QDir::homePath() + "/.nncg/"};
    bool ok {true};
    if (!qDir.exists(strDir)) ok = qDir.mkdir(strDir);
    if (ok) {
        qFile.setFileName(strDir + "nncg.json");
        if (!qFile.exists()) {
            ok = qFile.open(QIODevice::ReadWrite | QIODevice::NewOnly); // создаём файл
        } else {
            ok = qFile.open(QIODevice::ReadWrite); // либо открываем существующий
        }
        if (ok) {
            if (qFile.size() <= MAX_JSON_FILE_SIZE) {
                QByteArray qBA(qFile.readAll());
                QJsonParseError jsErr {};
                jsDoc = QJsonDocument::fromJson(qBA, &jsErr);
                if (!jsDoc.isNull()) {
                    width = jsDoc[QS_WIDTH].toInt();
                    height = jsDoc[QS_HEIGHT].toInt();
                    templFpFn = jsDoc[QS_TEMPLATE].toString();
                    curThemeId = (themeId_t) jsDoc[QS_THEME].toInt();
                    if (curThemeId < 0 || curThemeId >= themeId_t::UnknownTheme) curThemeId = themeId_t::Dark;
                    maximized = jsDoc[QS_MXMZD].toBool();
                    colWidth = jsDoc[QS_COLWIDTH].toInt();
                    if (colWidth < 39) colWidth = 39;
                    if (colWidth > 2048) colWidth = BASE_WIDTH_COLUMN_DESCR;
                    curLang = (langId_t) jsDoc[QS_LANG].toInt();
                    if (curLang < langId_t::English || curLang >= langId_t::Elfian) curLang = langId_t::English;
                    noLastErr = true;
                    lastErrMsg = tr("succesfully parsed nncg.json");
                } else {
                    noLastErr = false;
                    lastErrMsg = jsErr.errorString();
                }
            } else {
                noLastErr = false;
                lastErrMsg = tr("too big nncg.json, skipped");
            }
        } else { // ошибка создания или открытия файла
            noLastErr = false;
            lastErrMsg = tr("error creating or opening /.nncg/ in home dir");
        }
    } else { // ошибка создания или открытия подкаталога /.nncg/
        noLastErr = false;
        lastErrMsg = tr("error creating or opening /.nncg/ in home dir");
    }
}

bool NNCGSettings::saveSettings(const QString& fpfn, int theme, int w, int h, const QString& mxmzd, int colWidth, int lang) {
    qFile.resize(0);
    QString writeStr =  QString("{\r\n"
                                " \"%1\": \"%2\",\r\n"
                                " \"%3\": %4,\r\n"
                                " \"%5\": %6,\r\n"
                                " \"%7\": %8,\r\n"
                                " \"%9\": %10,\r\n"
                                " \"%11\": %12,\r\n"
                                " \"%13\": %14\r\n"
                                "}").arg(QS_TEMPLATE, fpfn, QS_THEME, QString::number(theme), QS_WIDTH, QString::number(w), QS_HEIGHT, QString::number(h),
                                         QS_MXMZD, mxmzd, QS_COLWIDTH, QString::number(colWidth), QS_LANG, QString::number(lang));
    qFile.write(writeStr.toUtf8());
    qFile.flush();
    return true;
}
