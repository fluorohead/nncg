#include "template.h"

extern varType_t s2t(const QString &str);

const QString defLogo {":/icons8-router-96.png"};

QString NNCGTemplate::getTitle(){
    return strList[2].mid(6);
}

QString NNCGTemplate::getComment(){
    return strList[3].mid(8);
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
    //QRegExp rex(serChar + QString(" *(\\%1.+\\%2) *, *\"(.*)\" *, *([a-zA-Z0-9]+) *").arg(delimOpen, delimClose));
    if (rex.indexIn(line) != -1) {
        varName = rex.cap(1);
        varDescr = rex.cap(2);
        if ((varName.length() >= 3) && (varName.length() <= MAX_VAR_NAME_LEN) && (rex.cap(3).length() != 0)) {
            if (varDescr.length() > MAX_VAR_DESCR_LEN) varDescr.truncate(MAX_VAR_DESCR_LEN); // обрезаем излишне длинное описание переменной
            varType = s2t(rex.cap(3));
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

void NNCGTemplate::inspectBrandColors() {
    QRegExp rex("([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})");
    if (rex.indexIn(strList[5]) != -1) {
        int colors[6];
        bool badGamma {false};
        for (int c = 0; c < 6; c++) {
            colors[c] = rex.cap(c + 1).toInt();
            if ((colors[c] < 0) or (colors[c] > 255)) {
                badGamma = true;
                break;
            }
        }
        if (!badGamma) {
            for (int c = 0; c < 6; c++) {
                brandColors[c] = colors[c];
            }
        } // иначе просто оставляем гамму по-умолчанию (при инициализации объекта)
    }
}


const QString   QS_NNCT = "NETWORK_NODE_CONFIG_TEMPLATE",
                QS_SOFTVER = "SOFTWARE_VERSION:",
                QS_TITLE = "TITLE:",
                QS_COMMENT = "COMMENT:",
                QS_LOGO = "LOGO:",
                QS_BRCOLORS = "BRAND_COLORS:",
                QS_DELIM_OPEN = "DELIMITER_OPEN:",
                QS_DELIM_CLOSE = "DELIMITER_CLOSE:",
                QS_BEGVARS = "BEGIN_VARIABLES",
                QS_ENDVARS = "END_VARIABLES";

// конструктор demo-шаблона
NNCGTemplate::NNCGTemplate() {
    beginConfig = 8;
    QString demoStr =   QS_NNCT + "\r\n" +
                        QS_SOFTVER + " 0.0.1\r\n" +
                        QS_TITLE + " Network Node Configuration Demo Template\r\n" +
                        QS_COMMENT + " created 2023-12-05-21-03\r\n" +
                        QS_LOGO + " \r\n" +
                        QS_DELIM_OPEN + " {\r\n" +
                        QS_DELIM_CLOSE + " }\r\n" +
                        QS_BRCOLORS + " \r\n" +
                        "hostname {hostname}\r\n" +
                        "interface {phy1_ifname}\r\n" +
                        " description {phy1_descr}\r\n" +
                        " ipv4 address {phy1_ip} {phy1_mask}\r\n";
    strList = QStringList(demoStr.split("\r\n", Qt::KeepEmptyParts, Qt::CaseSensitive));
    hashVars["{hostname}"] = {0, "Network node hostname ->", "demo-sr01", Domname};
    hashVars["{phy1_ifname}"] = {1, "Interface name ->", "Giga0/1/0/7", Text};
    hashVars["{phy1_descr}"] = {2, "Interface description ->", "to Internet", Text};
    hashVars["{phy1_ip}"] = {3, "IPv4 address -> ", "192.168.0.2", IPv4};
    hashVars["{phy1_mask}"] = {4, "IPv4 mask ->", "255.255.255.0", MASKv4};
    noOpenErr = true;
    lastErrMsg = tr("demo template loaded");
    pixLogo.load(defLogo);
    isDemo = true;
}

// извлекаем ограничители имён переменных (удаляя начальные и конечные пробелы)
void NNCGTemplate::extractDelimiters() {
    QString s = (strList[6].right(strList[6].length() - QS_DELIM_OPEN.length())).trimmed();
    if (!s.isEmpty()) delimOpen = s;
    s = (strList[7].right(strList[7].length() - QS_DELIM_CLOSE.length())).trimmed();
    if (!s.isEmpty()) delimClose = s;
}


// конструктор шаблона из файла
NNCGTemplate::NNCGTemplate(const QString &fn)
{   qFile.setFileName(fn);
    if (qFile.open(QIODevice::ReadOnly)) {
        if (qFile.size() <= MAX_TMPL_FILE_SIZE) {
            strList = QStringList(QString(qFile.readAll()).split("\r\n", Qt::KeepEmptyParts, Qt::CaseSensitive));
            if (strList.length() >= MIN_TMPL_HEADER_LINES) {
                bool wolfBill = false;
                if (strList[0] != QS_NNCT) wolfBill = true;
                if (!strList[1].startsWith(QS_SOFTVER)) wolfBill = true;
                if (!strList[2].startsWith(QS_TITLE)) wolfBill = true;
                if (!strList[3].startsWith(QS_COMMENT)) wolfBill = true;
                if (!strList[4].startsWith(QS_LOGO)) wolfBill = true;
                if (!strList[5].startsWith(QS_BRCOLORS)) wolfBill = true;
                if (!strList[6].startsWith(QS_DELIM_OPEN)) wolfBill = true;
                if (!strList[7].startsWith(QS_DELIM_CLOSE)) wolfBill = true;
                if (!strList[8].endsWith(QS_BEGVARS)) wolfBill = true;
                if (!wolfBill) {
                    serChar = strList[8][0]; // этим символом должны начинаться все последующие строки переменных
                    extractDelimiters();
                    int varsCount {0};
                    QString varName;
                    QString varDescr;
                    varType_t varType;
                    // настраиваем regexp-паттерн для проверки переменных
                    rex.setPattern(serChar + QString(" *(\\%1.+\\%2) *, *\"(.*)\" *, *([a-zA-Z0-9]+) *").arg(delimOpen, delimClose));
                    for (int h = MIN_TMPL_HEADER_LINES - 1; h < strList.length(); h++) {
                        if (!strList[h].isEmpty()) { // пропускаем пустые строки
                            if (strList[h][0] == serChar) { // проверка на служебный символ
                                if (strList[h] == serChar + QS_ENDVARS) { // значит переменные закончились и дальше текст конфига
                                    beginConfig = ++h;
                                    break;
                                }
                                // ещё не конец блока переменных
                                if (varsCount <= MAX_VARIABLES) {
                                    if (inspectLine(strList[h], varName, varDescr, varType)) {
                                        if (!hashVars.contains(varName)) { // всё нормально, вставляем [ключ]=значение
                                            hashVars[varName] = oneRec_t {varsCount, varDescr, "", varType};
                                            varsCount++;
                                        } else { // такой ключ уже есть, повторение -> ошибка, выходим
                                            noOpenErr = false;
                                            lastErrMsg = tr("repeating of variable at line : ");
                                            lastErrMsg.append(QString::number(++h));
                                            hashVars.clear();
                                            return;
                                        }
                                    } else { // инспекция линии не успешна -> ошибка, выход
                                        noOpenErr = false;
                                        lastErrMsg = tr("wrong syntax at line : ");
                                        lastErrMsg.append(QString::number(++h));
                                        hashVars.clear();
                                        return;
                                    }
                                } else { // превышено количество переменных
                                    noOpenErr = false;
                                    lastErrMsg = tr("too much variables (allowed 1000 MAX)");
                                    hashVars.clear();
                                    return;
                                }
                            } else { // встретился посторонний символ -> больше не ищем переменных и выходим с ошибкой
                                noOpenErr = false;
                                lastErrMsg = tr("syntax error at line : ");
                                lastErrMsg.append(QString::number(++h));
                                hashVars.clear();
                                return;
                            }
                        }
                    }
                    ///// тут, если всё корректно по заголовку и по переменным
                    inspectBrandColors();
                    noOpenErr = true;
                    lastErrMsg = tr("template loaded : ");
                    lastErrMsg.append(fn.section('\\', -1, -1));
                    // загружаем файл логотипа
                    if (!pixLogo.load(getFilePath() + "/" + strList[4].mid(5, -1).simplified())) pixLogo.load(defLogo);

                    /////
                } else {
                    noOpenErr = false;
                    lastErrMsg = tr("incorrect header");
                }
            } else {
                noOpenErr = false;
                lastErrMsg = tr("too short header");
            }
        } else {
            noOpenErr = false;
            lastErrMsg = tr("too big template (allowed 5_000_000 bytes MAX)");
        }
    } else {
        noOpenErr = false;
        lastErrMsg = tr("error opening template file");
    }
}

QString NNCGTemplate::getDelimOpen() {
    return delimOpen;
}

QString NNCGTemplate::getDelimClose() {
    return delimClose;
}
