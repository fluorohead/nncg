#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include <QTableWidget>

class NNCGTable: public QTableWidget
{

public:
    void changeEvent(QEvent *);
};

#endif // TABLE_H
