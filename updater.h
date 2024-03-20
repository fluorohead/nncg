#ifndef UPDATER_H
#define UPDATER_H

#include "mwindow.h"
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>

using namespace std;

class Updater: public QObject {
    Q_OBJECT
    QNetworkAccessManager naman;
    QNetworkRequest nreq;
    QNetworkReply *ptr_reply {nullptr};
    enum class en_state {Init = 0, Idle = 1, Working = 2, Finished = 3};
    bool need_update(QString &str_ver);
    en_state state {en_state::Init};

public:
    Updater();
    ~Updater();
    void make_request(NNCGMainWindow *mw_ptr);
    en_state get_state() { return state; };
signals:
    void upgrade_btn_show();
};

#endif // UPDATER_H
