#include "csv.h"

extern varType_t s2t(QString str);

// возвращает true, если формат переменной верный
bool NNCG_csv::inspectLine(const QString &line, QString &varName, QString &varValue, varType_t &varType) {
    QRegExp rex("\"(\\{.+\\})\";\"(.+)\";\"(.+)\"");
    if (rex.indexIn(line) != -1) {
        varName = rex.cap(1);
        varValue = rex.cap(2);
        if ((varName.length() >= 3) && (varName.length() <= MAX_VAR_NAME_LEN) && (rex.cap(3).length() != 0)) {
            varType = s2t(rex.cap(3));
            varValue.truncate(maxChars[varType]);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

NNCG_csv::NNCG_csv(const QString& fn)
{   qFile.setFileName(fn);
    if (qFile.open(QIODevice::ReadOnly)) {
        if (qFile.size() <= MAX_CSV_FILE_SIZE) {
            strList = QStringList(QString(qFile.readAll()).split("\r\n", QString::SkipEmptyParts));
            if (strList.length() > 1) {
                if (strList[0] == QS_VVT) {
                    QString varName, varValue;
                    varType_t varType;
                    int varsCount {0};
                    for (int h = 1; h < strList.length(); h++) {
                        if (varsCount <= MAX_VARIABLES) {
                            if (inspectLine(strList[h], varName, varValue, varType)) {
                                hashVars[varName] = oneRecShort_t {varValue, varType};
                                varsCount++;
                            }
                        } else {
                            noOpenErr = true;
                            lastErrMsg = QObject::tr("csv loaded : ");
                            lastErrMsg.append(fn.section('\\', -1, -1));
                            lastErrMsg.append(QObject::tr(", extra variables was skipped (allowed 1000 MAX)"));
                            qFile.close();
                            return;
                        }
                    }
/*                    std::cout << "csv lines : " << hashVars.count() << std::endl;
                    for (QHash<QString, oneRecShort_t>::iterator hIt = hashVars.begin(); hIt != hashVars.end(); ++hIt) {
                        std::cout << "key : " << hIt.key().toStdString() << "; value : " << hIt.value().value.toStdString() << std::endl;
                    };*/
                    noOpenErr = true;
                    lastErrMsg = QObject::tr("csv loaded : ");
                    lastErrMsg.append(fn.section('\\', -1, -1));
                } else {
                    noOpenErr = false;
                    lastErrMsg = QObject::tr("wrong header in csv");
                }
            } else {
                noOpenErr = false;
                lastErrMsg = QObject::tr("empty csv");
            }
        } else {
            noOpenErr = false;
            lastErrMsg = QObject::tr("too big csv (allowed 1_000_000 bytes MAX)");
        }
    } else {
        noOpenErr = false;
        lastErrMsg = QObject::tr("error opening csv file");
    }
    qFile.close();
}