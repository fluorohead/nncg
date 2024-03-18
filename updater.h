#ifndef UPDATER_H
#define UPDATER_H

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
    bool can_update(QString &str_ver);
    en_state state {en_state::Init};

public:
    Updater();
    ~Updater();
    void make_request();
    en_state get_state() { return state; };
};

#endif // UPDATER_H
