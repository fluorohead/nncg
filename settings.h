#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"
#include <QFile>
#include <QJsonDocument>

extern const QString QS_DARK;

class NNCGSettings: public QObject
{

private:
    QFile qFile;
    QJsonDocument jsDoc;

public:
    int width {800};
    int height {600}; // main window size
    bool maximized {false};
    int curLang {int(langId_t::English)};
    QString templFpFn {""}; // template file path with file name
    int curThemeId {themeId_t::Dark};
    bool noLastErr {false};
    QString lastErrMsg;
    int colWidth {BASE_WIDTH_COLUMN_DESCR};
    NNCGSettings();
    bool saveSettings(const QString &, int, int, int, const QString &, int, int);
};

#endif // SETTINGS_H
