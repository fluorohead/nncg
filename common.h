#ifndef COMMON_H
#define COMMON_H

#include <QColor>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600

#define MAX_TMPL_FILE_SIZE 5000000 // ~5 MBytes
#define MAX_JSON_FILE_SIZE 50000 // ~50 Kbytes
#define MAX_CSV_FILE_SIZE 1000000 // ~1 MByte

#define MAX_VARIABLES 1000         // максимальное кол-во переменных в файле шаблона
#define MIN_TMPL_HEADER_LINES 10   // минимальное количество строк в заголовке шаблона
#define MAX_VAR_NAME_LEN 100 // максимальная длина имени переменной (включая фигурные скобки)
#define MAX_VAR_DESCR_LEN 100 // максимальная длина описания переменной (без учёта двойных кавычек)

#define BASE_WIDTH_COLUMN_DESCR 324

// индексы языков
#define LANGS_AMOUNT 2
enum langId_t  {English = 0, Russian = 1, Elfian = LANGS_AMOUNT};

// типы переменных
enum varType_t {Domname = 0, Text, IPv4, Unsigned, Password, MASKv4, IPv6, MASKv6Len, WildcardV4, MASKv4Len, Prompt, Hash, MAX};

struct oneRec_t {
    int orderNum;
    QString descr;
    QString value;
    varType_t type;
};

struct oneRecShort_t {
    QString value;
    varType_t type;
};

struct theme_t {
    QColor  bw_bg;
    QColor  bw_fg;
};

#endif // COMMON_H
