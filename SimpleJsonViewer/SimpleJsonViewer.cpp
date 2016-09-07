#include "stdafx.h"
#include "SimpleJsonViewer.h"
#include "JsonItemModel.h"

SimpleJsonViewer::SimpleJsonViewer(QWidget *parent) : QMainWindow(parent)
{
    _ui.setupUi(this);
    _ui.actionOpen->setShortcuts(QKeySequence::Open);

    QObject::connect(_ui.actionOpen, &QAction::triggered, this, &SimpleJsonViewer::openNew);
    QObject::connect(_ui.actionOpenLast, &QAction::triggered, this, &SimpleJsonViewer::openLast);
    QObject::connect(_ui.actionExit, &QAction::triggered, this, &QApplication::quit);
    QObject::connect(_ui.actionExpandTree, &QAction::triggered, _ui.tvJSON, &QTreeView::expandAll);
    QObject::connect(_ui.actionCollapseTree, &QAction::triggered, _ui.tvJSON, &QTreeView::collapseAll);

    _jsonItemModel = new JsonItemModel(this);
    _ui.tvJSON->setModel(_jsonItemModel);

    loadLastFileName();
}

SimpleJsonViewer::~SimpleJsonViewer()
{
}

void SimpleJsonViewer::openNew()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON file", QString(), "JSON Files (*.*)");
    if (!fileName.isNull())
        open(fileName);
}

void SimpleJsonViewer::openLast()
{
    open(_lastFileName);
}

void SimpleJsonViewer::open(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", "Failed to open json file");
        return;
    }

    QString error;
    if (_jsonItemModel->loadData(file.readAll(), error))
    {
        _lastFileName = fileName;
        saveLastFileName();
        _ui.actionOpenLast->setEnabled(true);
    }
    else
        QMessageBox::critical(this, "Error", "Failed to load json data: " + error);
}

void SimpleJsonViewer::loadLastFileName()
{
    QFile file(qApp->applicationDirPath() + "\\last");
    if (file.open(QIODevice::ReadOnly))
    {
        _lastFileName = file.readLine();
        _ui.actionOpenLast->setEnabled(!_lastFileName.isEmpty());
    }
}

void SimpleJsonViewer::saveLastFileName()
{
    QFile file(qApp->applicationDirPath() + "\\last");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        file.write(_lastFileName.toLatin1());
}
