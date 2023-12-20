#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "common.h"
#include <QFile>
#include <QString>
#include <QPixmap>

class NNCGTemplate: public QObject
{

private:
    QFile qFile;
    QString serChar; // service char / служебный символ / первый символ строки №[8]
    QPixmap pixLogo;
    bool inspectLine(const QString &, QString &, QString &, varType_t &);
    void inspectBrandColors();
    QString delimOpen  {"{"};
    QString delimClose {"}"};
    void extractDelimiters();
    QRegExp rex;

public:
    bool noOpenErr {false};
    bool isDemo {false};
    QString lastErrMsg;
    QHash<QString, oneRec_t> hashVars;
    QString getTitle();
    QString getComment();
    QString getFilePath(); // возвр. только путь, без имени файла
    QString getFileName(); // возвр. только имя файла, без пути
    QPixmap* getPtrPixLogo(); // возвр. указатель на logo pixmap
    QString getDelimOpen(); // возвр. начальный ограничитель
    QString getDelimClose(); // возвр. конечный ограничитель
    int beginConfig {0}; // начало конфига в списке strList
    QStringList strList;
    int brandColors[6] {0, 139, 224, 255, 255, 255};
    NNCGTemplate(); // для demo-шаблона
    NNCGTemplate(const QString &);

};

#endif // TEMPLATE_H
