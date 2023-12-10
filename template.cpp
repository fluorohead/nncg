#include "template.h"

extern varType_t s2t(QString str);

const QString defLogo {":/icons8-router-96.png"};

QString NNCGTemplate::getTitle(){
    return strList[2].mid(7);
}

QString NNCGTemplate::getComment(){
    return strList[3].mid(9);
}

// возвращает только путь к файлу, без имени файла
QString NNCGTemplate::getFilePath() {
    QString path = QFileInfo(qFile).canonicalPath();
    if (path[path.length()-1] != "/") path.append("/");
    return path;
}

// возвращает только имя файла без пути
QString NNCGTemplate::getFileName() {
    return QFileInfo(qFile).fileName();
}

QPixmap* NNCGTemplate::getPtrPixLogo(){
    return &pixLogo;
}

// возвращает true, если формат переменной верный
bool NNCGTemplate::inspectLine(const QString &line, QString &varName, QString &varDescr, varType_t &varType) {
    QRegExp rex(serChar + " *(\\{.+\\}) *, *\"(.*)\" *, *([a-zA-Z0-9]+) *");
    if (rex.indexIn(line) != -1) {
        varName = rex.cap(1);
        varDescr = rex.cap(2);
        if ((varName.length() >= 3) && (varName.length() <= MAX_VAR_NAME_LEN) && (rex.cap(3).length() != 0)) {
            if (varDescr.length() > MAX_VAR_DESCR_LEN) varDescr.truncate(MAX_VAR_DESCR_LEN); // обрезаем излишне длинное описание переменной
            varType = s2t(rex.cap(3));
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// конструктор demo-шаблона
NNCGTemplate::NNCGTemplate() {
    beginConfig = 6;
    QString demoStr =   "NETWORK_NODE_CONFIG_TEMPLATE\r\n"
                        "SOFTWARE_VERSION: 0.1.0\r\n"
                        "TITLE: Network Node Configuration Demo Template\r\n"
                        "COMMENT: created 2023-12-05-21-03\r\n"
                        "LOGO: \r\n"
                        "BRAND_COLOR: \r\n"
                        "hostname {hostname}\r\n"
                        "interface {phy1_ifname}\r\n"
                        " description {phy1_descr}\r\n"
                        " ipv4 address {phy1_ip} {phy1_mask}\r\n";
    strList = QStringList(demoStr.split("\r\n", Qt::KeepEmptyParts, Qt::CaseSensitive));
    hashVars["{hostname}"] = {0, "Network node hostname ->", "demo-sr01", Sysname};
    hashVars["{phy1_ifname}"] = {1, "Interface name ->", "Giga0/1/0/7", Description};
    hashVars["{phy1_descr}"] = {2, "Interface description ->", "to Internet", Description};
    hashVars["{phy1_ip}"] = {3, "IPv4 address -> ", "192.168.0.2", IPv4};
    hashVars["{phy1_mask}"] = {4, "IPv4 mask ->", "255.255.255.0", MASKv4};
    noOpenErr = true;
    lastErrMsg = QObject::tr("demo template loaded");
    pixLogo.load(defLogo);
    isDemo = true;
}

// конструктор шаблона из файла
NNCGTemplate::NNCGTemplate(const QString &fn)
{   qFile.setFileName(fn);
    if (qFile.open(QIODevice::ReadOnly)) {
        if (qFile.size() <= MAX_TMPL_FILE_SIZE) {
            strList = QStringList(QString(qFile.readAll()).split("\r\n", Qt::KeepEmptyParts, Qt::CaseSensitive));
            if (strList.length() >= MIN_TMPL_HEADER_LINES) {
                bool wolfBill = false;
                if (strList[0] != "NETWORK_NODE_CONFIG_TEMPLATE") wolfBill = true;
                if (!strList[1].startsWith("SOFTWARE_VERSION:")) wolfBill = true;
                if (!strList[2].startsWith("TITLE:")) wolfBill = true;
                if (!strList[3].startsWith("COMMENT:")) wolfBill = true;
                if (!strList[4].startsWith("LOGO:")) wolfBill = true;
                if (!strList[5].startsWith("BRAND_COLOR:")) wolfBill = true;
                if (!strList[6].endsWith("BEGIN_VARIABLES")) wolfBill = true;
                if (!wolfBill) {
                    serChar = strList[6][0];
                    int varsCount {0};
                    QString varName, varDescr;
                    varType_t varType;
                    for (int h = MIN_TMPL_HEADER_LINES - 1; h < strList.length(); h++) {
                        if (!strList[h].isEmpty()) { // пропускаем пустые строки
                            if (strList[h][0] == serChar) { // проверка на служебный символ
                                if (strList[h] == serChar + "END_VARIABLES") { // значит переменные закончились и дальше текст конфига
                                    beginConfig = ++h;
                                    break;
                                } else { // ещё не конец блока переменных
                                    if (varsCount <= MAX_VARIABLES) {
                                        if (inspectLine(strList[h], varName, varDescr, varType)) {
                                            if (!hashVars.contains(varName)) { // всё нормально, вставляем [ключ]=значение
                                                hashVars[varName] = oneRec_t {varsCount, varDescr, "", varType};
                                                varsCount++;
                                            } else { // такой ключ уже есть, повторение -> ошибка, выходим
                                                noOpenErr = false;
                                                lastErrMsg = QObject::tr("repeating of variable at line : ");
                                                lastErrMsg.append(QString::number(++h));
                                                hashVars.clear();
                                                return;
                                            }
                                        } else { // инспекция линии не успешна -> ошибка, выход
                                            noOpenErr = false;
                                            lastErrMsg = QObject::tr("wrong syntax at line : ");
                                            lastErrMsg.append(QString::number(++h));
                                            hashVars.clear();
                                            return;
                                        }
                                    } else { // превышено количество переменных
                                        noOpenErr = false;
                                        lastErrMsg = QObject::tr("too much variables (allowed 1000 MAX)");
                                        hashVars.clear();
                                        return;
                                    }
                                }
                            } else { // встретился посторонний символ -> больше не ищем переменных и выходим с ошибкой
                                noOpenErr = false;
                                lastErrMsg = QObject::tr("syntax error at line : ");
                                lastErrMsg.append(QString::number(++h));
                                hashVars.clear();
                                return;
                            }
                        }
                    }
                    ///// тут, если всё корректно по заголовку и по переменным
                    noOpenErr = true;
                    lastErrMsg = QObject::tr("template loaded : ");
                    lastErrMsg.append(fn.section('\\', -1, -1));
                    //std::cout << "Logo path: " << (getFilePath().toStdString() + "/" + strList[4].mid(6, -1).simplified().toStdString()) << std::endl;
                    if (!pixLogo.load(getFilePath() + "/" + strList[4].mid(6, -1).simplified())) pixLogo.load(defLogo);
                    //std::cout << "variables = " << varsCount << std::endl;
                    //std::cout << "config begins from = " << beginConfig << std::endl;
                    /////
                } else {
                    noOpenErr = false;
                    lastErrMsg = QObject::tr("incorrect header");
                }
            } else {
                noOpenErr = false;
                lastErrMsg = QObject::tr("too short header");
            }
        } else {
            noOpenErr = false;
            lastErrMsg = QObject::tr("too big template (allowed 5_000_000 bytes MAX)");
        }
    } else {
        noOpenErr = false;
        lastErrMsg = QObject::tr("error opening template file");
    }
}
