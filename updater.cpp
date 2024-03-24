#include "updater.h"
#include "common.h"
#include "settings.h"
#include <QStandardPaths>
#include <QProcess>

extern NNCGSettings objSett;
extern const array<QString, LANGS_AMOUNT> QS_UPGRADE;

extern QString upgradeFilePath;
extern QString lastVerStr;

Updater::Updater(): naman(this), state(en_state::Idle) {
};

Updater::~Updater() {
    state = en_state::Idle;
    if (ptr_reply != nullptr) delete ptr_reply;
};

bool Updater::need_update(QString &str_ver) {
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

void Updater::make_request(NNCGMainWindow *mw_ptr) {
    state = en_state::Working;
    nreq.setUrl(QUrl("https://raw.githubusercontent.com/fluorohead/nncg/main/last_ver.txt")); // скачиваем текстовый файл с последней версией
    ptr_reply = naman.get(nreq);
    while (!ptr_reply->isFinished())
        QApplication::processEvents(QEventLoop::WaitForMoreEvents, 250);
    if (!ptr_reply->error()) {
        lastVerStr = ptr_reply->readLine();
        QStringList tmp_paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        if (need_update(lastVerStr) && (tmp_paths.length() > 0)) {
            QString fn = "nncg-" + lastVerStr + "-windows-x64.exe";
            QString fp = tmp_paths[0] + "/" + fn;
            QFile new_file(fp);
            bool hash_ok {false};
            bool run_installer {false};
            if (new_file.exists()) { // сначала проверяем не скачан ли уже этот файл
                qInfo() << "New installer detected in tmp dir!";
                new_file.open(QIODevice::ReadOnly);
                qInfo() << "New installer file opened to calculate MD5 hash.";
                QCryptographicHash crh(QCryptographicHash::Md5);
                if (crh.addData(&new_file)) {
                    qInfo() << "Calculating hash...";
                    qInfo() << "Calculated : " << crh.result().toHex();
                    QString new_ver_hash = ptr_reply->readLine();
                    qInfo() << "Downloaded hash : " << new_ver_hash;
                    if (crh.result().toHex() == new_ver_hash) {
                        hash_ok = true;
                        run_installer = true;
                        qInfo() << "Hash is equal, so no need to download installer again.";
                    }
                }
                new_file.close();
            }
            if (!hash_ok) { // используем переменную, как индикатор уже скачанного инсталлятора в tmp
                nreq.setUrl(QUrl("https://github.com/fluorohead/nncg/releases/download/" + lastVerStr + "/" + fn)); // скачиваем инсталлятор последней версии
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
                ptr_reply->deleteLater();
            } else {
                //qInfo() << "New version installer already downloaded and present in tmp dir.";
            }
            if (objSett.autoUpgrade && run_installer) { // отображаем кнопку апгрейда
                upgradeFilePath = fp;
                mw_ptr->btnUpgrade->setText(QS_UPGRADE.at(objSett.curLang).arg(lastVerStr));
                connect(this, SIGNAL(upgrade_btn_show()), mw_ptr->btnUpgrade, SLOT(show_on_signal()), Qt::AutoConnection);
                emit upgrade_btn_show();
            }
        }
    } else {
        ptr_reply->deleteLater();
    }
    state = en_state::Finished;
}
