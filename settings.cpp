#include "settings.h"

NNCGSettings::NNCGSettings() {
    QDir qDir;
    QString strDir {QDir::homePath() + "/.nncg/"};
    bool ok {true};
    if (!qDir.exists(strDir)) ok = qDir.mkdir(strDir);
    if (ok) {
//        std::cout << "no error while creating or opening /.nncg/ in home dir" << std::endl;
        qFile.setFileName(strDir + "nncg.json");
        if (!qFile.exists()) {
//            std::cout << "nncg.json doesn't exist" << std::endl;
            ok = qFile.open(QIODevice::ReadWrite | QIODevice::NewOnly); // создаём файл
        } else {
//            std::cout << "nncg.json exists" << std::endl;
            ok = qFile.open(QIODevice::ReadWrite); // либо открываем существующий
        }
        if (ok) {
            if (qFile.size() <= MAX_JSON_FILE_SIZE) {
                QByteArray qBA(qFile.readAll());
//                std::cout << "readed bytes : " << qBA.size() << std::endl;
                QJsonParseError jsErr {};
                jsDoc = QJsonDocument::fromJson(qBA, &jsErr);
                if (!jsDoc.isNull()) {
                    width = jsDoc[QS_WIDTH].toInt(); height = jsDoc[QS_HEIGHT].toInt();
                        ///
                        /// сделать проверку на соответствие текущему разрешению основного монитора
                        ///
                    templFpFn = jsDoc[QS_TEMPLATE].toString();
                    themeName = jsDoc[QS_THEME].toString().toLower();
                    if ((themeName != QS_DARK) && (themeName != QS_LIGHT)) themeName = QS_DARK;
                    maximized = jsDoc[QS_MXMZD].toBool();
/*                    std::cout << "width:" << width << std::endl;
                    std::cout << "height:" << height << std::endl;
                    std::cout << "theme:" << themeName.toStdString() << std::endl;
                    std::cout << "template:" << templFpFn.toStdString() << std::endl;
                    std::cout << "maximized:" << maximized << std::endl;*/
                    noLastErr = true;
                    lastErrMsg = QObject::tr("succesfully parsed nncg.json");
                } else {
                    noLastErr = false;
                    lastErrMsg = jsErr.errorString();
                }
            } else {
                noLastErr = false;
                lastErrMsg = QObject::tr("too big nncg.json, skipped");
            }
        } else { // ошибка создания или открытия файла
            noLastErr = false;
            lastErrMsg = QObject::tr("error creating or opening /.nncg/ in home dir");
        }
    } else { // ошибка создания или открытия подкаталога /.nncg/
        noLastErr = false;
        lastErrMsg = QObject::tr("error creating or opening /.nncg/ in home dir");
    }
}

bool NNCGSettings::saveSettings(const QString& fpfn, const QString& theme, int w, int h, const QString& mxmzd) {
    qFile.resize(0);
    QString writeStr =  QString("{\r\n"
                                " \"%1\": \"%2\",\r\n"
                                " \"%3\": \"%4\",\r\n"
                                " \"%5\": %6,\r\n"
                                " \"%7\": %8,\r\n"
                                " \"%9\": %10\r\n"
                                "}").arg(QS_TEMPLATE, fpfn, QS_THEME, theme, QS_WIDTH, QString::number(w), QS_HEIGHT, QString::number(h), QS_MXMZD, mxmzd);
//    std::cout << writeStr.toStdString() << std::endl;
    qFile.write(writeStr.toUtf8());
    qFile.flush();
    return true;
}
