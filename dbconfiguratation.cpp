#include "dbconfiguratation.h"
#include "ui_dbconfiguratation.h"
#include "FileManager.h"
#include "Constants.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


DBConfiguratation::DBConfiguratation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBConfiguratation)
{
    ui->setupUi(this);
    ui->connectionStatusCheck->setEnabled(false);
    ui->connectionStatusCheck->setStyleSheet("#connectionStatusCheck {background-color:  rgba(255, 0, 21, 128)}");

}

DBConfiguratation::~DBConfiguratation()
{
    delete ui;
}




void DBConfiguratation::on_dbConfigActionBox_accepted()
{
    on_testConnectButton_clicked();
    if (this->ui->connectionStatusCheck->isChecked())
    {
        std::string filePath = (Constants::dir.baseDIR + Constants::dir.dataDIR + Constants::dir.configFileName);

        FileManager::File file (filePath,FileManager::save,true,false);
        std::string host = "host = \"" + ui->dbHostTextField->text().toStdString() + "\"";
        std::string dbName = "dataBaseName = \"" + ui->dbNameTextField->text().toStdString() + "\"";
        std::string user = "userLogin = \"" + ui->dbUserTextField->text().toStdString() + "\"";
        std::string pass = "userPassword = \"" + ui->dbUserPassTextField->text().toStdString() + "\"";
        file.saveData(host);
        file.saveData(dbName);
        file.saveData(user);
        file.saveData(pass);
        accept();
    }
}


void DBConfiguratation::on_dbConfigActionBox_rejected()
{
    if (ui->connectionStatusCheck->isChecked())
    {
        accept();
    } else
    {
        reject();

    }
}


void DBConfiguratation::on_testConnectButton_clicked()
{

    if (ui->dbHostTextField->text().isEmpty() || ui->dbNameTextField->text().isEmpty() ||
        ui->dbUserPassTextField->text().isEmpty() || ui->dbUserTextField->text().isEmpty())
    {
        this->ui->connectionStatusCheck->setCheckState(Qt::Unchecked);
        this->ui->connectionStatusCheck->setStyleSheet("#connectionStatusCheck {background-color:  rgba(236, 80, 58, 200)}");
    } else
    {
        QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "testConnection"));
        db->setDatabaseName(ui->dbNameTextField->text());
        db->setHostName(ui->dbHostTextField->text());
        db->setUserName(ui->dbUserTextField->text());
        db->setPassword(ui->dbUserPassTextField->text());

        if (db->open())
        {
            this->ui->connectionStatusCheck->setCheckState(Qt::Checked);
            this->ui->connectionStatusCheck->setStyleSheet("#connectionStatusCheck {background-color:  rgba(117, 205, 109, 100)}");

        }
        else
        {
            qDebug() << "Ошибка открытия базы данных:" << db->lastError().text();
            this->ui->connectionStatusCheck->setCheckState(Qt::Unchecked);
            this->ui->connectionStatusCheck->setStyleSheet("#connectionStatusCheck {background-color:  rgba(236, 80, 58, 200)}");
        }
        delete db;
    }
    QSqlDatabase::removeDatabase("testConnection");
}

void DBConfiguratation::configureUI(const QString &text)
{
    this->ui->dbFormDescription->setText(text);
    std::string filePath = (Constants::dir.baseDIR + Constants::dir.dataDIR + Constants::dir.configFileName);

    if (std::filesystem::exists(filePath))
    {
        FileManager::File file (filePath,FileManager::load,false,false);
        std::string dataBaseName;
        std::string host;
        std::string user;
        std::string pass;
        file.readStringInsideDelimiters('"', host);
        file.readStringInsideDelimiters('"', dataBaseName);
        file.readStringInsideDelimiters('"', user);
        file.readStringInsideDelimiters('"', pass);
        ui->dbHostTextField->setText(QString::fromStdString(host));
        ui->dbNameTextField->setText(QString::fromStdString(dataBaseName));
        ui->dbUserTextField->setText(QString::fromStdString(user));
        ui->dbUserPassTextField->setText(QString::fromStdString(pass));

    }
    on_testConnectButton_clicked();
}

bool DBConfiguratation::getStateDbConnect() const
{
    return ui->connectionStatusCheck->checkState();
}

