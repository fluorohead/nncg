#include "validators.h"
#include <iostream>
#include <QRegExp>

extern QString b2s(bool b);

bool simpleIPv4Check(QString &str) { // check for common ipv4-address format requirements
    // разрешены только цифры и точки
    for (auto idx = 0; idx < str.length(); idx++) {
        if (str[idx].unicode() > 57) return false;
        if ((str[idx].unicode() < 48) && (str[idx].unicode() != 46)) return false;
    }
    // проверка на символы прошла успешно
    if (str.startsWith('.')) return false; // ip не может начинаться с точки
    if (str.contains("..")) return false; // не может быть двух точек подряд
    if (str.count('.') > 3) return false; // не может быть более 3 точек всего
    return true;
}


// проверяет октет маски на соответствие одному из 9 предопределённых значений
bool isCorrectMASKv4_Octet(int octet) {
    const int rightValues[] {255, 254, 252, 248, 240, 224, 192, 128, 0};
    for (int m = 0; m <= 8; m++) {
        if (octet == rightValues[m]) return true;
    }
    return false;
}


QValidator::State NNCGValidIPv4::validate(QString &input, int &pos) const {
    if (!simpleIPv4Check(input)) return Invalid;
    else {
        if (rex.indexIn(input) != -1) { // проверка на regexp
            // все октеты на месте, далее проверяем каждый
            input.clear(); // чистим строку, будем пересобирать её заново
            for (auto idx = 1; idx <= 4; idx++) {
                int octet;
                octet = rex.cap(idx).toInt();
                if (octet > 255) octet = 255;
                input.append(QString::number(octet) + '.');
            }
            input.chop(1); // откусываем лишнюю точку в конце
        } else { // не прошли проверку на regexp, значит незавершённый ввод
            return Intermediate;
        }
    }
    return Acceptable;
}

void NNCGValidIPv4::fixup(QString &input) const {
    input = "0.0.0.0";
}


QValidator::State NNCGValidMASKv4::validate(QString &input, int &pos) const {
    //std::cout << "current mask to inspect : " << input.toStdString() << "; cursor position : " << pos << std::endl;
    if (!simpleIPv4Check(input)) return Invalid;
    else {
        if (rex.indexIn(input) != -1) { // проверка на regexp
            // все октеты на месте, далее проверяем каждый
            input.clear(); // чистим строку, будем пересобирать её заново
            int bitsArr[4];
            for (int idx = 1; idx <= 4; idx++) {
                int octet;
                octet = rex.cap(idx).toInt();
                if (octet > 255) octet = 255;
                input.append(QString::number(octet) + '.');
                bitsArr[idx - 1] = octet;
            }
            input.chop(1); // откусываем лишнюю точку в конце
            for (int ar = 0; ar <= 3; ar++){ // проверяем каждый октет : могут иметь только одно из 9 определённых значений
                if (!isCorrectMASKv4_Octet(bitsArr[ar])) return Intermediate;
            }
            if (!(bitsArr[0] >= bitsArr[1]) || !(bitsArr[1] >= bitsArr[2]) || !(bitsArr[2] >= bitsArr[3])) return Intermediate;
        } else { // не прошли проверку на regexp, значит незавершённый ввод
            return Intermediate;
        }
    }
    return Acceptable;
}


void NNCGValidMASKv4::fixup(QString &input) const {
    input = "255.255.255.255";
}


QValidator::State NNCGValidDomname::validate(QString &input, int &pos) const {
    for (auto idx = 0; idx < input.length(); idx ++) {
        for (auto f = 0; f < forbidden.length(); f++) {
            if (input[idx] == forbidden[f]) return Invalid;
        }
    }
    input = input.toLower();
    if (input.startsWith('-')) return Invalid; // имя хоста не может начинаться с "-"
    if (input.contains("-.") || input.contains(".-")) return Invalid; // label не может начинаться с или заканчиваться "-"
    if (input.contains("..")) return Invalid; // не может идти две точки подряд
    if (input.endsWith('-') || input.endsWith('.')) return Intermediate; // имя хоста не может заканчиваться на "-"
                                                              // только при статусе Intermediate будет вызываться метод fixup
    return Acceptable;
}


void NNCGValidDomname::fixup(QString &input) const {
    auto n = input.length();
    int cntHyph {0};
    if (n > 0) { // на всякий случай проверка длины
        do { // подсчёт запрещённых "-" в конце строки
            n--;
            if (input[n] == '-') cntHyph++;
            else break;
        } while (n > 0);
        input.chop(cntHyph); // удаление всех "-" в конце строки
        if (input.endsWith('.')) input.chop(1); // удаление послдней точки
    }
}


QValidator::State NNCGValidUnsigned::validate(QString &input, int &pos) const {
    for (auto idx = 0; idx < input.length(); idx++) {
        if (input[idx].unicode() > 57) return Invalid;
        if (input[idx].unicode() < 48) return Invalid;
    }
    if (input.length() > 10) return Invalid;
    if (input.toLongLong() > UINT32_MAX) input.setNum(UINT32_MAX);
    return Acceptable;
}


void NNCGValidUnsigned::fixup(QString &input) const {
}


bool isIntegratedIPv4Exists(QString &str) {


    return false;
}

QValidator::State NNCGValidIPv6::validate(QString &input, int &pos) const {
    //std::cout << "string : " << input.toStdString() << std::endl;
    bool overlap; // совпадение со списком разрешённых символов
    for (int idx = 0; idx < input.length(); idx++) {
        overlap = false;
        for (int alwd = 0; alwd < allowed.length(); alwd++) {
            if (input[idx] == allowed[alwd]) {
                overlap = true;
                break;
            }
        }
        if (!overlap) return Invalid; // не совпало со списком разрешённых символов
    }
    if (input.startsWith('.')) return Invalid; // не может начинаться с точки
    if (input.contains(":::")) return Invalid; // не может быть двоеточий 3 раза подряд
    if (input.contains(":.")) return Invalid; // не может быть сочетания ":."
    if (input.contains("..")) return Invalid; // не может быть двух точек подряд
    if (input.count("::") > 1) return Invalid; // нельзя использовать "::" более одного раза
    if (input.count('.') > 3) return Invalid; // не может быть более 3 точек всего
    if (!input.contains(':')) return Intermediate; // если вообще нет символа ":" => неполный ввод
    if (input.count(':') < 2) return Intermediate; // если всего символов ":" меньше двух => неполный ввод
    if (input.contains('.') && input.endsWith(':')) return Invalid;
    if (input.contains('.') && (!input.contains(':'))) return Invalid;
    QRegExp rex;
    rex.setPattern("[0-9A-Fa-f]{5,}:"); // больше 4 символов перед ":" => ошибка
    if (rex.indexIn(input) != -1) {
        std::cout << "error : too much symbols before \":\" ! " << std::endl;
        return Invalid;
    }
    rex.setPattern(":[0-9A-Fa-f]{5,}"); // больше 4 символов после ":" => ошибка
    if (rex.indexIn(input) != -1) {
        std::cout << "error : too much symbols after \":\" ! " << std::endl;
        return Invalid;
    }
    if (input.contains('.')) { // => ищем ошибки в интегрированном ipv4-адресе
        rex.setPattern(":[A-Fa-f0-9]{4,}\\."); // между : и . не должно быть более 3 символов
        if (rex.indexIn(input) != - 1) {
            std::cout << "error found : too much symbols between \":\" and \".\"" << std::endl;
            return Invalid;
        }
        rex.setPattern(":.?[A-Fa-f]+.?\\."); // между : и . не должно быть букв
        if (rex.indexIn(input) != - 1) {
            std::cout << "error found : letters between \":\" and \".\"" << std::endl;
            return Invalid;
        }
        rex.setPattern("\\..*[A-Fa-f]+.*"); // после . не должно быть букв
        if (rex.indexIn(input) != - 1) {
            std::cout << "error found : letters after \".\"" << std::endl;
            return Invalid;
        }
        rex.setPattern("\\.[0-9]{4,}"); // после . не должно быть более 3 символов
        if (rex.indexIn(input) != - 1) {
            std::cout << "error found : too much symbols after \".\"" << std::endl;
            return Invalid;
        }
    }
    if (input.endsWith('.')) return Intermediate;
    std::cout << "last checking for string : " << input.toStdString() << std::endl;
    // если здесь, значит в полном виде присутствует интегрированный ipv4; проверяем его октеты
    rex.setPattern("^(.*:)([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})$");
    if (rex.indexIn(input) != -1) { // нашли ipv6 + 4 октета ipv4
        std::cout << "integrated ipv4 in full form exists" << std::endl;
        input.clear(); // будем пересобирать строку заново
        input.append(rex.cap(1));
        for (int gr = 2; gr <= 5; gr++) {
            if (rex.cap(gr).toInt() > 255) input.append("255.");
            else input.append(rex.cap(gr) + '.');
        }
        input.chop(1); // откусываем последнюю точку
    }
    return Acceptable;
}


void NNCGValidIPv6::fixup(QString &input) const {
//    std::cout << "ipv6 fixup called" << std::endl;
    input = "::1";
}