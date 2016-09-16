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
    QObject::connect(_ui.actionOpenFromClipboard, &QAction::triggered, this, &SimpleJsonViewer::openFromClipboard);
    QObject::connect(_ui.actionExit, &QAction::triggered, this, &QApplication::quit);

    // create model
    _jsonItemModel = new JsonItemModel(this);

    // setup json treeview
    setupJsonTreeView();

    // enable/disable OpenLastFile action
    _ui.actionOpenLast->setEnabled(!lastFileName().isEmpty());
}

SimpleJsonViewer::~SimpleJsonViewer()
{
}

QString SimpleJsonViewer::lastFileName() const
{
    QSettings settings;
    return settings.value("LastFileName", "").toString();
}

void SimpleJsonViewer::setupJsonTreeView()
{
    // setup context menu
    _jsonMenu = new QMenu(_ui.tvJson);
    _jsonMenu->addAction("Expand item", this, &SimpleJsonViewer::expandJsonItems);
    _jsonMenu->addAction("Collapse item", this, &SimpleJsonViewer::collapseJsonItems);

    _ui.tvJson->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_ui.tvJson, &QWidget::customContextMenuRequested, this, &SimpleJsonViewer::jsonContextMenu);

    // set view model
    _ui.tvJson->setModel(_jsonItemModel);
}

void SimpleJsonViewer::openNew()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open json file", QString(), "Json Files (*.*)");
    if (!fileName.isNull() && open(fileName))
    {
        QSettings settings;
        settings.setValue("LastFileName", fileName);
        _ui.actionOpenLast->setEnabled(true);
    }
}

void SimpleJsonViewer::openLast()
{
    open(lastFileName());
}

void SimpleJsonViewer::openFromClipboard()
{
    QString clipboardText = QApplication::clipboard()->text();
    if (!clipboardText.isEmpty())
        open(clipboardText.toLatin1());
}

bool SimpleJsonViewer::open(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", QString("Failed to open json file at path %1").arg(fileName));
        return false;
    }

    return open(file.readAll());
}

bool SimpleJsonViewer::open(const QByteArray& jsonData)
{
    QString error;
    if (_jsonItemModel->loadData(jsonData, error))
    {
        _ui.teJson->setPlainText(_jsonItemModel->rawData(QJsonDocument::Indented));
        _ui.actionFormat->setEnabled(true);
        return true;
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to load json data: " + error);
        return false;
    }
}

void SimpleJsonViewer::jsonContextMenu(const QPoint& point)
{
    QModelIndex index = _ui.tvJson->indexAt(point);
    if (index.isValid())
    {
        _jsonMenuIndex = &index;
        _jsonMenu->exec(_ui.tvJson->mapToGlobal(point));
    }
}

void SimpleJsonViewer::expandJsonItems()
{
    if (_jsonMenuIndex)
    {
        qApp->setOverrideCursor(Qt::WaitCursor);
        expandJsonItem(*_jsonMenuIndex);
        qApp->restoreOverrideCursor();
    }
}

void SimpleJsonViewer::expandJsonItem(QModelIndex& index)
{
    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; ++i)
        expandJsonItem(index.child(i, 0));
    _ui.tvJson->expand(index);
}

void SimpleJsonViewer::collapseJsonItems()
{
    if (_jsonMenuIndex)
    {
        qApp->setOverrideCursor(Qt::WaitCursor);
        collapseJsonItem(*_jsonMenuIndex);
        qApp->restoreOverrideCursor();
    }
}

void SimpleJsonViewer::collapseJsonItem(QModelIndex& index)
{
    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; ++i)
        collapseJsonItem(index.child(i, 0));
    _ui.tvJson->collapse(index);
}
