#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("JKS Stock Manager");
    app.setApplicationVersion("v0.0.1");
    app.setWindowIcon(QIcon(":/image/logo.ico"));

    MainWindow window;
    window.show();

    app.exec();
}
