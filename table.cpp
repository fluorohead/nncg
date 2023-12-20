#include "table.h"
#include "settings.h"
#include "template.h"

#include <QEvent>
#include <QLineEdit>

#include <iostream>

extern NNCGSettings objSett;
extern NNCGTemplate *objTempl;
extern QString QS_PLCHLDRS[][LANGS_AMOUNT];

void NNCGTable::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) { // переводим текст своих дочерних элементов
        this->setHorizontalHeaderLabels({"#", QS_TBLDESCR[objSett.curLang], QS_TBLVALUE[objSett.curLang]});
        for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
            QLineEdit *cw = (QLineEdit*) this->cellWidget(hIt.value().orderNum, 2);
            if (!QS_PLCHLDRS[hIt.value().type][1].isEmpty()) cw->setPlaceholderText(QS_PLCHLDRS[hIt.value().type][objSett.curLang]);
            else cw->setPlaceholderText(QS_PLCHLDRS[hIt.value().type][0]);
        }
    } else QTableWidget::changeEvent(event);
}


