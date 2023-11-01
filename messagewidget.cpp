#include "messagewidget.h"
#include "ui_messagewidget.h"
#include "Message.h"

MessageWidget::MessageWidget(const QJsonObject &message, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
    _message = message;
    setUpUIData();
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

void MessageWidget::setUpUIData()
{
    Message message;
    message.decodeFromJson(_message);
    ui->wMessageId->setTitle("id: " + QString::number(message.getId()));
    ui->wMessageTextLable->setText(message.getMessage());
    ui->wMessageAuthorLable->setText(tr("Author id:  ") + QString::number(message.getAuthorID()));
    ui->wMessageRecipientLable->setText(tr("Recipient id:  ") + QString::number(message.getRecipientID()));
}

void MessageWidget::wedgetSelectedEditUpdateUi()
{

}
