#ifndef NNCG_CSV_H
#define NNCG_CSV_H

#include "common.h"

class NNCG_csv
{
private:
    QFile qFile;
    QStringList strList;
    bool inspectLine(const QString&, QString&, QString&, varType_t&);

public:
    bool noOpenErr {false};
    QString lastErrMsg;
    QHash<QString, oneRecShort_t> hashVars;
    NNCG_csv(const QString&);
};

#endif // NNCG_CSV_H
