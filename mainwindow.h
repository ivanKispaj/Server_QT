#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TCPServer.h"
#include "administrationdb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startServer_clicked();
    void on_stopServer_clicked();
    void on_disconnectClient_clicked();
    void on_actionConfigure_triggered();
    void startServer();
    void updateLabel(const QString &message);

    void on_actionadministration_triggered();
    void administrationClose();
private:
    Ui::MainWindow *ui;
    TCPServer *_server {nullptr};
    bool _stateServerConf{false};
    bool presentDBForm(const QString &title, bool isPresent = false);
protected:
    AdministrationDB *_consoleAdmin{nullptr};

};
#endif // MAINWINDOW_H
