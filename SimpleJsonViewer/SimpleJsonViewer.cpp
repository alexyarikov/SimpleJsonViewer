#include "stdafx.h"
#include "SimpleJsonViewer.h"
#include "JsonItemModel.h"

SimpleJsonViewer::SimpleJsonViewer(QWidget *parent) : QMainWindow(parent)
{
    _ui.setupUi(this);
    _ui.actionOpen->setShortcuts(QKeySequence::Open);
    _ui.actionOpen->setStatusTip(tr("Open file..."));
    QObject::connect(_ui.actionOpen, &QAction::triggered, this, &SimpleJsonViewer::open);
    QObject::connect(_ui.actionExit, &QAction::triggered, this, &QApplication::quit);

    _JsonItemModel = new JsonItemModel(this);
    _ui.tvJSON->setModel(_JsonItemModel);
}

SimpleJsonViewer::~SimpleJsonViewer()
{
}

void SimpleJsonViewer::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open JSON file"), QString(), tr("JSON Files (*.*)"));
    if (fileName.isNull())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", "Failed to open json file");
        return;
    }

    QString error;
    if (!_JsonItemModel->loadData(file.readAll(), error))
        QMessageBox::critical(this, "Error", "Failed to load json data: " + error);
}
