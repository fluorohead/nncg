#include "validators.h"
#include <iostream>

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
    std::cout << "current mask to inspect : " << input.toStdString() << "; cursor position : " << pos << std::endl;
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
