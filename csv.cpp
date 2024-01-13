#include "csv.h"

using namespace std;

extern varType_t s2t(const QString &str);
extern array<int, int(varType_t::MAX)> maxChars;

const QString QS_CSV {"csv"};

extern const QString QS_VVT {R"("variable";"value";"type")"};

// возвращает true, если формат переменной верный
bool NNCG_csv::inspectLine(const QString &line, QString &varName, QString &varValue, varType_t &varType) {
    auto rexMatch = rex.match(line);
    if (rexMatch.hasMatch()) {
        varName = rexMatch.captured(1);
        varValue = rexMatch.captured(2);
        if ((varName.length() >= 3) && (varName.length() <= MAX_VAR_NAME_LEN) && (rexMatch.captured(3).length() != 0)) {
            varType = s2t(rexMatch.captured(3));
            varValue.truncate(maxChars.at(int(varType)));
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

NNCG_csv::NNCG_csv(const QString &fn, const QString &dlmOpen, const QString &dlmClose)
{   qFile.setFileName(fn);
    if (qFile.open(QIODevice::ReadOnly)) {
        if (qFile.size() <= MAX_CSV_FILE_SIZE) {
            strList = QStringList(QString(qFile.readAll()).split("\r\n", Qt::SkipEmptyParts));
            if (strList.length() > 1) {
                if (strList[0] == QS_VVT) {
                    delimOpen = dlmOpen;
                    delimClose = dlmClose;
                    rex.setPattern(QString("\"(\\%1.+\\%2)\";\"(.+)\";\"(.+)\"").arg(delimOpen, delimClose));
                    QString varName;
                    QString varValue;
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
                            lastErrMsg = tr("csv loaded : ");
                            lastErrMsg.append(fn.section('\\', -1, -1));
                            lastErrMsg.append(tr(", extra variables was skipped (allowed 1000 MAX)"));
                            qFile.close();
                            return;
                        }
                    }
                    noOpenErr = true;
                    lastErrMsg = tr("csv loaded : ");
                    lastErrMsg.append(fn.section('\\', -1, -1));
                } else {
                    noOpenErr = false;
                    lastErrMsg = tr("wrong header in csv");
                }
            } else {
                noOpenErr = false;
                lastErrMsg = tr("empty csv");
            }
        } else {
            noOpenErr = false;
            lastErrMsg = tr("too big csv (allowed 1_000_000 bytes MAX)");
        }
    } else {
        noOpenErr = false;
        lastErrMsg = tr("error opening csv file");
    }
    qFile.close();
}
