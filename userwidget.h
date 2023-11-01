#pragma once

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class UserWidget;
}

class UserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserWidget(const QJsonObject &user, QWidget *parent = nullptr);
    ~UserWidget();
    QJsonObject _user;

    void wedgetSelectedEditUpdateUi();
    void wedgetSelectedEndEditUpdateUi();
    void userNameEndEditing();
    void isAdminUserChecked();
    void isBannedUserChecked();
private:
    Ui::UserWidget *ui;
    void setUIData();

};

