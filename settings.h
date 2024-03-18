#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"
#include <QFile>
#include <QJsonDocument>

class NNCGSettings: public QObject {

    QFile qFile;
    QJsonDocument jsDoc;

public:
    NNCGSettings();

    int width {800};
    int height {600}; // main window size
    int curThemeId {themeId_t::Dark};
    int curLang {int(langId_t::English)};
    int colWidth {BASE_WIDTH_COLUMN_DESCR};
    bool maximized {false};
    bool noLastErr {false};
    bool autoUpdate {false};
    QString templFpFn {""}; // template file path with file name
    QString lastErrMsg;
    QString lastPathCfg {"./"};
    QString lastPathCSV {"./"};

    QString getTemplPathWOFn(); // returns template file path without file name
    bool saveSettings(const QString &, int, int, int, const QString &, int, int, const QString &, const QString &, bool);
};

#endif // SETTINGS_H
