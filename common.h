#ifndef ALLCONSTS_H
#define ALLCONSTS_H

//#include <iostream>
#include <QApplication>
#include <QString>
#include <QFileInfo>
#include <QMessageBox>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QColor>
#include <QCloseEvent>
#include <QFont>
#include <QFileDialog>
#include <QHash>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QPixmap>
#include <QDir>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QScrollBar>
#include <QList>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600

#define MAX_TMPL_FILE_SIZE 5000000 // ~5 MBytes
#define MAX_JSON_FILE_SIZE 50000 // ~50 Kbytes
#define MAX_CSV_FILE_SIZE 1000000 // ~1 MByte

#define MAX_VARIABLES 1000         // максимальное кол-во переменных в файле шаблона
#define MIN_TMPL_HEADER_LINES 8   // минимальное количество строк в заголовке шаблона
#define MAX_VAR_NAME_LEN 100 // максимальная длина имени переменной (включая фигурные скобки)
#define MAX_VAR_DESCR_LEN 100 // максимальная длина описания переменной (без учёта двойных кавычек)

#define BASE_WIDTH_COLUMN_DESCR 324

const QString   QS_WIDTH = "width",
                QS_HEIGHT = "height",
                QS_DARK = "dark",
                QS_LIGHT = "light",
                QS_TEMPLATE = "template",
                QS_THEME = "theme",
                QS_MXMZD = "maximized",
                QS_CSV = "csv",
                QS_VVT = "\"variable\";\"value\";\"type\"",
                QS_COLWIDTH = "col_width",
                QS_REXIPV4 = "^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})$";

const QString   QS_VARTYPES[] {"domname", "description", "ipv4", "unsigned", "password", "maskv4", "ipv6", "maskv6len", "wildcardv4", "maskv4len", "prompt", "hash"};

enum varType_t       {Domname = 0, Description, IPv4, Unsigned, Password, MASKv4, IPv6, MASKv6Len, WildcardV4, MASKv4Len, Prompt, Hash, MAX};
const int maxChars[] {        253,         255,   15,       10,      128,     15,   45,         4,         15,         2,     64,  128};

struct oneRec_t{
    int orderNum;
    QString descr;
    QString value;
    varType_t type;
};

struct oneRecShort_t{
    QString value;
    varType_t type;
};

struct theme_t {
    QColor  bw_bg;
    QColor  bw_fg;
};

#endif // ALLCONSTS_H
