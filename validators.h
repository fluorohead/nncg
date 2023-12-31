#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <QValidator>
#include <QRegExp>

class NNCGValidIPv4: public QValidator
{
private:
    QRegExp rex;

public:
    NNCGValidIPv4(QObject * = nullptr);
    QValidator::State validate(QString &, int &) const;
    void fixup(QString &) const;
};


class NNCGValidMASKv4: public QValidator
{
private:
    QRegExp rex;

public:
    NNCGValidMASKv4(QObject * = nullptr);
    QValidator::State validate(QString &input, int &pos) const;
    void fixup(QString &) const;
};


class NNCGValidDomname: public QValidator
{
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


class NNCGValidHash: public QValidator
{
public:
    NNCGValidHash(QObject *parent = nullptr): QValidator(parent) {};
    QValidator::State validate(QString &input, int &pos) const;
};

#endif // VALIDATORS_H
