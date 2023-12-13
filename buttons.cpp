#include "buttons.h"
#include "mwindow.h"
#include "csv.h"
#include "template.h"

extern NNCGMainWindow *mainWindow;
extern NNCG_csv *objCSV;
extern NNCGTemplate *objTempl;
extern QString t2s(varType_t type);

NNCGBtnCsvLoad::NNCGBtnCsvLoad(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGBtnCsvLoad::slotClicked() {
    QString newFN = QFileDialog::getOpenFileName(this, QObject::tr("Opening csv"), "./", QObject::tr("CSV files (*.csv)"));
    if (!newFN.isEmpty()) {
        /// потом выделить в отдельный поток
        auto *newCSV = new NNCG_csv(newFN);
        if (newCSV->noOpenErr) { // с новым csv всё ок, тогда удаляем старый объект и подменяем указателем на новый
            delete objCSV;
            objCSV = newCSV;
            for (QHash<QString, oneRecShort_t>::iterator hIt = objCSV->hashVars.begin(); hIt != objCSV->hashVars.end(); ++hIt) {
                if (objTempl->hashVars.contains(hIt.key()) ) { // загружаем значения из csv в template
                    if (objTempl->hashVars[hIt.key()].type == hIt.value().type) { // проверка на соответствие типа переменной из шаблона и csv
                        objTempl->hashVars[hIt.key()].value = hIt.value().value;
                    }
                }
            };
            mainWindow->refreshTable();
        } else { // ошибка при открытии файла или некорректные данные в заголовке
            QMessageBox(QMessageBox::Critical, QObject::tr("Error!"), newCSV->lastErrMsg, QMessageBox::Ok, mainWindow).exec();
            delete newCSV;
        }
        ///
    }
}


NNCGBtnCsvSave::NNCGBtnCsvSave(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGBtnCsvSave::slotClicked() {
    QString saveFpFn = QFileDialog::getSaveFileName(this, QObject::tr("Saving csv"), "./", QObject::tr("CSV files (*.csv)"));
    if (!saveFpFn.isEmpty()) {
        /// потом выделить в отдельный поток
        QFile fileCSV(saveFpFn);
        if (fileCSV.open(QIODevice::WriteOnly)) {
            QByteArray ba;
            mainWindow->dumpTableToHash();
            ba.append(QS_VVT);
            for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
                  ba.append(QString("\r\n\"%1\";\"%2\";\"%3\"").arg(hIt.key(), hIt.value().value, t2s(hIt.value().type)));
            }
            if (fileCSV.write(ba)) {
                QMessageBox(QMessageBox::Information, QObject::tr("Info"), QObject::tr("csv saved successfully"), QMessageBox::Ok, mainWindow).exec();
                fileCSV.flush();
            } else {
                QMessageBox(QMessageBox::Critical, QObject::tr("Error!"), QObject::tr("error writing csv"), QMessageBox::Ok, mainWindow).exec();
            }
            fileCSV.close();
        } else { // ошибка при создании файла
            QMessageBox(QMessageBox::Critical, QObject::tr("Error!"), QObject::tr("error saving csv"), QMessageBox::Ok, mainWindow).exec();
        }
        ///
    }
}


NNCGButtonLoad::NNCGButtonLoad(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGButtonLoad::slotClicked() {
    QString newFN = QFileDialog::getOpenFileName(this, QObject::tr("Opening template"), "./",  QObject::tr("Text files (*.txt)"));
    if (!newFN.isEmpty()) {
        /// потом выделить в отдельный поток
        auto *newTempl = new NNCGTemplate(newFN);
        ///
        if (newTempl->noOpenErr) { // с новым шаблоном всё ок, тогда удаляем старый объект и подменяем указателем на новый
            delete objTempl;
            objTempl = newTempl;
            mainWindow->refreshTable();
            mainWindow->btnCfgCreate->setEnabled(true);
            mainWindow->btnCsvLoad->setEnabled(true);
            mainWindow->btnCsvSave->setEnabled(true);
        } else { // ошибка при открытии файла или некорректные данные в заголовке
            QMessageBox(QMessageBox::Critical, QObject::tr("Error!"), newTempl->lastErrMsg, QMessageBox::Ok, mainWindow).exec();
            delete newTempl;
        }
    }
}


NNCGButtonCreate::NNCGButtonCreate(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGButtonCreate::slotClicked(){
    QString saveFpFn = QFileDialog::getSaveFileName(this, QObject::tr("Saving config"), "./", QObject::tr("CFG files (*.cfg)"));
    if (!saveFpFn.isEmpty()) {
        /// потом выделить в отдельный поток
        QFile fileCFG(saveFpFn);
        if (fileCFG.open(QIODevice::WriteOnly)) {
            QByteArray ba;
            mainWindow->dumpTableToHash();
            for (int idx = objTempl->beginConfig; idx < objTempl->strList.size(); idx++) {
                QString tmp = objTempl->strList[idx];
                for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
                    tmp = tmp.replace(hIt.key(), hIt.value().value, Qt::CaseSensitive);
                }
                ba.append(tmp + "\r\n");
            }
            if (fileCFG.write(ba)) {
                QMessageBox(QMessageBox::Information, QObject::tr("Info"), QObject::tr("config saved successfully"), QMessageBox::Ok, mainWindow).exec();
                fileCFG.flush();
            } else {
                QMessageBox(QMessageBox::Critical, QObject::tr("Error!"), QObject::tr("error writing config"), QMessageBox::Ok, mainWindow).exec();
            }
            fileCFG.close();
        } else { // ошибка при создании файла
            QMessageBox(QMessageBox::Critical, QObject::tr("Error!"), QObject::tr("error saving config"), QMessageBox::Ok, mainWindow).exec();
        }
        ///
    }
}
