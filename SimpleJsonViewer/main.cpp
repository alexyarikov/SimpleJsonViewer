#include "stdafx.h"
#include "SimpleJsonViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleJsonViewer w;
    w.show();
    return a.exec();
}
