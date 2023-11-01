#pragma once

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(const QJsonObject &message, QWidget *parent = nullptr);
    ~MessageWidget();
    QJsonObject _message;
    void wedgetSelectedEditUpdateUi();

private:
    Ui::MessageWidget *ui;
    void setUpUIData();
};

