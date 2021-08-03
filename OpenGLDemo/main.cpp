#include "glslwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLSLWidget w;
    w.resize(800,600);//设置大小
    w.show();//显示窗口

    return a.exec();
}
