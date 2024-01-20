#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "QRegularExpression"
#include "common.h"
#include <QFile>
#include <QString>
#include <QPixmap>

using namespace std;

class NNCGTemplate {

    QFile qFile;
    QString serChar; // service char / служебный символ / первый символ строки №[8]
    QPixmap pixLogo;
    QString delimOpen  {R"(\{)"};
    QString delimClose {R"(\})"};
    QRegularExpression rex;

    bool inspectLine(const QString &, QString &, QString &, varType_t &);
    void extractDelimiters();
    void inspectBrandColors();
    void escapingCtrlSymbols(QString &);

public:
    NNCGTemplate(); // для demo-шаблона
    NNCGTemplate(const QString &);

    bool noOpenErr {false};
    bool isDemo {false};
    int beginConfig {0}; // начало конфига в списке strList
    QString lastErrMsg;
    QStringList strList;
    array<int, 6> brandColors {0, 139, 224, 255, 255, 255};
    QHash<QString, oneRec_t> hashVars;

    QString getTitle();
    QString getComment();
    QString getFilePath(); // возвр. только путь, без имени файла
    QString getFileName(); // возвр. только имя файла, без пути
    QPixmap* getPtrPixLogo(); // возвр. указатель на logo pixmap
    QString getDelimOpen(); // возвр. начальный ограничитель
    QString getDelimClose(); // возвр. конечный ограничитель
};

#endif // TEMPLATE_H
