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
            nreq.setUrl(QUrl("https://github.com/fluorohead/nncg/releases/download/" + last_ver + "/" + fn)); // скачиваем инсталлятор последней версии
            delete ptr_reply;
            ptr_reply = naman.get(nreq);
            while (!ptr_reply->isFinished())
                QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 250);
            if (!ptr_reply->error()) {
                QFile new_file(tmp_paths[0] + "/" + fn);
                if (new_file.open(QIODevice::WriteOnly)) {
                    new_file.write(ptr_reply->readAll());
                    new_file.flush();
                    new_file.close();
                }
            }
        }
    }
    state = en_state::Finished;
}
