#include "common.h"
#include "mwindow.h"
#include "settings.h"
#include "template.h"
#include "table.h"
#include "validators.h"

#include <QCommonStyle>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QScrollBar>
#include <QHeaderView>
#include <QFont>
#include <QApplication>
#include <QPalette>

using namespace std;

extern NNCGSettings objSett;
//extern theme_t themeCurrent;
extern array<theme_t, themeId_t::UnknownTheme> gammaApp;
extern NNCGTemplate* objTempl;
extern QString b2s(bool b);

extern array<QString, LANGS_AMOUNT> QS_TBLDESCR;
extern array<QString, LANGS_AMOUNT> QS_TBLVALUE;

//enum varType_t                                      {Domname = 0, Text, IPv4, Unsigned, Password, MASKv4, IPv6, MASKv6Len, WildcardV4, MASKv4Len, Prompt, Hash, MAX}; // типы переменных
extern const array<int, int(varType_t::MAX)> maxChars {        253,  255,   15,       10,      128,     15,   45,         3,         15,         2,     64,  128};      // длины полей ввода в символах

// индексы языков [][0] - eng, [][1] - rus, [][2] - elfian
//
// placeholders text
const extern array<array<QString, LANGS_AMOUNT>, int(varType_t::MAX)> QS_PLCHLDRS {{
        {"max 253 symbols, latin and hyphens", "макс. 253 символа, латиница и дефисы"}, // domname
        {"max 255 unicode symbols", "макс. 255 символов юникод"}, // text
        {"___.___.___.___", ""}, // ipv4
        {"unsigned integer", "целое беззнаковое"}, // unsigned
        {"hidden, max 128 symbols", "скрытый ввод, макс. 128 символов"}, // password
        {"___.___.___.___", ""}, // ipv4 dotted mask
        {"____:____:____:____:____:____:____:____", ""}, // ipv6
        {"integer from 0 to 128", "целое от 0 до 128"}, // ipv6 mask length
        {"___.___.___.___", ""}, // ipv4 wildcard
        {"integer from 0 to 32", "целое от 0 до 32"}, // ipv4 mask length
        {"max 64 unicode symbols", "макс. 64 символа юникод"}, // system prompt
        {"max 128 symbols, latin and special", "макс. 128 символов, латиница и спецсимволы"} // hash
      }};


// создаём все элементы GUI
NNCGMainWindow::NNCGMainWindow(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags) {
    this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    this->resize(objSett.width, objSett.height);
    if (objSett.maximized) setWindowState(Qt::WindowMaximized);
    bigWidget = new QWidget(this, Qt::Widget);
    statusBar = new QStatusBar(this);
    statusBar->showMessage(DISPLAY_APPVER);

    this->setAutoFillBackground(true);

    bigWidget->setAutoFillBackground(true);

    statusBar->setFixedHeight(20);
    statusBar->setAutoFillBackground(true);

    this->setCentralWidget(bigWidget);
    this->setStatusBar(statusBar);

    auto *vbLayout = new QVBoxLayout(nullptr);
    bigWidget->setLayout(vbLayout);
    vbLayout->setContentsMargins(20, 0, 20, 0);

    auto *hbTop = new QHBoxLayout(nullptr);
    hbTop->setSpacing(24);
    hbTop->setContentsMargins(4, 20, 0, 20);

    logoLabel = new QLabel();
    logoLabel->setFixedSize(64, 64);
    logoLabel->setScaledContents(true);
    hbTop->addWidget(logoLabel);

    auto *vbTopRight = new QVBoxLayout(nullptr);
    vbTopRight->setContentsMargins(0, 0, 0, 0);
    vbTopRight->setSpacing(6);
    hbTop->addLayout(vbTopRight);

    titleLabel = new QLabel();
    QFont tmpFnt("Tahoma", 16, 0, false);
    tmpFnt.setKerning(false);
    titleLabel->setFixedHeight(32);
    titleLabel->setFont(tmpFnt);
    titleLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    vbTopRight->addWidget(titleLabel);

    commentLabel = new QLabel();
    commentLabel->setFixedHeight(24);
    commentLabel->setFont(QFont("Tahoma", 10, 0, false));
    commentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vbTopRight->addWidget(commentLabel);

    vbLayout->addLayout(hbTop);

    table = new NNCGTable();
    table->setColumnCount(3);
    table->setColumnWidth(0, TABLE_ZERO_COLUMN_WIDTH);
    table->setColumnWidth(1, objSett.colWidth);
    table->setCornerButtonEnabled(false);
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setFixedHeight(24);
    table->horizontalHeader()->setHighlightSections(false); // не делать жирным шрифт в заголовке при выборе строки
    table->horizontalHeader()->setStretchLastSection(true);
    table->setFrameShape(QFrame::NoFrame);
    table->setSortingEnabled(false);
    table->setHorizontalHeaderLabels({"", "", ""});
    table->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);

    auto *hbBottom = new QHBoxLayout(nullptr);
    hbBottom->setSpacing(12);
    hbBottom->setContentsMargins(0, 16, 0, 20);

    btnCsvLoad = new NNCGBtnCsvLoad(80, 32, "");
    btnCsvSave = new NNCGBtnCsvSave(80, 32, "");

    auto *hbSI = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btnClearAll = new NNCGBtnClearAll(48, 48, "", this);
    btnLangSwitch = new NNCGBtnLangSwitch(32, 32, this);
    btnThemeSwitch = new NNCGBtnThemeSwitch(32, 32, this);

    btnTemplLoad = new NNCGButtonLoad(108, 32, "");
    btnCfgCreate = new NNCGButtonCreate(108, 32, "");

    hbBottom->addWidget(btnCsvLoad);
    hbBottom->addWidget(btnCsvSave);
    hbBottom->addSpacerItem(hbSI);
    hbBottom->addWidget(btnTemplLoad);
    hbBottom->addWidget(btnCfgCreate);

    bigWidget->layout()->addWidget(table);
    vbLayout->addLayout(hbBottom);

    connect(btnCsvLoad, SIGNAL(clicked()), btnCsvLoad, SLOT(slotClicked()), Qt::AutoConnection);
    connect(btnCsvSave, SIGNAL(clicked()), btnCsvSave, SLOT(slotClicked()), Qt::AutoConnection);
    connect(btnTemplLoad, SIGNAL(clicked()), btnTemplLoad, SLOT(slotClicked()), Qt::AutoConnection);
    connect(btnCfgCreate, SIGNAL(clicked()), btnCfgCreate, SLOT(slotClicked()), Qt::AutoConnection);
    connect(btnClearAll, SIGNAL(clicked()), btnClearAll, SLOT(slotClicked()), Qt::AutoConnection);
    connect(btnLangSwitch, SIGNAL(clicked()), btnLangSwitch, SLOT(slotClicked()), Qt::AutoConnection);
    connect(btnThemeSwitch, SIGNAL(clicked()), btnThemeSwitch, SLOT(slotClicked()), Qt::AutoConnection);

   // создаём валидаторы
    vldtrs[varType_t::Domname] = new NNCGValidDomname(this);
    vldtrs[varType_t::Text] = nullptr;
    vldtrs[varType_t::IPv4] = new NNCGValidIPv4(this);
    vldtrs[varType_t::Unsigned] = new NNCGValidUnsigned(this);
    vldtrs[varType_t::Password] = nullptr;
    vldtrs[varType_t::MASKv4] = new NNCGValidMASKv4(this);
    vldtrs[varType_t::IPv6] = new NNCGValidIPv6(this);
    vldtrs[varType_t::MASKv6Len] = new NNCGValidMASKv6Len(this);
    vldtrs[varType_t::WildcardV4] = new NNCGValidIPv4(this);
    vldtrs[varType_t::MASKv4Len] = new NNCGValidMASKv4Len(this);
    vldtrs[varType_t::Prompt] = nullptr;
    vldtrs[varType_t::Hash] = new NNCGValidHash(this);
}


// обновляем наполнение таблицы и меняем оформление окна в соответствии с текущим шаблоном
void NNCGMainWindow::refreshTable() {
    this->setDisabled(true);
    logoLabel->setPixmap(*objTempl->getPtrPixLogo());
    titleLabel->setText(objTempl->getTitle());
    commentLabel->setText(objTempl->getComment());
    table->clear(); // уничтожаются ли дочерние QLineEdit ?
    table->setHorizontalHeaderLabels({"#", QS_TBLDESCR.at(objSett.curLang), QS_TBLVALUE.at(objSett.curLang)});
    table->setRowCount((int)objTempl->hashVars.size());
    for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
        array <QTableWidgetItem*, 3> oneRow {};
        oneRow[0] = new QTableWidgetItem(QString::number(hIt.value().orderNum + 1), 0);
        oneRow[0]->setFont(fntCons10);
        oneRow[0]->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        oneRow[0]->setFlags(Qt::NoItemFlags);
        oneRow[1] = new QTableWidgetItem(hIt.value().descr, 0);
        oneRow[1]->setFont(fntCons11);
        oneRow[1]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        oneRow[1]->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
        for (int col = 0; col < 2; col++) table->setItem(hIt.value().orderNum, col, oneRow.at(col));
        auto qle = new QLineEdit;
        qle->setFont(fntCons12bold);
        qle->setFrame(false);
        qle->setMaxLength(maxChars.at(hIt.value().type));
        qle->setClearButtonEnabled(true);
        if (!QS_PLCHLDRS.at(hIt.value().type).at(1).isEmpty()) qle->setPlaceholderText(QS_PLCHLDRS.at(hIt.value().type).at(objSett.curLang));
        else qle->setPlaceholderText(QS_PLCHLDRS.at(hIt.value().type).at(0));
        if (hIt.value().type == varType_t::Password)
            qle->setEchoMode(QLineEdit::Password);
        qle->setValidator(vldtrs.at(int(hIt.value().type)));
        qle->setText(hIt.value().value);
        table->setCellWidget(hIt.value().orderNum, 2, qle);
    };
    int colW = table->columnWidth(1);
    table->horizontalHeader()->setStyleSheet(QString("::section {background-color: rgb(%1, %2, %3); color: rgb(%4, %5, %6); font: bold 14px 'Consolas'}")
                                             .arg(QString::number(objTempl->brandColors[0]),
                                                  QString::number(objTempl->brandColors[1]),
                                                  QString::number(objTempl->brandColors[2]),
                                                  QString::number(objTempl->brandColors[3]),
                                                  QString::number(objTempl->brandColors[4]),
                                                  QString::number(objTempl->brandColors[5]))
                                             );
    table->setColumnWidth(0, TABLE_ZERO_COLUMN_WIDTH);
    table->setColumnWidth(1, colW);

    auto sbPal = statusBar->palette();
    sbPal.setColor(QPalette::Window, QColor(objTempl->brandColors[0], objTempl->brandColors[1], objTempl->brandColors[2]));
    sbPal.setColor(QPalette::WindowText, QColor(objTempl->brandColors[3], objTempl->brandColors[4], objTempl->brandColors[5]));
    statusBar->setPalette(sbPal);

    auto btnSS = QString(
        ":enabled  {background: rgb(216, 216, 216); border: 6px rgb(%1, %2, %3); border-radius: 14px; border-style: outset; font: 12px 'Tahoma'}"
        ":disabled {background: rgb(96, 96, 96);    border: 6px rgb(%4, %5, %6); border-radius: 14px; border-style: outset; font: 12px 'Tahoma'}"
        ":hover    {background: rgb(216, 216, 216); border: 6px rgb(%1, %2, %3); border-radius: 14px; border-style: inset;  font: 12px 'Tahoma'}"
        ":pressed  {background: rgb(216, 216, 216); border: 6px rgb(%1, %2, %3); border-radius:  8px; border-style: inset;  font: 12px 'Tahoma'}"
        "QToolTip:enabled {background : white; color: black; border: 0px}"
    ).arg(QString::number(objTempl->brandColors[0]), QString::number(objTempl->brandColors[1]), QString::number(objTempl->brandColors[2]),
          QString::number(objTempl->brandColors[3] / 2), QString::number(objTempl->brandColors[4] / 2), QString::number(objTempl->brandColors[5] / 2));

    btnCsvLoad->setStyleSheet(btnSS);
    btnCsvSave->setStyleSheet(btnSS);
    btnTemplLoad->setStyleSheet(btnSS);
    btnCfgCreate->setStyleSheet(btnSS);

    btnClearAll->setStyleSheet(QString(
                                   ":enabled  {background: transparent; border: 3px rgb(%1, %2, %3); border-radius: 24px; border-style: outset}"
                                   ":disabled {background: transparent;    border: 3px rgb(%4, %5, %6); border-radius: 24px; border-style: outset}"
                                   ":hover    {background: rgb(216, 216, 216); border: 3px rgb(%1, %2, %3); border-radius: 24px; border-style: inset}"
                                   ":pressed  {background: rgb(216, 216, 216); border: 3px rgb(%1, %2, %3); border-radius:  16px; border-style: inset}"
                                   "QToolTip:enabled {background : white; color: black; border: 0px}"
                               ).arg(   QString::number(objTempl->brandColors[0]),
                                        QString::number(objTempl->brandColors[1]),
                                        QString::number(objTempl->brandColors[2]),
                                        QString::number(objTempl->brandColors[3] / 2),
                                        QString::number(objTempl->brandColors[4] / 2),
                                        QString::number(objTempl->brandColors[5] / 2)
                                    ));


    this->setEnabled(true);
    this->show();
};


// при событии закрытия окна сохраняем настройки в json
void NNCGMainWindow::closeEvent(QCloseEvent *event) {
    this->hide();
    if (!objTempl->isDemo) objSett.saveSettings(objTempl->getFilePath() + objTempl->getFileName(), objSett.curThemeId, width(), height(), b2s(isMaximized()), table->horizontalHeader()->sectionSize(1), objSett.curLang);
    else objSett.saveSettings("", themeId_t::Dark, width(), height(), b2s(isMaximized()), table->horizontalHeader()->sectionSize(1), objSett.curLang);
    event->accept();
}


// сброс данных из таблицы в хэш объекта шаблона
void NNCGMainWindow::dumpTableToHash() {
    for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
        auto *cw = dynamic_cast<QLineEdit*>(table->cellWidget(hIt.value().orderNum, 2));
        objTempl->hashVars[hIt.key()].value = cw->text();
    }
}


// обнуление всех введённых значений в таблице; и в объекте шаблона (необязательно, но экономит память)
void NNCGMainWindow::clearTable() {
    for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
        hIt.value().value.clear();
        auto *cw = dynamic_cast<QLineEdit*>(table->cellWidget(hIt.value().orderNum, 2));
        cw->clear();
    }
}


void NNCGMainWindow::resizeEvent(QResizeEvent *event) {
    auto w = this->width();
    btnClearAll->move(w - 84, 48);
    btnLangSwitch->move(w - 130, 90);
    btnThemeSwitch->move(w - 178, 90);
    event->accept();
}


// вызывается сигналом при переключении темы оформления
void NNCGMainWindow::repaintWithTheme() {
    if (objSett.curThemeId >= themeId_t::UnknownTheme) objSett.curThemeId = 0;

    this->setDisabled(true);

    QPalette tmpPal;
    QColor tmpCol;

    tmpPal = bigWidget->palette();

    tmpCol = {gammaApp.at(objSett.curThemeId).bw_bg.r, gammaApp.at(objSett.curThemeId).bw_bg.g, gammaApp.at(objSett.curThemeId).bw_bg.b, 255};
    tmpPal.setColor(QPalette::Window, tmpCol);
    bigWidget->setPalette(tmpPal);

    tmpPal = titleLabel->palette();
    tmpCol = {gammaApp.at(objSett.curThemeId).ttl_fg.r, gammaApp.at(objSett.curThemeId).ttl_fg.g, gammaApp.at(objSett.curThemeId).ttl_fg.b, 255};
    tmpPal.setColor(QPalette::WindowText, tmpCol);
    titleLabel->setPalette(tmpPal);
    commentLabel->setPalette(tmpPal);

    int colW = table->columnWidth(1);
    table->setStyleSheet(QString("color: rgb(%1, %2, %3); gridline-color: rgb(%4, %5, %6); background-color: rgb(%7, %8, %9)")
                                    .arg(
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_fg.r),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_fg.g),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_fg.b),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_gr.r),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_gr.g),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_gr.b),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_bg.r),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_bg.g),
                                 QString::number(gammaApp.at(objSett.curThemeId).tbl_bg.b)
                                        )
                         );
    table->setColumnWidth(0, TABLE_ZERO_COLUMN_WIDTH);
    table->setColumnWidth(1, colW);

    table->verticalScrollBar()->setStyle(new QCommonStyle);
    table->verticalScrollBar()->setStyleSheet(QString(":vertical {background-color: rgb(%1, %2, %3)}"
                                                      "::handle:vertical {background-color: rgb(%4, %5, %6); border: 4px solid rgb(%1, %2, %3); border-radius: 8px; max-width: 20px}"
                                                      "::sub-line:vertical {height: 0px}"
                                                      "::add-line:vertical {height: 0px}")
                                              .arg(
                                                      QString::number(gammaApp.at(objSett.curThemeId).bw_bg.r),
                                                      QString::number(gammaApp.at(objSett.curThemeId).bw_bg.g),
                                                      QString::number(gammaApp.at(objSett.curThemeId).bw_bg.b),
                                                      QString::number(gammaApp.at(objSett.curThemeId).vh.r),
                                                      QString::number(gammaApp.at(objSett.curThemeId).vh.g),
                                                      QString::number(gammaApp.at(objSett.curThemeId).vh.b)
                                                  )
                                              );

    this->setEnabled(true);
}
