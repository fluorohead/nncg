#include "mwindow.h"
#include "settings.h"
#include "template.h"
//#include <iostream>

extern NNCGSettings objSett;
extern theme_t themeCurrent;
extern NNCGTemplate* objTempl;
extern QString b2s(bool b);

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
    sbPal.setColor(QPalette::Background, themeCurrent.sb_bg);
    sbPal.setColor(QPalette::Foreground, themeCurrent.sb_fg);
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
    table->setHorizontalHeaderLabels({"1", "2", "3"});
    table->setFrameShape(QFrame::NoFrame);
    table->setSortingEnabled(false);
    table->setHorizontalHeaderLabels({QObject::tr("#"), QObject::tr("Description"), QObject::tr("Value")});
    table->setColumnWidth(0, 48);
    table->setColumnWidth(1, 300);
    table->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    table->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // выравнивание текста вправо для колонки 0
    //table->setRowCount(objTempl->hashVars.size());
    table->setStyleSheet("color: rgb(190, 190, 190); gridline-color: rgb(50, 50, 50); background-color: rgb(60, 60, 60)");

    auto *hbBottom = new QHBoxLayout(nullptr);
    hbBottom->setSpacing(12);
    hbBottom->setContentsMargins(0, 16, 0, 20);

    QString btnSS {
        ":enabled  {background: rgb(216, 216, 216); border: 6px rgb(0, 139, 224); border-radius: 14px; border-style: outset; font: 12px 'Tahoma'}"
        ":disabled {background: rgb(96, 96, 96);    border: 6px rgb(0, 50, 135);  border-radius: 14px; border-style: outset; font: 12px 'Tahoma'}"
        ":hover    {background: rgb(216, 216, 216); border: 6px rgb(0, 139, 224); border-radius: 14px; border-style: inset;  font: 12px 'Tahoma'}"
        ":pressed  {background: rgb(216, 216, 216); border: 6px rgb(0, 139, 224); border-radius:  8px; border-style: inset;  font: 12px 'Tahoma'}"
    };

    btnCsvLoad = new NNCGBtnCsvLoad(80, 32, QObject::tr("Load CSV"));
    btnCsvLoad->setStyleSheet(btnSS);
    btnCsvSave = new NNCGBtnCsvSave(80, 32, QObject::tr("Save CSV"));
    btnCsvSave->setStyleSheet(btnSS);

    auto *hbSI = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btnTemplLoad = new NNCGButtonLoad(108, 32, QObject::tr("Load template"));
    btnTemplLoad->setStyleSheet(btnSS);
    btnCfgCreate = new NNCGButtonCreate(108, 32, QObject::tr("Create config"));
    btnCfgCreate->setStyleSheet(btnSS);

    hbBottom->addWidget(btnCsvLoad);
    hbBottom->addWidget(btnCsvSave);
    hbBottom->addSpacerItem(hbSI);
    hbBottom->addWidget(btnTemplLoad);
    hbBottom->addWidget(btnCfgCreate);

    bigWidget->layout()->addWidget(table);
    vbLayout->addLayout(hbBottom);

    QObject::connect(btnCsvLoad, SIGNAL(clicked()), btnCsvLoad, SLOT(slotClicked()), Qt::AutoConnection);
    QObject::connect(btnCsvSave, SIGNAL(clicked()), btnCsvSave, SLOT(slotClicked()), Qt::AutoConnection);
    QObject::connect(btnTemplLoad, SIGNAL(clicked()), btnTemplLoad, SLOT(slotClicked()), Qt::AutoConnection);
    QObject::connect(btnCfgCreate, SIGNAL(clicked()), btnCfgCreate, SLOT(slotClicked()), Qt::AutoConnection);
}

void NNCGMainWindow::refreshTable() {
    this->hide();
    logoLabel->setPixmap(*objTempl->getPtrPixLogo());
    titleLabel->setText(objTempl->getTitle());
    commentLabel->setText(objTempl->getComment());
    table->clear();
    table->setHorizontalHeaderLabels({QObject::tr("#"), QObject::tr("Description"), QObject::tr("Value")});
    table->setRowCount(objTempl->hashVars.size());
    for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
        QTableWidgetItem *oneRow[3];
        oneRow[0] = new QTableWidgetItem(QString::number(hIt.value().orderNum + 1), 0);
        oneRow[0]->setFont(twiTah);
        oneRow[0]->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        oneRow[0]->setFlags(Qt::NoItemFlags);
        oneRow[1] = new QTableWidgetItem(hIt.value().descr, 0);
        oneRow[1]->setFont(twiTahBold);
        oneRow[1]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        oneRow[1]->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
        oneRow[2] = new QTableWidgetItem(hIt.value().value, 0);
        oneRow[2]->setFont(twiCourNew);
        //oneRow[2]->setForeground(QColor(100, 10, 255, 255));
        oneRow[2]->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
        for (int col = 0; col < 3; col++) table->setItem(hIt.value().orderNum, col, oneRow[col]);
    };
    this->show();
   // std::cout << "refresh called! " << std::endl;
};

// при событии закрытия окна сохраняем настройки в json
void NNCGMainWindow::closeEvent(QCloseEvent* event) {
//    std::cout << "--------------" << std::endl;
//    std::cout << "close window event" << std::endl;
//    std::cout << "is maximized: " << isMaximized() << std::endl;
    this->hide();
    if (!objTempl->isDemo) objSett.saveSettings(objTempl->getFilePath() + objTempl->getFileName(), QS_DARK, width(), height(), b2s(isMaximized()));
    else objSett.saveSettings("", QS_DARK, width(), height(), b2s(isMaximized()));
    event->accept();
}

void NNCGMainWindow::dumpTableToHash() {
//    std::cout << "table rows : " <<  table->rowCount() << std::endl;
    for (int r = 0; r < this->table->rowCount(); r++) {
        // обход хэша
        for (QHash<QString, oneRec_t>::iterator hIt = objTempl->hashVars.begin(); hIt != objTempl->hashVars.end(); ++hIt) {
            if (hIt.value().orderNum == r) {
                objTempl->hashVars[hIt.key()].value = table->item(r, 2)->text();
                break;
            }
        }
    }
}
