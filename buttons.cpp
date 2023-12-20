#include "buttons.h"
#include "mwindow.h"
#include "csv.h"
#include "template.h"
#include "settings.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

//#include <iostream>

extern QApplication *app;
extern NNCGMainWindow *mainWindow;
extern NNCG_csv *objCSV;
extern NNCGTemplate *objTempl;
extern NNCGSettings objSett;
extern QString t2s(varType_t type);
extern QString QS_VVT;

// тексты, связанные с работой кнопок
const QString QS_LOADCSV [LANGS_AMOUNT] {"Load CSV", "Загр. CSV", "_"};
const QString QS_SAVECSV [LANGS_AMOUNT] {"Save CSV", "Сохр. CSV", "_"};
const QString QS_CSVFILES [LANGS_AMOUNT] {"CSV files (*.csv)", "Файлы CSV (*.csv)", "_"};
const QString QS_DIAGERR [LANGS_AMOUNT] {"Error!", "Ошибка!", "_"};
const QString QS_DIAGINFO [LANGS_AMOUNT] {"Information", "Информация", "_"};
const QString QS_OPNNGCSV [LANGS_AMOUNT] {"Opening CSV", "Открытие CSV", "_"};
const QString QS_SVNGCSV [LANGS_AMOUNT] {"Saving CSV", "Сохранение CSV", "_"};
const QString QS_LDTMPLT [LANGS_AMOUNT] {"Load template", "Загр. шаблон", "_"};
const QString QS_OPNNGTMPLT [LANGS_AMOUNT] {"Opening template", "Открытие шаблона", "_"};
const QString QS_TXTFILES [LANGS_AMOUNT] {"Text files (*.txt)", "Текстовые файлы (*.txt)", "_"};
const QString QS_SVNGCFG [LANGS_AMOUNT] {"Saving config", "Сохранение конфига", "_"};
const QString QS_CFGFILES [LANGS_AMOUNT] {"CFG files (*.cfg)", "Файлы CFG (*.cfg)", "_"};
const QString QS_CRTCFG [LANGS_AMOUNT] {"Create config", "Созд. конфиг", "_"};
const QString QS_CSV_SSC [LANGS_AMOUNT] {"csv saved successfully", "csv успешно сохранён", "_"};
const QString QS_CSV_ERWR [LANGS_AMOUNT] {"error writing csv", "ошибка записи в csv", "_"};
const QString QS_CSV_ERSV [LANGS_AMOUNT] {"error creating csv", "ошибка при создании csv", "_"};
const QString QS_CFG_SSC [LANGS_AMOUNT] {"config saved successfully", "конфиг успешно сохранён", "_"};
const QString QS_CFG_ERWE [LANGS_AMOUNT] {"error writing config", "ошибка при записи конфига", "_"};
const QString QS_CFG_ERSV [LANGS_AMOUNT] {"error creating config", "ошибка при создании конфига", "_"};


NNCGBtnCsvLoad::NNCGBtnCsvLoad(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGBtnCsvLoad::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) setText(QS_LOADCSV[objSett.curLang]);
    else QPushButton::changeEvent(event);
}

void NNCGBtnCsvLoad::slotClicked() {
    QString newFN = QFileDialog::getOpenFileName(this, QS_OPNNGCSV[objSett.curLang], "./", QS_CSVFILES[objSett.curLang]);
    if (!newFN.isEmpty()) {
        /// потом выделить в отдельный поток
        auto *newCSV = new NNCG_csv(newFN, objTempl->getDelimOpen(), objTempl->getDelimClose());
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
            QMessageBox(QMessageBox::Critical, QS_DIAGERR[objSett.curLang], newCSV->lastErrMsg, QMessageBox::Ok, mainWindow).exec();
            delete newCSV;
        }
        ///
    }
}


NNCGBtnCsvSave::NNCGBtnCsvSave(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGBtnCsvSave::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) setText(QS_SAVECSV[objSett.curLang]);
    else QPushButton::changeEvent(event);
}

void NNCGBtnCsvSave::slotClicked() {
    QString saveFpFn = QFileDialog::getSaveFileName(this, QS_SVNGCSV[objSett.curLang], "./", QS_CSVFILES[objSett.curLang]);
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
                QMessageBox(QMessageBox::Information, QS_DIAGINFO[objSett.curLang], QS_CSV_SSC[objSett.curLang], QMessageBox::Ok, mainWindow).exec();
                fileCSV.flush();
            } else {
                QMessageBox(QMessageBox::Critical, QS_DIAGERR[objSett.curLang], QS_CSV_ERWR[objSett.curLang], QMessageBox::Ok, mainWindow).exec();
            }
            fileCSV.close();
        } else { // ошибка при создании файла
            QMessageBox(QMessageBox::Critical, QS_DIAGERR[objSett.curLang], QS_CSV_ERSV[objSett.curLang], QMessageBox::Ok, mainWindow).exec();
        }
        ///
    }
}


NNCGButtonLoad::NNCGButtonLoad(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGButtonLoad::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) setText(QS_LDTMPLT[objSett.curLang]);
    else QPushButton::changeEvent(event);
}

void NNCGButtonLoad::slotClicked() {
    QString newFN = QFileDialog::getOpenFileName(this, QS_OPNNGTMPLT[objSett.curLang], "./",  QS_TXTFILES[objSett.curLang]);
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
            mainWindow->btnClearAll->setEnabled(true);
        } else { // ошибка при открытии файла или некорректные данные в заголовке
            QMessageBox(QMessageBox::Critical, QS_DIAGERR[objSett.curLang], newTempl->lastErrMsg, QMessageBox::Ok, mainWindow).exec();
            delete newTempl;
        }
    }
}


NNCGButtonCreate::NNCGButtonCreate(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGButtonCreate::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) setText(QS_CRTCFG[objSett.curLang]);
    else QPushButton::changeEvent(event);
}

void NNCGButtonCreate::slotClicked() {
    QString saveFpFn = QFileDialog::getSaveFileName(this, QS_SVNGCFG[objSett.curLang], "./", QS_CFGFILES[objSett.curLang]);
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
                QMessageBox(QMessageBox::Information, QS_DIAGINFO[objSett.curLang], QS_CFG_SSC[objSett.curLang], QMessageBox::Ok, mainWindow).exec();
                fileCFG.flush();
            } else {
                QMessageBox(QMessageBox::Critical, QS_DIAGERR[objSett.curLang], QS_CFG_ERWE[objSett.curLang], QMessageBox::Ok, mainWindow).exec();
            }
            fileCFG.close();
        } else { // ошибка при создании файла
            QMessageBox(QMessageBox::Critical, QS_DIAGERR[objSett.curLang], QS_CFG_ERSV[objSett.curLang], QMessageBox::Ok, mainWindow).exec();
        }
        ///
    }
}


NNCGBtnClearAll::NNCGBtnClearAll(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
    setIconSize({40, 40});
    setIcon(QIcon(":/ca.png"));
}


void NNCGBtnClearAll::slotClicked() {
    mainWindow->clearTable();
}


NNCGBtnSetLang::NNCGBtnSetLang(int w, int h, langId_t lang, QWidget *parent): QPushButton(parent) {
    setFixedSize(w, h);
    setIconSize({w, h});
    setIcon(QIcon(natEmblems[lang]));
    newLang = lang;
}

void NNCGBtnSetLang::slotClicked() {
    objSett.curLang = newLang;
    app->postEvent(mainWindow, new QEvent(QEvent::LanguageChange));
}
