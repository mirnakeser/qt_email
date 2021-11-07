#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QMenu;
class QPushButton;
class QLineEdit;
class QStandardItemModel;
class QLabel;
class QFormLayout;
class QVBoxLayout;
class QHBoxLayout;
class QTableView;
class QFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void createIcons();
    void createActions();
    void createMenuBar();
    void createToolBar();
    void setupCoreWidgets();
    void setupSignalsAndSlots();
    void clearFields();
    //void prepisivanjeDatoteke(QFile f);
    bool event(QEvent* e) override;

    QPixmap newIcon;
    QPixmap openIcon;
    QPixmap quitIcon;
    QPixmap deleteIcon;
    QPixmap saveIcon;
    QPixmap saveAsIcon;
    QPixmap deleteAllIcon;
    QPixmap logoIcon;

    QAction * newAction;
    QAction * openAction;
    QAction * quitAction;
    QAction * aboutAction;
    QAction * deleteRecordAction;
    QAction * saveAction;
    QAction * saveAsAction;
    QAction * deleteAllAction;

    QMenu * fileMenu;
    QMenu * helpMenu;

    QLineEdit * iipLineEdit;
    QLineEdit * emailLineEdit;
    QLineEdit * napomenaLineEdit;

    QStandardItemModel * tableModel;

    QWidget * widget;

    QVBoxLayout * vbLayout;
    QHBoxLayout * hbLayout;
    QFormLayout * formLayout;

    QTableView * tableView;

    QPushButton * saveButton;
    QPushButton * clearAllButton;

    QString fileName = "";
    //QFile * file;
    QStringList * fileNameDijelovi;

    bool flagOtvorenaDatoteka = false;
    bool flagDatotekaPromijenjena = false;

private slots:
    void saveButtonClicked();
    void aboutDialog();
    void clearAllRecords();
    void deleteSavedRecord();
    void otvaranjeDatoteke();
    void datotekaSave();
    void datotekaSaveAs();

signals:
    void mojSignal(QString s);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
