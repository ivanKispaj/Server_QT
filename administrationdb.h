#pragma once

#include <QMainWindow>
#include "mysqldatabase.h"
#include <QModelIndex>

namespace Ui {
class AdministrationDB;
}

class AdministrationDB : public QMainWindow
{
    Q_OBJECT

signals:
    void  closeWindowAdmin();
public:

    explicit AdministrationDB(QWidget *parent = nullptr);
    ~AdministrationDB();    
private slots:
    void on_wActionSelectUsers_clicked();

    void on_wActionSelectMessages_clicked();

    void on_wActionDelete_clicked();

    void on_wActionEdit_clicked();


    void on_wActionExit_clicked();

    void closeEvent (QCloseEvent *) override;

    void onItemClicked(const QModelIndex &index);
    void onItemClearFocus();
    void on_wActionSave_clicked();

private:
    Ui::AdministrationDB *ui;
    MysqlDataBase *_db = new MysqlDataBase();
    void setUIDataUsers();
    void setUIDataMessages();
    bool _isuserTableOnDisplay{true};
    QModelIndex _selectedItemIndex;
};

