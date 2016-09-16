#include "stdafx.h"
#include "SimpleJsonViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Alexaner Yarikov");
    a.setApplicationName("Simple Json Viewer");
    a.setApplicationVersion("1.0.0");

    SimpleJsonViewer w;
    w.show();
    return a.exec();
}
