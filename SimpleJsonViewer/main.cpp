#include "stdafx.h"
#include "SimpleJsonViewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Alexaner Yarikov");
    app.setApplicationName("Simple Json Viewer");
    app.setApplicationVersion("1.0.0");

    SimpleJsonViewer wnd;
    wnd.show();

    try
    {
        return app.exec();
    }
    catch (const std::bad_alloc& e)
    {
        QMessageBox::critical(Q_NULLPTR, qAppName(), "Not enough memory to proceed, application will be terminated");
        return -1;
    }
}
