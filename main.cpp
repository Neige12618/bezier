#include <QApplication>
#include "MainWindow.h"
#include "MainOpenGLWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto *p = new MainWindow;
    p->show();
    return QApplication::exec();
}
