#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include "mysqldatabase.h"

int main(int argc, char *argv[])
{


    MysqlDataBase *db = new MysqlDataBase();
    db->createDefaultTable();
    delete db;
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load("translations/my_ru.qm");
    a.installTranslator(&translator);
    MainWindow w;
    w.setWindowTitle(QObject::tr("ChatServer"));
    w.show();
    return a.exec();
}
