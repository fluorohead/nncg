#ifndef NNCG_CSV_H
#define NNCG_CSV_H


#include "common.h"

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

#endif // NNCG_CSV_H
