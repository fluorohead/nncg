#include "settings.h"

#include <QDir>

const QString    QS_TEMPLATE {"template"},
                        QS_THEME {"theme"},
                        QS_MXMZD {"maximized"},
                        QS_WIDTH {"width"},
                        QS_HEIGHT {"height"},
                        QS_COLWIDTH {"col_width"},
                        QS_DARK {"dark"},
                        QS_LIGHT {"light"},
                        QS_LANG {"lang"};



                        const theme_t themeDark {
                            {37, 37, 37, 255}, // big widget background
                            {169, 169, 169, 255} // big widget foreground
                        };

                        theme_t themeCurrent {
                            {37, 37, 37, 255}, // big widget background
                            {169, 169, 169, 255} // big widget foreground
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
                    themeName = jsDoc[QS_THEME].toString().toLower();
                    if ((themeName != QS_DARK) && (themeName != QS_LIGHT)) themeName = QS_DARK;
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

bool NNCGSettings::saveSettings(const QString& fpfn, const QString& theme, int w, int h, const QString& mxmzd, int colWidth, int lang) {
    qFile.resize(0);
    QString writeStr =  QString("{\r\n"
                                " \"%1\": \"%2\",\r\n"
                                " \"%3\": \"%4\",\r\n"
                                " \"%5\": %6,\r\n"
                                " \"%7\": %8,\r\n"
                                " \"%9\": %10,\r\n"
                                " \"%11\": %12,\r\n"
                                " \"%13\": %14\r\n"
                                "}").arg(QS_TEMPLATE, fpfn, QS_THEME, theme, QS_WIDTH, QString::number(w), QS_HEIGHT, QString::number(h),
                                         QS_MXMZD, mxmzd, QS_COLWIDTH, QString::number(colWidth), QS_LANG, QString::number(lang));
    qFile.write(writeStr.toUtf8());
    qFile.flush();
    return true;
}
