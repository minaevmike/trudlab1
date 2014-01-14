#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setMaximumSize(800, 800);
    w.setMinimumSize(800, 800);
    w.show();

    return a.exec();
}
