#ifndef CSV_H
#define CSV_H

#include <QRegularExpression>
#include "common.h"

#include <QFile>
#include <QStringList>
#include <QHash>

class NNCG_csv: public QObject {

    QFile qFile;
    QStringList strList;
    bool inspectLine(const QString &, QString &, QString &, varType_t &);
    QString delimOpen  {"{"};
    QString delimClose {"}"};
    QRegularExpression rex;

public:
    NNCG_csv(const QString &, const QString &, const QString &);

    QString loadPath {"./"};
    bool noOpenErr {false};
    QString lastErrMsg;
    QHash<QString, oneRecShort_t> hashVars;

    QString getFilePath(); // возвр. только путь, без имени файла

};

#endif // CSV_H
