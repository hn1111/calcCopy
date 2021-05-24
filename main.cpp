#include "widget.h"
#include "calculate.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Calculator");
    a.setApplicationDisplayName("Calculator");

    Widget w;
    w.show();
    return a.exec();
}
