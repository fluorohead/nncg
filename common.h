#ifndef COMMON_H
#define COMMON_H

#include <QString>

constexpr struct { int major; int minor; int micro; } APPVER = {0, 0, 2};
constexpr auto DISPLAY_APPVER = "v0.1.2 : GIA : 2024";

constexpr auto MIN_WIDTH = 800;
constexpr auto MIN_HEIGHT = 600;

constexpr auto MAX_TMPL_FILE_SIZE = 5000000; // ~5 MBytes
constexpr auto MAX_JSON_FILE_SIZE = 50000; // ~50 Kbytes
constexpr auto MAX_CSV_FILE_SIZE = 1000000; // ~1 MByte

constexpr auto MAX_VARIABLES = 1000;         // максимальное кол-во переменных в файле шаблона
constexpr auto MIN_TMPL_HEADER_LINES = 10;   // минимальное количество обязательных строк в заголовке шаблона
constexpr auto MAX_VAR_NAME_LEN = 100; // максимальная длина имени переменной (включая фигурные скобки)
constexpr auto MAX_VAR_DESCR_LEN = 100; // максимальная длина описания переменной (без учёта двойных кавычек)

constexpr auto TABLE_COLUMN_ZERO_FIXED_WIDTH = 32; // ширина нулевой колонки таблицы
constexpr auto BASE_WIDTH_COLUMN_DESCR = 324; // начальная ширина 1-й колонки таблицы
constexpr auto TABLE_COLUMN_ONE_MIN_WIDTH = 39;

// индексы языков
constexpr auto LANGS_AMOUNT = 2;
enum langId_t  {English = 0, Russian = 1, Elfian = LANGS_AMOUNT};

// типы переменных
enum varType_t {Domname = 0, Text, IPv4, Unsigned, Password, MASKv4, IPv6, MASKv6Len, WildcardV4, MASKv4Len, Prompt, Hash, WildcardV6, Separator, MAX};

// типы графических схем
enum themeId_t {Dark = 0, Light = 1, UnknownTheme};

struct oneRec_t {
    int         orderNum;
    QString     descr;
    QString     value;
    varType_t   type;
};

struct oneRecShort_t {
    QString     value;
    varType_t   type;
};

struct rgb {int r; int g; int b;};

struct theme_t {
    rgb  bw_bg; // big widget bg
    rgb  tbl_fg; //table fg
    rgb  tbl_gr; // table gridline
    rgb  tbl_bg; // table bg
    rgb  vh; // vetical scroll handle
    rgb  ttl_fg; // title fg
};


#endif // COMMON_H
