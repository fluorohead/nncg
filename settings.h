#ifndef NNCGSETTINGS_H
#define NNCGSETTINGS_H

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
    langId_t curLang {langId_t::English};
    QString templFpFn {""}; // template file path with file name
    QString themeName {QS_DARK}; // название цветовой темы : dark, light
    bool noLastErr {false};
    QString lastErrMsg;
    int colWidth {BASE_WIDTH_COLUMN_DESCR};
    NNCGSettings();
    bool saveSettings(const QString &, const QString &, int, int, const QString &, int, langId_t);
};

#endif // NNCGSETTINGS_H
