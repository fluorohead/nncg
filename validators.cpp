#include "validators.h"

using namespace std;

extern QString b2s(bool b);

const QString QS_REXIPV4 {R"(^([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})$)"};
const QString allowedInIPv6 {"0123456789:.ABCDEFabcdef"};
const QString forbidInDomname {"'\"`!@#$%^&*()+=\\/{}[],;<>?:_"};

bool simpleIPv4Check(QString &s) { // check for common ipv4-address format requirements
    // разрешены только цифры и точки
    for (auto && unoChar : s) {
        if (unoChar.unicode() > 57) return false;
        if ((unoChar.unicode() < 48) && (unoChar.unicode() != 46)) return false;
    }
    // проверка на символы прошла успешно.
    // ip не может начинаться с точки, не может быть двух точек подряд, не может быть более 3 точек всего
    return !(s.startsWith('.') || s.contains("..") || (s.count('.') > 3));
}


// проверяет октет маски на соответствие одному из 9 предопределённых значений
bool isCorrectMASKv4_Octet(int octet) {
    const array<int, 9> rightValues {255, 254, 252, 248, 240, 224, 192, 128, 0};
    for (int m = 0; m <= 8; m++) {
        if (octet == rightValues.at(m)) return true;
    }
    return false;
}


// проверяет на принадлежность к непрерывному диапазону (включая сами границы)
bool inRange(QString &s, int lowLimit, int highLimit) {
    for (auto && unoChar : s) {
        if ((unoChar.unicode() < lowLimit) || (unoChar.unicode() > highLimit)) return false;
    }
    return true;
}


// проверяем на цифровые символы
bool onlyDigits(QString &s){
    return inRange(s, 48, 57);
}


NNCGValidIPv4::NNCGValidIPv4(QObject *parent):QValidator(parent) {
    rex.setPattern(QS_REXIPV4);
};


QValidator::State NNCGValidIPv4::validate(QString &input, int &pos) const {
    if (simpleIPv4Check(input)) {
        auto rexMatch = rex.match(input);
        if (rexMatch.hasMatch()) { // проверка на regexp
            // все октеты на месте, далее проверяем каждый
            input.clear(); // чистим строку, будем пересобирать её заново
            for (auto idx = 1; idx <= 4; idx++) {
                int octet;
                octet = rexMatch.captured(idx).toUInt();
                if (octet > 255) octet = 255;
                input.append(QString::number(octet) + '.');
            }
            input.chop(1); // откусываем лишнюю точку в конце
        } else { // не прошли проверку на regexp, значит незавершённый ввод
            return Intermediate;
        }
    } else {
        return Invalid;
    }
    return Acceptable;
}


void NNCGValidIPv4::fixup(QString &input) const {
    input = "0.0.0.0";
}


NNCGValidMASKv4::NNCGValidMASKv4(QObject *parent): QValidator(parent) {
    rex.setPattern(QS_REXIPV4);
};


QValidator::State NNCGValidMASKv4::validate(QString &input, int &pos) const {
    if (!simpleIPv4Check(input)) {
        auto rexMatch = rex.match(input);
        if (rexMatch.hasMatch()) { // проверка на regexp
            // все октеты на месте, далее проверяем каждый
            input.clear(); // чистим строку, будем пересобирать её заново
            array<int, 4> bitsArr {};
            for (int idx = 1; idx <= 4; idx++) {
                int octet;
                octet = rexMatch.captured(idx).toUInt();
                if (octet > 255) octet = 255;
                input.append(QString::number(octet) + '.');
                bitsArr.at(idx-1) = octet;
            }
            input.chop(1); // откусываем лишнюю точку в конце
            for (int ar = 0; ar <= 3; ar++){ // проверяем каждый октет : могут иметь только одно из 9 определённых значений
                if (!isCorrectMASKv4_Octet(bitsArr.at(ar))) return Intermediate;
            }
            if (!(bitsArr.at(0) >= bitsArr.at(1)) || !(bitsArr.at(1) >= bitsArr.at(2)) || !(bitsArr.at(2) >= bitsArr.at(3))) return Intermediate;
        } else { // не прошли проверку на regexp, значит незавершённый ввод
            return Intermediate;
        }
    } else {
        return Invalid;
    }
    return Acceptable;
}


void NNCGValidMASKv4::fixup(QString &input) const {
    input = "255.255.255.255";
}


QValidator::State NNCGValidDomname::validate(QString &input, int &pos) const {
    for (auto && unoChar : input) {
        for (auto && forbChar : forbidInDomname) {
            if (unoChar == forbChar) return Invalid;
        }
        auto c = unoChar.unicode();
        if ((c < 45) || (c > 122)) return Invalid;
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
        if (input.endsWith('.')) input.chop(1); // удаление последней точки
    }
}


QValidator::State NNCGValidUnsigned::validate(QString &input, int &pos) const {
    if ((!onlyDigits(input)) || input.startsWith("0")) return Invalid; // не может начинаться с нуля
    if (input.toLongLong() > UINT32_MAX) input.setNum(UINT32_MAX);
    return Acceptable;
}


QValidator::State NNCGValidIPv6::validate(QString &input, int &pos) const {
    bool overlap; // совпадение со списком разрешённых символов
    for (auto && unoChar : input) {
        overlap = false;
        for (auto && alwd : allowedInIPv6) {
            if (unoChar == alwd) {
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
    if ((input.count(':') < 7) && (!input.contains("::"))) return Intermediate;
    if (input.count(':') > 7) return Invalid;
    if (input.contains('.') && input.endsWith(':')) return Invalid;
    if (input.contains('.') && (!input.contains(':'))) return Invalid;
    QRegularExpression rex;
    rex.setPattern("[0-9A-Fa-f]{5,}:"); // больше 4 символов перед ":" => ошибка
    auto rexMatch = rex.match(input);
    if (rexMatch.hasMatch()) {
        return Invalid;
    }
    rex.setPattern(":[0-9A-Fa-f]{5,}"); // больше 4 символов после ":" => ошибка
    rexMatch = rex.match(input);
    if (rexMatch.hasMatch()) {
        return Invalid;
    }
    if (input.contains('.')) { // => ищем ошибки в интегрированном ipv4-адресе
        rex.setPattern(":[A-Fa-f0-9]{4,}\\."); // между : и . не должно быть более 3 символов
        rexMatch = rex.match(input);
        if (rexMatch.hasMatch()) {
            return Invalid;
        }
        rex.setPattern(":.?[A-Fa-f]+.?\\."); // между : и . не должно быть букв
        rexMatch = rex.match(input);
        if (rexMatch.hasMatch()) {
            return Invalid;
        }
        rex.setPattern("\\..*[A-Fa-f]+.*"); // после . не должно быть букв
        rexMatch = rex.match(input);
        if (rexMatch.hasMatch()) {
            return Invalid;
        }
        rex.setPattern("\\.[0-9]{4,}"); // после . не должно быть более 3 символов
        rexMatch = rex.match(input);
        if (rexMatch.hasMatch()) {
            return Invalid;
        }
    }
    if (input.endsWith('.')) return Intermediate;
    // если здесь, значит в полном виде присутствует интегрированный ipv4; проверяем его октеты
    rex.setPattern(R"(^(.*:)([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})$)");
    rexMatch = rex.match(input);
    if (rexMatch.hasMatch()) { // нашли ipv6 + 4 октета ipv4
        input.clear(); // будем пересобирать строку заново
        input.append(rexMatch.captured(1));
        for (int gr = 2; gr <= 5; gr++) {
            if (rexMatch.captured(gr).toUInt() > 255) input.append("255.");
            else input.append(rexMatch.captured(gr) + '.');
        }
        input.chop(1); // откусываем последнюю точку
    }
    return Acceptable;
}


void NNCGValidIPv6::fixup(QString &input) const {
    input = "::1";
}


QValidator::State NNCGValidMASKv4Len::validate(QString &input, int &pos) const {
    if (!onlyDigits(input)) return Invalid; // д.б. только цифры
    if (input.toUInt() > 32) input.setNum(32); // макс значение = 32
    if (input.startsWith("0") && (input.length() > 1)) return Invalid; // может быть = 0, но не может быть 01, 02 и т.д.
    return Acceptable;
}


QValidator::State NNCGValidMASKv6Len::validate(QString &input, int &pos) const {
    if (!onlyDigits(input)) return Invalid; // д.б. только цифры
    if (input.toUInt() > 128) input.setNum(128); // макс значение = 128
    if (input.startsWith("0") && (input.length() > 1)) return Invalid; // может быть = 0, но не может быть 01, 02 и т.д.
    return Acceptable;
}


QValidator::State NNCGValidHash::validate(QString &input, int &pos) const {
    if (!inRange(input, 33, 126)) return Invalid;
    return Acceptable;
}
