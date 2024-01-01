#ifndef CSV_H
#define CSV_H

#include "QtCore5Compat/qregexp.h"
#include "common.h"

#include <QFile>
#include <QStringList>
#include <QHash>

class NNCG_csv: public QObject
{

private:
    QFile qFile;
    QStringList strList;
    bool inspectLine(const QString &, QString &, QString &, varType_t &);
    QString delimOpen  {"{"};
    QString delimClose {"}"};
    QRegExp rex;

public:
    bool noOpenErr {false};
    QString lastErrMsg;
    QHash<QString, oneRecShort_t> hashVars;
    NNCG_csv(const QString &, const QString &, const QString &);
};

#endif // CSV_H
