#include "common.h"
#include "settings.h"

#include <QDir>

using namespace std;

const QString QS_TEMPLATE {"template"};
const QString QS_LAST_PATH_CFG {"last_cfg_path"};
const QString QS_LAST_PATH_CSV {"last_csv_path"};
const QString QS_AUTO_UPDATE {"auto_update"};
const QString QS_THEME {"theme"};
const QString QS_MXMZD {"maximized"};
const QString QS_WIDTH {"width"};
const QString QS_HEIGHT {"height"};
const QString QS_COLWIDTH {"col_width"};
const QString QS_LANG {"lang"};

const array<array<QString, LANGS_AMOUNT>, int(themeId_t::UnknownTheme)> QS_THEMESNAMES {{ {"Dark", "Тёмная"}, {"Light", "Светлая"} }};

extern const array<theme_t, themeId_t::UnknownTheme> appGamma {
    //       bg               table fg (text)  table gridline   table bg         vert. handle     title
    theme_t{{37,   37,  37}, {190, 190, 190}, { 50,  50,  50}, { 60,  60,  60}, { 60,  60,  60}, {169, 169, 169}}, // dark theme gamma
    theme_t{{210, 202, 188}, {  0,   0,   0}, {150, 150, 150}, {240, 239, 229}, {160, 160, 160}, { 97,  88,  96}} // light theme gamma
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
                    if (colWidth < TABLE_COLUMN_ONE_MIN_WIDTH) colWidth = TABLE_COLUMN_ONE_MIN_WIDTH;
                    if (colWidth > 2048) colWidth = BASE_WIDTH_COLUMN_DESCR;
                    curLang = (langId_t) jsDoc[QS_LANG].toInt();
                    if (curLang < langId_t::English || curLang >= langId_t::Elfian) curLang = langId_t::English;
                    lastPathCfg = jsDoc[QS_LAST_PATH_CFG].toString();
                    lastPathCSV = jsDoc[QS_LAST_PATH_CSV].toString();
                    autoUpdate = jsDoc[QS_AUTO_UPDATE].toBool();
                    noLastErr = true;
                    lastErrMsg = "succesfully parsed nncg.json";
                } else {
                    noLastErr = false;
                    lastErrMsg = jsErr.errorString();
                }
            } else {
                noLastErr = false;
                lastErrMsg = "too big nncg.json, skipped";
            }
        } else { // ошибка создания или открытия файла
            noLastErr = false;
            lastErrMsg = "error creating or opening /.nncg/ in home dir";
        }
    } else { // ошибка создания или открытия подкаталога /.nncg/
        noLastErr = false;
        lastErrMsg = "error creating or opening /.nncg/ in home dir";
    }
}

bool NNCGSettings::saveSettings(const QString& fpfn, int theme, int w, int h, const QString& mxmzd, int colWidth, int lang, const QString& fpCfg, const QString& fpcsv, bool auto_upd) {
    qFile.resize(0);
    QString writeStr =  QString("{\r\n"
                                " \"%1\": \"%2\",\r\n"
                                " \"%3\": %4,\r\n"
                                " \"%5\": %6,\r\n"
                                " \"%7\": %8,\r\n"
                                " \"%9\": %10,\r\n"
                                " \"%11\": %12,\r\n"
                                " \"%13\": %14,\r\n"
                                " \"%15\": \"%16\",\r\n"
                                " \"%17\": \"%18\",\r\n"
                                " \"%19\": %20\r\n"
                                "}").arg(QS_TEMPLATE, fpfn,
                                         QS_THEME, QString::number(theme),
                                         QS_WIDTH, QString::number(w),
                                         QS_HEIGHT, QString::number(h),
                                         QS_MXMZD, mxmzd,
                                         QS_COLWIDTH, QString::number(colWidth),
                                         QS_LANG, QString::number(lang),
                                         QS_LAST_PATH_CFG, fpCfg,
                                         QS_LAST_PATH_CSV, fpcsv,
                                         QS_AUTO_UPDATE, (auto_upd) ? "true" : "false"
                                );
    qFile.write(writeStr.toUtf8());
    qFile.flush();
    return true;
}

QString NNCGSettings::getTemplPathWOFn() {
    return templFpFn.left(templFpFn.lastIndexOf('/')+1);
}
