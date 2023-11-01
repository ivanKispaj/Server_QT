#include "administrationdb.h"
#include "ui_administrationdb.h"
#include "userwidget.h"
#include "messagewidget.h"
#include <QListWidgetItem>

AdministrationDB::AdministrationDB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdministrationDB)
{
    ui->setupUi(this);
    setUIDataUsers();
    connect(ui->listWidget, &QListView::clicked, this, &AdministrationDB::onItemClicked);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &AdministrationDB::onItemClearFocus);
}

AdministrationDB::~AdministrationDB()
{
    delete ui;
}

void AdministrationDB::on_wActionSelectUsers_clicked()
{
    if(!_isuserTableOnDisplay)
    {
        ui->listWidget->clear();
        setUIDataUsers();
        _isuserTableOnDisplay = true;
        ui->wActionEdit->setHidden(false);
        ui->wActionSave->setHidden(false);
    }
}


void AdministrationDB::on_wActionSelectMessages_clicked()
{
    if(_isuserTableOnDisplay)
    {
        ui->listWidget->clear();
        setUIDataMessages();
        _isuserTableOnDisplay = false;
        ui->wActionEdit->hide();
        ui->wActionSave->hide();
    }
}

void AdministrationDB::on_wActionDelete_clicked()
{
    if(_selectedItemIndex.isValid())
    {
        QListWidgetItem *item = ui->listWidget->itemFromIndex(_selectedItemIndex);
        UserWidget* itemUserWidget = qobject_cast<UserWidget*>(ui->listWidget->itemWidget(item));

        if(itemUserWidget)
        {
            itemUserWidget->wedgetSelectedEndEditUpdateUi();
            QJsonObject userObj = itemUserWidget->_user;
            User user;
            user.decodeFromJson(userObj);
            if (_db->deleteUser(user))
            {
                ui->listWidget->clear();
                setUIDataUsers();
            } else
            {
                qDebug()<< "Ошибка обновления пользлвателя!";
            }
        }
        MessageWidget* itemMessageWidget = qobject_cast<MessageWidget*>(ui->listWidget->itemWidget(item));
        if (itemMessageWidget)
        {
            itemMessageWidget->wedgetSelectedEditUpdateUi();
            QJsonObject userObj = itemMessageWidget->_message;
            Message message;
            message.decodeFromJson(userObj);
            if (_db->deleteMessageById(message.getId()))
            {
                ui->listWidget->clear();
                setUIDataMessages();
            } else
            {
                qDebug()<< "Ошибка обновления пользлвателя!";
            }
        }

    }
}

void AdministrationDB::on_wActionEdit_clicked()
{
    QListWidgetItem *item = ui->listWidget->itemFromIndex(_selectedItemIndex);
    UserWidget* itemUserWidget = qobject_cast<UserWidget*>(ui->listWidget->itemWidget(item));
    if (itemUserWidget)
    {
        itemUserWidget->wedgetSelectedEditUpdateUi();
    }
}



void AdministrationDB::on_wActionExit_clicked()
{
    this->close();

}

void AdministrationDB::closeEvent(QCloseEvent *)
{
    emit closeWindowAdmin();
}

void AdministrationDB::onItemClicked(const QModelIndex &index)
{
    _selectedItemIndex = index;
}

void AdministrationDB::onItemClearFocus()
{
    on_wActionSave_clicked();
    _selectedItemIndex = QModelIndex();

}

void AdministrationDB::setUIDataUsers()
{
    QJsonArray users = _db->getAllUser();

    for(int i = 0; i < users.count(); i++)
    {
        QJsonObject user = users[i].toObject();
        if (!user["is_deleted"].toBool())
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            UserWidget *uWidget = new UserWidget(user, this);
            item->setSizeHint(QSize(200, 170));
            QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            uWidget->setSizePolicy(sizePolicy);
            ui->listWidget->setItemWidget(item, uWidget);
        }

    }

}

void AdministrationDB::setUIDataMessages()
{
    QJsonArray messages = _db->getAllMessages(MysqlDataBase::allmesage);

    for(int i = 0; i < messages.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        QJsonObject message = messages[i].toObject();
        MessageWidget *uWidget = new MessageWidget(message, this);
        item->setSizeHint(QSize(200, 150));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        uWidget->setSizePolicy(sizePolicy);
        ui->listWidget->setItemWidget(item, uWidget);
    }

}


void AdministrationDB::on_wActionSave_clicked()
{
    if(_selectedItemIndex.isValid())
    {
        QListWidgetItem *item = ui->listWidget->itemFromIndex(_selectedItemIndex);
        UserWidget* itemUserWidget = qobject_cast<UserWidget*>(ui->listWidget->itemWidget(item));
        if(itemUserWidget)
        {
            itemUserWidget->wedgetSelectedEndEditUpdateUi();
            QJsonObject userObj = itemUserWidget->_user;
            User user;
            user.decodeFromJson(userObj);
            if (_db->updateUser(user))
            {

            } else
            {
                qDebug()<< "Ошибка обновления пользлвателя!";
            }

        }
    }
}

