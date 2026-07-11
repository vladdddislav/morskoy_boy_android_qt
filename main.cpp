
#include <QApplication>
#include <ctime>
#include <cstdlib>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}