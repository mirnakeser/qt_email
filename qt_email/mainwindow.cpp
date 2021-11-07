#include "mainwindow.h"

#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QApplication>
#include <QPixmap>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <QTableView>    // MVC pattern
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>

#include <QMessageBox>
#include <QInputDialog>
#include <QIODevice>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QStatusBar>
#include <QVector>
#include <QStatusTipEvent>

void MainWindow::createIcons()
{
    newIcon = QPixmap(":/Ikone/new.png");
    deleteIcon = QPixmap(":/Ikone/delete.png");
    saveIcon = QPixmap(":/Ikone/save.png");
    saveAsIcon = QPixmap(":/Ikone/saveas.ico");

    openIcon = QPixmap(":/Ikone/open.png");
    quitIcon = QPixmap(":/Ikone/close.png");
    deleteAllIcon = QPixmap(":/Ikone/clear.png");
    logoIcon = QPixmap(":/Ikone/window_logo.png");
}

void MainWindow::createActions()
{
    saveAction = new QAction(saveIcon, "Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = new QAction(saveAsIcon, "Save as", this);
    openAction = new QAction(openIcon, "Open", this);
    openAction->setShortcut(Qt::CTRL + Qt::Key_O);
    deleteRecordAction = new QAction(deleteIcon, "Delete", this);
    deleteAllAction = new QAction(deleteAllIcon, "Delete all", this);

    quitAction = new QAction(quitIcon, "Quit", this);
    quitAction->setShortcut(QKeySequence::Quit);

    aboutAction = new QAction("About", this);
    aboutAction->setShortcut(Qt::CTRL + Qt::Key_H);
}

void MainWindow::createMenuBar()
{
    fileMenu = menuBar()->addMenu("&File");
    helpMenu = menuBar()->addMenu("Help");

    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(openAction);

    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBar()
{
    QToolBar * toolBar = addToolBar("Main tool bar");
    toolBar->setMovable(false);

    toolBar->addAction(saveAction);
    toolBar->addAction(openAction);
    toolBar->addSeparator();

    toolBar->addAction(deleteRecordAction);
    toolBar->addAction(deleteAllAction);
    toolBar->addAction(quitAction);
}

void MainWindow::setupCoreWidgets()
{
    widget = new QWidget();

    vbLayout = new QVBoxLayout();
    hbLayout = new QHBoxLayout();
    formLayout = new QFormLayout();

    iipLineEdit = new QLineEdit();
    emailLineEdit = new QLineEdit();
    napomenaLineEdit = new QLineEdit();

    QRegularExpression rxNoComma("[^,]*");
    QValidator * validatorNoComma = new QRegularExpressionValidator(rxNoComma, this);
    iipLineEdit->setValidator(validatorNoComma);
    napomenaLineEdit->setValidator(validatorNoComma);

    formLayout->addRow("Ime i prezime:", iipLineEdit);
    formLayout->addRow("Email:", emailLineEdit);
    formLayout->addRow("Napomena:", napomenaLineEdit);

    tableView = new QTableView();
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableModel = new QStandardItemModel(1, 3, this);
    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Ime i prezime")));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Email")));
    tableModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Napomena")));

    QStandardItem * first = new QStandardItem(QString("John Doe"));
    QStandardItem * second = new QStandardItem(QString("johnDoe@math.hr"));
    QStandardItem * third = new QStandardItem(QString("Računarstvo"));
    tableModel->setItem(0,0, first);
    tableModel->setItem(0,1, second);
    tableModel->setItem(0,2, third);

    tableView->setModel(tableModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    saveButton = new QPushButton("Save");
    clearAllButton = new QPushButton("Clear All");

    hbLayout->addStretch();
    hbLayout->addWidget(saveButton);
    hbLayout->addWidget(clearAllButton);

    vbLayout->addLayout(formLayout);
    vbLayout->addWidget(tableView);
    vbLayout->addLayout(hbLayout);

    widget->setLayout(vbLayout);
    setCentralWidget(widget);
}

void MainWindow::setupSignalsAndSlots()
{
    connect(quitAction, &QAction::triggered, &QApplication::quit);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveButtonClicked);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::aboutDialog);
    connect(clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAllRecords);
    connect(deleteAllAction, &QAction::triggered, this, &MainWindow::clearAllRecords);
    connect(deleteRecordAction, &QAction::triggered, this, &MainWindow::deleteSavedRecord);
    connect(openAction,  &QAction::triggered, this, &MainWindow::otvaranjeDatoteke);
    connect(this, SIGNAL(mojSignal(QString)), statusBar(), SLOT(showMessage(QString)));
    connect(saveAction, &QAction::triggered, this, &MainWindow::datotekaSave);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::datotekaSaveAs);

}

void MainWindow::clearFields()
{
    iipLineEdit->clear();
    emailLineEdit->clear();
    napomenaLineEdit->clear();
}

void MainWindow::saveButtonClicked()
{
    QString name = iipLineEdit->text();
    QString email = emailLineEdit->text();
    QString napomena = napomenaLineEdit->text();

    QRegularExpression rxEmail("[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+");
    QValidator * validatorEmail = new QRegularExpressionValidator(rxEmail, this);
    int pos = 0;

    if( validatorEmail->validate(email, pos) == QValidator::Acceptable ){
            QStandardItem * it1{new QStandardItem(name)};
            QStandardItem * it2{new QStandardItem(email)};
            QStandardItem * it3{new QStandardItem(napomena)};

           // it1->setEditable(false);
            //it2->setEditable(false);
            //it3->setEditable(false);

            tableModel->appendRow({it1, it2, it3});

            clearFields();

            if(flagOtvorenaDatoteka)
                flagDatotekaPromijenjena = true;
        }
        else
            QMessageBox::about(this, "Neispravan email", "Email adresa nije ispravnog oblika.");

}

void MainWindow::aboutDialog()
{
    QMessageBox::about(this, "About Application", "My app d.o.o.");
}

void MainWindow::clearAllRecords()
{
    auto button = QMessageBox::question(this, "Clear all", "Želite li baš sve obrisati????");
    if(button == QMessageBox::Yes)
    {
        tableModel->removeRows(0,tableModel->rowCount());

        if(flagOtvorenaDatoteka)
            flagDatotekaPromijenjena = true;
    }
}

void MainWindow::deleteSavedRecord()
{
    bool ok = true;
    int row = QInputDialog::getInt(this, tr("Select row to delete"),
                                    tr("Row ID to be deleted"), 1, 1, tableModel->rowCount(), 1, &ok);
    if(ok)
    {
        tableModel->removeRow(row-1);

        if(flagOtvorenaDatoteka)
            flagDatotekaPromijenjena = true;
    }
}

void MainWindow::otvaranjeDatoteke()
{
    if(flagDatotekaPromijenjena)
    {
        auto button = QMessageBox::question(this, "Save", "Želite li spremiti promjene?");
        if(button == QMessageBox::Yes)
            datotekaSave();
    }

    if(flagOtvorenaDatoteka)
        flagDatotekaPromijenjena = false;

    QFile file;

    fileName = QFileDialog::getOpenFileName(this, "OpenFile", ".", "Text files (*.txt)");
    if(fileName == ""){
        qDebug() << "Canceled.";
    }
    else{
        tableModel->removeRows(0,tableModel->rowCount());

        file.setFileName(fileName);
        file.open(QIODevice::ReadWrite);

        QTextStream stream(&file);
        while(!stream.atEnd()){
            QString line = stream.readLine();
            qDebug() << line;
            QStringList redakTablice = line.split(QLatin1Char(','));
            QString name = redakTablice[0];//.at(0).toLocal8Bit().constData();
            QString email = redakTablice[1];//.at(1).toLocal8Bit().constData();
            QString napomena = redakTablice[2];//.at(2).toLocal8Bit().constData();

            QStandardItem * it1{new QStandardItem(name)};
            QStandardItem * it2{new QStandardItem(email)};
            QStandardItem * it3{new QStandardItem(napomena)};

            tableModel->appendRow({it1, it2, it3});
        }
        fileNameDijelovi = new QStringList(fileName.split(QRegularExpression("/")));
        emit mojSignal(fileNameDijelovi->last());
        if(!flagOtvorenaDatoteka) flagOtvorenaDatoteka = true;
    }
    file.close();
}



void MainWindow::datotekaSave()
{
    QFile file;

    if(!flagOtvorenaDatoteka && tableModel->rowCount())
        datotekaSaveAs();

    else if(flagDatotekaPromijenjena)
    {
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) //otvaramo ju prvo
              qDebug() << "File: Error.";
        else
              qDebug() << "File: Opened.";
        file.resize(fileName, 0);
        QTextStream stream(&file);
        for (int i=0; i<tableModel->rowCount(); ++i)
                stream << tableModel->item(i,0)->text() << ',' << tableModel->item(i,1)->text() << ',' << tableModel->item(i,2)->text() << "\n";
        file.close();
        flagDatotekaPromijenjena = false;
    }
}

void MainWindow::datotekaSaveAs()
{
    QFile file;
    fileName = QFileDialog::getOpenFileName(this, "OpenFile", ".", "Text files (*.txt)");
    if(fileName == ""){
        qDebug() << "Canceled.";
    }
    else{
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) //otvaramo ju prvo
              qDebug() << "File: Error.";
        else
              qDebug() << "File: Opened.";
        file.resize(fileName, 0);
        QTextStream stream(&file);
        for (int i=0; i<tableModel->rowCount(); ++i)
                stream << tableModel->item(i,0)->text() << ',' << tableModel->item(i,1)->text() << ',' << tableModel->item(i,2)->text() << "\n";
        fileNameDijelovi = new QStringList(fileName.split(QRegularExpression("/")));
        emit mojSignal(fileNameDijelovi->last());
        if(!flagOtvorenaDatoteka) flagOtvorenaDatoteka = true;
        flagDatotekaPromijenjena = false;
        file.close();

    }
}

bool MainWindow::event(QEvent* e)
{
    if( dynamic_cast<QStatusTipEvent*>(e) != nullptr )
        return true;
    return QMainWindow::event(e);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("EMail adrese");
    resize(400,200);
    createIcons();
    setWindowIcon(logoIcon);

    createActions();
    createMenuBar();
    createToolBar();
    setupCoreWidgets();
    setupSignalsAndSlots();

}

MainWindow::~MainWindow()
{

}
