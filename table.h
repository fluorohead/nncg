#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include <QTableWidget>

const QString QS_TBLDESCR[LANGS_AMOUNT] {"Description", "Описание"};
const QString QS_TBLVALUE[LANGS_AMOUNT] {"Value", "Значение"};

class NNCGTable: public QTableWidget
{

public:
    void changeEvent(QEvent *);
};

#endif // TABLE_H
