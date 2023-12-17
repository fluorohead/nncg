#ifndef VALIDATORS_H
#define VALIDATORS_H
#include "common.h"
#include <QValidator>

class NNCGValidIPv4: public QValidator
{
private:
    QRegExp rex {QS_REXIPV4};

public:
    NNCGValidIPv4(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &, int &) const;
    void fixup(QString &) const;
};


class NNCGValidMASKv4: public QValidator
{
private:
    QRegExp rex {QS_REXIPV4};

public:
    NNCGValidMASKv4(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &input, int &pos) const;
    void fixup(QString &) const;
};


class NNCGValidDomname: public QValidator
{
private:
    const QString forbidden {"'\"`!@#$%^&*()+=\\/{}[],;<>?:"};

public:
    NNCGValidDomname(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &, int &) const;
    void fixup(QString &) const;
};


class NNCGValidUnsigned: public QValidator
{
public:
    NNCGValidUnsigned(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &, int &) const;
};


class NNCGValidIPv6: public QValidator
{
private:
    const QString allowed {"0123456789:.ABCDEFabcdef"};

public:
    NNCGValidIPv6(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &, int &) const;
    void fixup(QString &) const;
};


class NNCGValidMASKv4Len: public QValidator
{
public:
    NNCGValidMASKv4Len(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &input, int &pos) const;
};


class NNCGValidMASKv6Len: public QValidator
{
public:
    NNCGValidMASKv6Len(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &input, int &pos) const;
};

#endif // VALIDATORS_H
