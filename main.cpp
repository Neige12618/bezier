#include <QApplication>
#include "MainWindow.h"
#include "MainOpenGLWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto *p = new MainWindow;
    auto *gl = new MainOpenGLWidget(p);
    p->setCentralWidget(gl);
    p->show();
    return QApplication::exec();
}
