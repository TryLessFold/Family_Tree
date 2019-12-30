#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QFile File(":/stylesheets.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    a.setStyleSheet(StyleSheet);

    return a.exec();
}
