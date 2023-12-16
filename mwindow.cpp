#include "mwindow.h"
#include "settings.h"
#include "template.h"
#include <QCommonStyle>
#include <QLineEdit>
//#include <iostream>

extern NNCGSettings objSett;
extern theme_t themeCurrent;
extern NNCGTemplate* objTempl;
extern QString b2s(bool b);

// создаём все элементы GUI
NNCGMainWindow::NNCGMainWindow(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags) {
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    resize(objSett.width, objSett.height);
    if (objSett.maximized) setWindowState(Qt::WindowMaximized);
    bigWidget = new QWidget(this, Qt::Widget);
    statusBar = new QStatusBar(this);

    setFont(QFont("Tahoma", 10, 0, false));
    setAutoFillBackground(true);

    bigWidget->setAutoFillBackground(true);

    statusBar->setFixedHeight(20);
    statusBar->setAutoFillBackground(true);

    setCentralWidget(bigWidget);
    setStatusBar(statusBar);

    QPalette bwPal = bigWidget->palette();
    QPalette sbPal = statusBar->palette();
    bwPal.setColor(QPalette::Background, themeCurrent.bw_bg);
    bwPal.setColor(QPalette::Foreground, themeCurrent.bw_fg);
    bigWidget->setPalette(bwPal);
    sbPal.setColor(QPalette::Background, QColor(objTempl->brandColors[0], objTempl->brandColors[1], objTempl->brandColors[2]));
    sbPal.setColor(QPalette::Foreground, QColor(objTempl->brandColors[3], objTempl->brandColors[4], objTempl->brandColors[5]));
    statusBar->setPalette(sbPal);

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
    titleLabel->setFixedHeight(32);
    titleLabel->setFont(QFont("Tahoma", 16, 0, false));
    titleLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    vbTopRight->addWidget(titleLabel);

    commentLabel = new QLabel();
    commentLabel->setFixedHeight(24);
    commentLabel->setFont(QFont("Tahoma", 10, 0, false));
    commentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vbTopRight->addWidget(commentLabel);

    vbLayout->addLayout(hbTop);

    table = new QTableWidget();
    table->setColumnCount(3);
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
    table->setHorizontalHeaderLabels({tr("#"), tr("Description"), tr("Value")});
    table->setColumnWidth(0, 32);
    table->setColumnWidth(1, objSett.colWidth);
    table->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->setStyleSheet("color: rgb(190, 190, 190); gridline-color: rgb(50, 50, 50); background-color: rgb(60, 60, 60)");
    table->horizontalHeader()->setStyleSheet(QString("::section {background-color: rgb(%1, %2, %3); color: rgb(%4, %5, %6); font: bold 14px 'Consolas'}")
                                             .arg(QString::number(objTempl->brandColors[0]),
                                                  QString::number(objTempl->brandColors[1]),
                                                  QString::number(objTempl->brandColors[2]),
                                                  QString::number(objTempl->brandColors[3]),
                                                  QString::number(objTempl->brandColors[4]),
                                                  QString::number(objTempl->brandColors[5]))
                                             );
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->verticalScrollBar()->setStyle(new QCommonStyle);
    table->verticalScrollBar()->setStyleSheet(":vertical {background-color: rgb(37, 37, 37)}"
                                              "::handle:vertical {background-color: rgb(60, 60, 60); border: 4px solid rgb(37, 37, 37); border-radius: 8px; max-width: 20px}"
                                              "::sub-line:vertical {height: 0px}"
                                              "::add-line:vertical {height: 0px}"
                                              );

    auto *hbBottom = new QHBoxLayout(nullptr);
    hbBottom->setSpacing(12);
    hbBottom->setContentsMargins(0, 16, 0, 20);

    btnCsvLoad = new NNCGBtnCsvLoad(80, 32, tr("Load CSV"));
    btnCsvSave = new NNCGBtnCsvSave(80, 32, tr("Save CSV"));

    auto *hbSI = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btnTemplLoad = new NNCGButtonLoad(108, 32, tr("Load template"));
    btnCfgCreate = new NNCGButtonCreate(108, 32, tr("Create config"));

    auto btnSS = QString(
        ":enabled  {background: rgb(216, 216, 216); border: 6px rgb(%1, %2, %3); border-radius: 14px; border-style: outset; font: 12px 'Tahoma'}"
        ":disabled {background: rgb(96, 96, 96);    border: 6px rgb(%4, %5, %6); border-radius: 14px; border-style: outset; font: 12px 'Tahoma'}"
        ":hover    {background: rgb(216, 216, 216); border: 6px rgb(%1, %2, %3); border-radius: 14px; border-style: inset;  font: 12px 'Tahoma'}"
        ":pressed  {background: rgb(216, 216, 216); border: 6px rgb(%1, %2, %3); border-radius:  8px; border-style: inset;  font: 12px 'Tahoma'}"
    ).arg(QString::number(objTempl->brandColors[0]), QString::number(objTempl->brandColors[1]), QString::number(objTempl->brandColors[2]),
          QString::number(objTempl->brandColors[3] / 2), QString::number(objTempl->brandColors[4] / 2), QString::number(objTempl->brandColors[5] / 2));

    btnCsvLoad->setStyleSheet(btnSS);
    btnCsvSave->setStyleSheet(btnSS);
    btnTemplLoad->setStyleSheet(btnSS);
    btnCfgCreate->setStyleSheet(btnSS);

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

   // validators creation
    for (int vld = varType_t::Domname; vld < varType_t::MAX; vld++) vldtrs[vld] = nullptr;
    vldtrs[varType_t::Domname] = new NNCGValidDomname(this);
    vldtrs[varType_t::IPv4] = new NNCGValidIPv4(this);
    vldtrs[varType_t::Unsigned] = new NNCGValidUnsigned(this);
    vldtrs[varType_t::MASKv4] = new NNCGValidMASKv4(this);
}


// обновляем наполнение таблицы и меняем оформление окна в соответствии с текущим шаблоном
void NNCGMainWindow::refreshTable() {
    this->hide();
    logoLabel->setPixmap(*objTempl->getPtrPixLogo());
    titleLabel->setText(objTempl->getTitle());
    commentLabel->setText(objTempl->getComment());
    table->clear();
    table->setHorizontalHeaderLabels({tr("#"), tr("Description"), tr("Value")});
    table->setRowCount(objTempl->hashVars.size());
    for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
        QTableWidgetItem *oneRow[3];
        oneRow[0] = new QTableWidgetItem(QString::number(hIt.value().orderNum + 1), 0);
        oneRow[0]->setFont(fntCons10);
        oneRow[0]->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        oneRow[0]->setFlags(Qt::NoItemFlags);
        oneRow[1] = new QTableWidgetItem(hIt.value().descr, 0);
        oneRow[1]->setFont(fntCons11);
        oneRow[1]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        oneRow[1]->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
        //oneRow[2] = new QTableWidgetItem(hIt.value().value, 0);
        //oneRow[2]->setFont(fntCons12bold);
        //oneRow[2]->setForeground(QColor(100, 10, 255, 255));
        //oneRow[2]->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
        auto qle = new QLineEdit;
        qle->setFont(fntCons12bold);
        qle->setFrame(false);
        qle->setMaxLength(maxChars[hIt.value().type]);
        qle->setClearButtonEnabled(true);
        //qle->setPlaceholderText("текст");
        if (hIt.value().type == varType_t::Password)
            qle->setEchoMode(QLineEdit::Password);
        qle->setValidator(vldtrs[hIt.value().type]);
        qle->setText(hIt.value().value);
        table->setCellWidget(hIt.value().orderNum, 2, qle);
        for (int col = 0; col < 2; col++) table->setItem(hIt.value().orderNum, col, oneRow[col]);
    };
    this->show();
};


// при событии закрытия окна сохраняем настройки в json
void NNCGMainWindow::closeEvent(QCloseEvent* event) {
    this->hide();
    if (!objTempl->isDemo) objSett.saveSettings(objTempl->getFilePath() + objTempl->getFileName(), QS_DARK, width(), height(), b2s(isMaximized()), table->horizontalHeader()->sectionSize(1));
    else objSett.saveSettings("", QS_DARK, width(), height(), b2s(isMaximized()), table->horizontalHeader()->sectionSize(1));
    event->accept();
}


// сброс данных из таблицы в хэш объекта шаблона
void NNCGMainWindow::dumpTableToHash() {
    for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
        QLineEdit *cw = (QLineEdit*) table->cellWidget(hIt.value().orderNum, 2);
        objTempl->hashVars[hIt.key()].value = cw->text();
    }
}
