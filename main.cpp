#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("app");
    app.setApplicationVersion("v0.0.1");

    MainWindow window;
    window.show();

    app.exec();
}
