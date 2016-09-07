#include "stdafx.h"
#include "SimpleJsonViewer.h"
#include "JsonItemModel.h"

SimpleJsonViewer::SimpleJsonViewer(QWidget *parent) : QMainWindow(parent)
{
    // setup UI
    _ui.setupUi(this);
    _ui.actionOpen->setShortcuts(QKeySequence::Open);

    // setup connections
    QObject::connect(_ui.actionOpen, &QAction::triggered, this, &SimpleJsonViewer::openNew);
    QObject::connect(_ui.actionOpenLast, &QAction::triggered, this, &SimpleJsonViewer::openLast);
    QObject::connect(_ui.actionExit, &QAction::triggered, this, &QApplication::quit);

    // create model
    _jsonItemModel = new JsonItemModel(this);

    // setup json treeview context menu
    setupJsonTreeView();

    // load last open file name from text file
    loadLastFileName();
}

SimpleJsonViewer::~SimpleJsonViewer()
{
}

void SimpleJsonViewer::setupJsonTreeView()
{
    // setup context menu
    _jsonMenu = new QMenu(_ui.tvJSON);
    _jsonMenu->addAction("Expand item", this, &SimpleJsonViewer::expandJsonItem);
    _jsonMenu->addAction("Collapse item", this, &SimpleJsonViewer::collapseJsonItem);

    _ui.tvJSON->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_ui.tvJSON, &QWidget::customContextMenuRequested, this, &SimpleJsonViewer::jsonContextMenu);

    // set view model
    _ui.tvJSON->setModel(_jsonItemModel);
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

void SimpleJsonViewer::jsonContextMenu(const QPoint& point)
{
    QModelIndex index = _ui.tvJSON->indexAt(point);
    if (index.isValid())
    {
        _jsonMenuIndex = &index;
        _jsonMenu->exec(_ui.tvJSON->mapToGlobal(point));
    }
}

void SimpleJsonViewer::expandJsonItem()
{
    if (_jsonMenuIndex)
        _ui.tvJSON->expand(*_jsonMenuIndex);
}

void SimpleJsonViewer::collapseJsonItem()
{
    if (_jsonMenuIndex)
        _ui.tvJSON->collapse(*_jsonMenuIndex);
}
