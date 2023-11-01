#include "userwidget.h"
#include "ui_userwidget.h"
#include "User.h"

UserWidget::UserWidget(const QJsonObject &user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserWidget)
{
    ui->setupUi(this);
    _user = user;
    setUIData();
    connect(ui->wUserNameEdit, &QLineEdit::editingFinished, this, &UserWidget::userNameEndEditing);
    connect(ui->wuserIsAdmin, &QCheckBox::stateChanged, this, &UserWidget::isAdminUserChecked);
    connect(ui->wUserIsBanned, &QCheckBox::stateChanged, this, &UserWidget::isBannedUserChecked);

}

UserWidget::~UserWidget()
{
    delete ui;

}

void UserWidget::wedgetSelectedEditUpdateUi()
{
    ui->wUserNameEdit->setEnabled(true);
    ui->wuserIsAdmin->setEnabled(true);
    ui->wUserIsBanned->setEnabled(true);
}

void UserWidget::wedgetSelectedEndEditUpdateUi()
{
    ui->wUserNameEdit->setEnabled(false);
    ui->wuserIsAdmin->setEnabled(false);
    ui->wUserIsBanned->setEnabled(false);
}

void UserWidget::userNameEndEditing()
{
    _user["name"] = ui->wUserNameEdit->text();

}

void UserWidget::isAdminUserChecked()
{
    bool state = ui->wuserIsAdmin->checkState();
    _user["is_admin"] = state;
}

void UserWidget::isBannedUserChecked()
{
     bool state = ui->wUserIsBanned->checkState();
    _user["is_banned"] = state;

}

void UserWidget::setUIData()
{
    User user;
    user.decodeFromJson(_user);
    ui->userWidgetId->setTitle("id: " + QString::number(user.getId()));
    ui->wUserNameEdit->setText(user.getUserName());
    ui->wUserLoginEdit->setText(user.getUserLogin());
    if (user.isAdmin())
    {
        ui->wuserIsAdmin->setCheckState(Qt::CheckState::Checked);
    }
    if (user.isBanned())
    {
        ui->wUserIsBanned->setCheckState(Qt::CheckState::Checked);
    }
}
