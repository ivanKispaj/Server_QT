#ifndef DBCONFIGURATATION_H
#define DBCONFIGURATATION_H

#include <QDialog>

namespace Ui {
class DBConfiguratation;
}

class DBConfiguratation : public QDialog
{
    Q_OBJECT


public:
    explicit DBConfiguratation(QWidget *parent = nullptr);
    ~DBConfiguratation();
    void configureUI(const QString &text);
    bool getStateDbConnect() const;
private slots:

    void on_dbConfigActionBox_accepted();

    void on_dbConfigActionBox_rejected();

    void on_testConnectButton_clicked();

private:
    Ui::DBConfiguratation *ui;
};

#endif // DBCONFIGURATATION_H
