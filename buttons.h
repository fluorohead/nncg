#ifndef BUTTONS_H
#define BUTTONS_H

#include "common.h"

#include <QPushButton>
#include <QEvent>

class NNCGBtnCsvLoad: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnCsvLoad(int, int, const QString &, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};


class NNCGBtnCsvSave: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnCsvSave(int, int, const QString &, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};


class NNCGButtonLoad: public QPushButton
{
    Q_OBJECT

public:
    NNCGButtonLoad(int, int, const QString &, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};


class NNCGButtonCreate: public QPushButton
{
    Q_OBJECT

public:
    NNCGButtonCreate(int, int, const QString &, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};


class NNCGBtnClearAll: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnClearAll(int, int, const QString &, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};


class NNCGBtnLangSwitch: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnLangSwitch(int, int, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};


class NNCGBtnThemeSwitch: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnThemeSwitch(int, int, QWidget * = nullptr);
    void changeEvent(QEvent *);

public slots:
    void slotClicked();
};

#endif // BUTTONS_H
