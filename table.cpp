#include "table.h"
#include "settings.h"
#include "template.h"

#include <QEvent>
#include <QLineEdit>
#include <QHeaderView>
#include <QScrollBar>
#include <QCommonStyle>

using namespace std;

extern NNCGSettings objSett;
extern NNCGTemplate *objTempl;
extern array<array<QString, LANGS_AMOUNT>, int(varType_t::MAX)> QS_PLCHLDRS;

extern const array<QString, LANGS_AMOUNT> QS_TBLDESCR {"Description", "Описание"};
extern const array<QString, LANGS_AMOUNT> QS_TBLVALUE {"Value", "Значение"};

void NNCGTable::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) { // переводим текст своих дочерних элементов
        this->setHorizontalHeaderLabels({"#", QS_TBLDESCR.at(objSett.curLang), QS_TBLVALUE.at(objSett.curLang)});
        for (auto hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
            if (hIt.value().type != varType_t::Separator) {
                auto *cw = static_cast<QLineEdit*>(this->cellWidget(hIt.value().orderNum, 2));
                if (!QS_PLCHLDRS.at(hIt.value().type).at(1).isEmpty()) cw->setPlaceholderText(QS_PLCHLDRS.at(hIt.value().type).at(objSett.curLang));
                else cw->setPlaceholderText(QS_PLCHLDRS.at(hIt.value().type).at(0));
            }
        }
    }
  //  QTableWidget::changeEvent(event);
  //  event->accept();
}


