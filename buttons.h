#ifndef BUTTONS_H
#define BUTTONS_H

#include "common.h"

class NNCGBtnCsvLoad: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnCsvLoad(int, int, const QString&, QWidget* = nullptr);

public slots:
    void slotClicked();
};


class NNCGBtnCsvSave: public QPushButton
{
    Q_OBJECT

public:
    NNCGBtnCsvSave(int, int, const QString&, QWidget* = nullptr);

public slots:
    void slotClicked();
};


class NNCGButtonLoad: public QPushButton
{
    Q_OBJECT

public:
    NNCGButtonLoad(int, int, const QString&, QWidget* = nullptr);

public slots:
    void slotClicked();
};


class NNCGButtonCreate: public QPushButton
{
    Q_OBJECT

public:
    NNCGButtonCreate(int, int, const QString&, QWidget* = nullptr);

public slots:
    void slotClicked();

};


#endif // BUTTONS_H
