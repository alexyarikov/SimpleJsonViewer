#pragma once
#include "ui_SimpleJsonViewer.h"

class JsonItemModel;

class SimpleJsonViewer : public QMainWindow
{
    Q_OBJECT

public:
    SimpleJsonViewer(QWidget *parent = 0);
    ~SimpleJsonViewer();

private:
    Ui::SimpleJsonViewerClass _ui;
    QPointer<JsonItemModel> _JsonItemModel;

    void open();
};
