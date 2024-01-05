#include "buttons.h"
#include "mwindow.h"
#include "csv.h"
#include "template.h"
#include "settings.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

//#include <iostream>

using namespace std;

extern QApplication *app;
extern NNCGMainWindow *mainWindow;
extern NNCG_csv *objCSV;
extern NNCGTemplate *objTempl;
extern NNCGSettings objSett;
extern QString t2s(varType_t type);
extern QString QS_VVT;

// тексты, связанные с работой кнопок
const array<QString, LANGS_AMOUNT> QS_LOADCSV {"Load CSV", "Загр. CSV"};
const array<QString, LANGS_AMOUNT> QS_TT_LOADCSV {"Loading previously saved values from a CSV file.", "Загрузка ранее сохранённых значений из файла CSV."};
const array<QString, LANGS_AMOUNT> QS_SAVECSV {"Save CSV", "Сохр. CSV"};
const array<QString, LANGS_AMOUNT> QS_TT_SAVECSV {"Saving entered values to a CSV file.", "Сохранение введённых значений в файл CSV."};
const array<QString, LANGS_AMOUNT> QS_CSVFILES {"CSV files (*.csv)", "Файлы CSV (*.csv)"};
const array<QString, LANGS_AMOUNT> QS_DIAGERR {"Error!", "Ошибка!"};
const array<QString, LANGS_AMOUNT> QS_DIAGINFO {"Information", "Информация"};
const array<QString, LANGS_AMOUNT> QS_OPNNGCSV {"Opening CSV", "Открытие CSV"};
const array<QString, LANGS_AMOUNT> QS_SVNGCSV {"Saving CSV", "Сохранение CSV"};
const array<QString, LANGS_AMOUNT> QS_LDTMPLT {"Load template", "Загр. шаблон"};
const array<QString, LANGS_AMOUNT> QS_TT_LDTMPLT {"Loading a template from a file.", "Загрузка шаблона из файла."};
const array<QString, LANGS_AMOUNT> QS_OPNNGTMPLT {"Opening template", "Открытие шаблона"};
const array<QString, LANGS_AMOUNT> QS_TXTFILES {"Text files (*.txt)", "Текстовые файлы (*.txt)"};
const array<QString, LANGS_AMOUNT> QS_SVNGCFG {"Saving config", "Сохранение конфига"};
const array<QString, LANGS_AMOUNT> QS_CFGFILES {"CFG files (*.cfg)", "Файлы CFG (*.cfg)"};
const array<QString, LANGS_AMOUNT> QS_CRTCFG {"Create config", "Созд. конфиг"};
const array<QString, LANGS_AMOUNT> QS_TT_CRTCFG {"Creating a device configuration.", "Создание конфигурации устройства."};
const array<QString, LANGS_AMOUNT> QS_CSV_SSC {"csv saved successfully", "csv успешно сохранён"};
const array<QString, LANGS_AMOUNT> QS_CSV_ERWR {"error writing csv", "ошибка записи в csv"};
const array<QString, LANGS_AMOUNT> QS_CSV_ERSV {"error creating csv", "ошибка при создании csv"};
const array<QString, LANGS_AMOUNT> QS_CFG_SSC {"config saved successfully", "конфиг успешно сохранён"};
const array<QString, LANGS_AMOUNT> QS_CFG_ERWE {"error writing config", "ошибка при записи конфига"};
const array<QString, LANGS_AMOUNT> QS_CFG_ERSV {"error creating config", "ошибка при создании конфига"};
const array<QString, LANGS_AMOUNT> QS_TT_LANGSW {"Switching the interface language.", "Переключение языка интерфейса."};
const array<QString, LANGS_AMOUNT> QS_TT_CLRALL {"Clear the entire table.", "Очистить всю таблицу."};
const array<QString, LANGS_AMOUNT> QS_TT_THMCHNG {"Change the GUI theme.", "Сменить тему оформления."};

NNCGBtnCsvLoad::NNCGBtnCsvLoad(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGBtnCsvLoad::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setText(QS_LOADCSV.at(objSett.curLang));
        setToolTip(QS_TT_LOADCSV.at(objSett.curLang));
    }
    event->accept();
}

void NNCGBtnCsvLoad::slotClicked() {
    QString newFN = QFileDialog::getOpenFileName(this, QS_OPNNGCSV.at(objSett.curLang), "./", QS_CSVFILES.at(objSett.curLang));
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
            QMessageBox(QMessageBox::Critical, QS_DIAGERR.at(objSett.curLang), newCSV->lastErrMsg, QMessageBox::Ok, mainWindow).exec();
            delete newCSV;
        }
        ///
    }
}


NNCGBtnCsvSave::NNCGBtnCsvSave(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGBtnCsvSave::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setText(QS_SAVECSV.at(objSett.curLang));
        setToolTip(QS_TT_SAVECSV.at(objSett.curLang));
    }
    event->accept();
}

void NNCGBtnCsvSave::slotClicked() {
    QString saveFpFn = QFileDialog::getSaveFileName(this, QS_SVNGCSV.at(objSett.curLang), "./", QS_CSVFILES.at(objSett.curLang));
    if (!saveFpFn.isEmpty()) {
        /// потом выделить в отдельный поток
        QFile fileCSV(saveFpFn);
        if (fileCSV.open(QIODevice::WriteOnly)) {
            QByteArray ba;
            mainWindow->dumpTableToHash();
            ba.append(QString(QS_VVT).toStdString());
            for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
                ba.append(QString(QString("\r\n\"%1\";\"%2\";\"%3\"").arg(hIt.key(), hIt.value().value, t2s(hIt.value().type))).toStdString());
            }
            if (fileCSV.write(ba)) {
                QMessageBox(QMessageBox::Information, QS_DIAGINFO.at(objSett.curLang), QS_CSV_SSC.at(objSett.curLang), QMessageBox::Ok, mainWindow).exec();
                fileCSV.flush();
            } else {
                QMessageBox(QMessageBox::Critical, QS_DIAGERR.at(objSett.curLang), QS_CSV_ERWR.at(objSett.curLang), QMessageBox::Ok, mainWindow).exec();
            }
            fileCSV.close();
        } else { // ошибка при создании файла
            QMessageBox(QMessageBox::Critical, QS_DIAGERR.at(objSett.curLang), QS_CSV_ERSV.at(objSett.curLang), QMessageBox::Ok, mainWindow).exec();
        }
        ///
    }
}


NNCGButtonLoad::NNCGButtonLoad(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGButtonLoad::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setText(QS_LDTMPLT.at(objSett.curLang));
        setToolTip(QS_TT_LDTMPLT.at(objSett.curLang));
    }
    event->accept();
}


void NNCGButtonLoad::slotClicked() {
    QString newFN = QFileDialog::getOpenFileName(this, QS_OPNNGTMPLT.at(objSett.curLang), "./",  QS_TXTFILES.at(objSett.curLang));
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
            QMessageBox(QMessageBox::Critical, QS_DIAGERR.at(objSett.curLang), newTempl->lastErrMsg, QMessageBox::Ok, mainWindow).exec();
            delete newTempl;
        }
    }
}


NNCGButtonCreate::NNCGButtonCreate(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
}

void NNCGButtonCreate::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setText(QS_CRTCFG.at(objSett.curLang));
        setToolTip(QS_TT_CRTCFG.at(objSett.curLang));
    }
    event->accept();
}

void NNCGButtonCreate::slotClicked() {
    QString saveFpFn = QFileDialog::getSaveFileName(this, QS_SVNGCFG.at(objSett.curLang), "./", QS_CFGFILES.at(objSett.curLang));
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
                ba.append(QString(tmp + "\r\n").toStdString());
            }
            if (fileCFG.write(ba)) {
                QMessageBox(QMessageBox::Information, QS_DIAGINFO.at(objSett.curLang), QS_CFG_SSC.at(objSett.curLang), QMessageBox::Ok, mainWindow).exec();
                fileCFG.flush();
            } else {
                QMessageBox(QMessageBox::Critical, QS_DIAGERR.at(objSett.curLang), QS_CFG_ERWE.at(objSett.curLang), QMessageBox::Ok, mainWindow).exec();
            }
            fileCFG.close();
        } else { // ошибка при создании файла
            QMessageBox(QMessageBox::Critical, QS_DIAGERR.at(objSett.curLang), QS_CFG_ERSV.at(objSett.curLang), QMessageBox::Ok, mainWindow).exec();
        }
        ///
    }
}


NNCGBtnClearAll::NNCGBtnClearAll(int w, int h, const QString &text, QWidget *parent): QPushButton(text, parent) {
    setFixedSize(w, h);
    setIconSize({40, 40});
    setIcon(QIcon(":/ca.png"));
}

void NNCGBtnClearAll::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setToolTip(QS_TT_CLRALL.at(objSett.curLang));
    }
    event->accept();
}

void NNCGBtnClearAll::slotClicked() {
    mainWindow->clearTable();
}


NNCGBtnLangSwitch::NNCGBtnLangSwitch(int w, int h, QWidget *parent): QPushButton(parent) {
    setFixedSize(w, h);
    setStyleSheet(QString(":enabled {background: transparent; image : url(:/mt.png)}"
                          ":hover   {background: transparent; image : url(:/mth.png)}"
                          ":pressed {background: transparent; image : url(:/mtp.png)}"));
}


void NNCGBtnLangSwitch::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setToolTip(QS_TT_LANGSW.at(objSett.curLang));
    }
    event->accept();
}


void NNCGBtnLangSwitch::slotClicked() {
    if (objSett.curLang == langId_t::English) objSett.curLang = langId_t::Russian;
    else objSett.curLang = langId_t::English;
    setToolTip(QS_TT_LANGSW.at(objSett.curLang));
    QApplication::postEvent(mainWindow, new QEvent(QEvent::LanguageChange));
}


NNCGBtnThemeSwitch::NNCGBtnThemeSwitch(int w, int h, QWidget *parent): QPushButton(parent) {
    setFixedSize(w, h);
    setStyleSheet(QString(":enabled {background: transparent; image : url(:/tm.png)}"
                          ":hover   {background: transparent; image : url(:/tmh.png)}"
                          ":pressed {background: transparent; image : url(:/tmp.png)}"));
}


void NNCGBtnThemeSwitch::slotClicked() {
    objSett.curThemeId++; // меняем тему на следующую
    if (objSett.curThemeId >= themeId_t::UnknownTheme) objSett.curThemeId = 0;
    mainWindow->repaintWithTheme(); // перерисовываем главное окно и его дочерние виджеты в соотв. с выставленной темой
}

void NNCGBtnThemeSwitch::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        setToolTip(QS_TT_THMCHNG.at(objSett.curLang));
    }
    event->accept();
}
