#include "updater.h"
#include "common.h"
#include <QStandardPaths>

Updater::Updater(): naman(this), state(en_state::Idle) {
};

Updater::~Updater() {
    state = en_state::Idle;
    if (ptr_reply != nullptr) delete ptr_reply;
};

bool Updater::can_update(QString &str_ver){
    QStringList qsl = str_ver.split('.');
    if (qsl.length() != 3) return false;
    bool ok;
    int new_ver[3];
    for (int idx = 0; idx < 3; idx++) {
        new_ver[idx] = qsl[idx].toInt(&ok, 10);
        if (!ok) return false;
    }
    if (new_ver[0] > APPVER.major) return true;
    if ((new_ver[0] == APPVER.major) && (new_ver[1] > APPVER.minor)) return true;
    if ((new_ver[0] == APPVER.major) && (new_ver[1] == APPVER.minor) && (new_ver[2] > APPVER.micro)) return true;
    return false;
}

void Updater::make_request() {
    state = en_state::Working;
    nreq.setUrl(QUrl("https://raw.githubusercontent.com/fluorohead/nncg/main/last_ver.txt")); // скачиваем текстовый файл с последней версией
    ptr_reply = naman.get(nreq);
    while (!ptr_reply->isFinished())
        QApplication::processEvents(QEventLoop::WaitForMoreEvents, 250);
    if (!ptr_reply->error()) {
        QString last_ver = ptr_reply->readAll();
        QStringList tmp_paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        if (can_update(last_ver) && (tmp_paths.length() > 0)) {
            QString fn = "nncg-" + last_ver + "-windows-x64.exe";
            QString fp = tmp_paths[0] + "/" + fn;
            QFile new_file(fp);
            bool hash_ok {false};
            if (new_file.exists()) { // сначала проверяем не скачан ли уже этот файл
                qInfo() << "New installer detected in tmp dir!";
                nreq.setUrl(QUrl("https://raw.githubusercontent.com/fluorohead/nncg/main/last_ver.md5")); // для этого узнаем какой у последней версии должен быть MD5-хэш
                delete ptr_reply;
                ptr_reply = naman.get(nreq);
                while (!ptr_reply->isFinished())
                    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 250);
                qInfo() << "File last_ver.md5 downloaded.";
                if (!ptr_reply->error()) {
                    new_file.open(QIODevice::ReadOnly);
                    qInfo() << "New installer file opened to calculate MD5 hash";
                    QCryptographicHash crh(QCryptographicHash::Md5);
                    if (crh.addData(&new_file)) {
                        if (crh.result().toHex() == ptr_reply->readAll().toHex()) {
                            hash_ok = true;
                        }
                    }
                }
            }
            if (!hash_ok) { // используем эту переменную, как индикатор наличия скачанного инсталлятора
                nreq.setUrl(QUrl("https://github.com/fluorohead/nncg/releases/download/" + last_ver + "/" + fn)); // скачиваем инсталлятор последней версии
                delete ptr_reply;
                ptr_reply = naman.get(nreq);
                while (!ptr_reply->isFinished())
                    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 250);
                if (!ptr_reply->error()) {
                    if (new_file.open(QIODevice::WriteOnly)) {
                        new_file.write(ptr_reply->readAll());
                        new_file.flush();
                        new_file.close();
                    }
                }
            } else {
                qInfo() << "new version installer already downloaded and present in tmp dir";
            }

        }
    }
    state = en_state::Finished;
}
