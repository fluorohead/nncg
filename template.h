#ifndef NNCGTEMPLATE_H
#define NNCGTEMPLATE_H

#include "common.h"

class NNCGTemplate
{
private:
    QFile qFile;
    QString serChar; // service char / служебный символ / первый символ строки №[4]
    QPixmap pixLogo;
    bool inspectLine(const QString&, QString&, QString&, varType_t&);

public:
    bool noOpenErr {false};
    bool isDemo {false};
    QString lastErrMsg;
    QHash<QString, oneRec_t> hashVars;
    QString getTitle();
    QString getComment();
    QString getFilePath(); // только путь, без имени файла
    QString getFileName(); // только имя файла, без пути
    QPixmap* getPtrPixLogo(); // указатель на logo pixmap
    int beginConfig {0}; // индекс начала самого конфига в strList
    QStringList strList;
    NNCGTemplate(); // для demo-шаблона
    NNCGTemplate(const QString&);
};

#endif // NNCGTEMPLATE_H
