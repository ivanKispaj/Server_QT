#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <filesystem>
#include <QMessageBox>
#include "Constants.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTranslator translator;
    QString appPath = Constants::dir.cwd;
    if (!translator.load(appPath + "/Server_translations/"+ "my_ru.qm"))
    {
        QString message = "Failed to upload translations. The 'Server_translations' folder with my_ru.qm file should be located in:  \n" + appPath;
        QMessageBox::critical(nullptr, "Error", message);
    } else
    {
        a.installTranslator(&translator);
    }
    MainWindow w;
    w.setWindowTitle(QObject::tr("ChatServer"));
    w.show();
    return a.exec();
}
