#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dbconfiguratation.h"
#include "Constants.h"
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include "administrationdb.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!this->presentDBForm(tr("Please enter the data to connect to the mysql database!")))
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Серверу требуются данные для подключения к базе данных!"));
        QTimer::singleShot(1, qApp, &QCoreApplication::quit);
    }
    QTimer::singleShot(0, this, &MainWindow::on_startServer_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete _server;
}


void MainWindow::on_startServer_clicked()
{
    if (!_server)
    {
        startServer();
        _server = new TCPServer();
        connect(_server, &TCPServer::updateUI, this, &MainWindow::updateLabel);
        if (_server->isConnected())
        {
            ui->startServer->setStyleSheet("QPushButton {"
                                           "    background-color: rgb(117,205,109)"
                                           "}");
        } else
        {
            qDebug() << "Ошибка при запуске сервера";
            ui->startServer->setStyleSheet("QPushButton {"
                                           "    background-color: rgb(236,85,59)"
                                           "}");
        }
    }
}


void MainWindow::on_stopServer_clicked()
{

    if (_server)
    {
        delete _server;
        _server = nullptr;
        ui->startServer->setStyleSheet("");
    }
}


void MainWindow::on_disconnectClient_clicked()
{
    on_stopServer_clicked();
}


bool MainWindow::presentDBForm(const QString &title, bool isPresent)
{
    std::string filePath = (Constants::dir.baseDIR + Constants::dir.dataDIR + Constants::dir.configFileName);
    DBConfiguratation dbConf;
    dbConf.setWindowTitle(tr("Database configuration"));
    dbConf.configureUI(title);
    _stateServerConf = true;
    if (dbConf.getStateDbConnect() && !isPresent)
    {
        return true;
    }
    auto result = dbConf.exec();
    if (result == QDialog::Rejected)
    {
        _stateServerConf = false;
        return false;
    }
    return true;
}


void MainWindow::on_actionConfigure_triggered()
{

    if(!presentDBForm(tr("Edit DB settings"), true))
    {
        on_stopServer_clicked();
    }
}

void MainWindow::startServer()
{
    if (_stateServerConf)
    {
        QThread* thread = new QThread(this);
        _server = new TCPServer();
        _server->moveToThread(thread);
    }
}

void MainWindow::updateLabel(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(message);
    ui->listWidget->addItem(item);
}


void MainWindow::on_actionadministration_triggered()
{
    on_stopServer_clicked();

    this->setEnabled(false);
    AdministrationDB *admin = new AdministrationDB();
    _consoleAdmin = admin;
    connect(_consoleAdmin, &AdministrationDB::closeWindowAdmin, this, &MainWindow::administrationClose);
    _consoleAdmin->show();
}

void MainWindow::administrationClose()
{
        this->setEnabled(true);
        delete _consoleAdmin;
        _consoleAdmin = nullptr;
        on_startServer_clicked();
}










