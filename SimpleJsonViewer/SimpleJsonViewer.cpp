#include "stdafx.h"
#include "SimpleJsonViewer.h"
#include "JsonItemModel.h"
#include "Preferences.h"
#include "PreferencesDialog.h"

SimpleJsonViewer::SimpleJsonViewer(QWidget *parent) : QMainWindow(parent)
{
    // setup UI
    _ui.setupUi(this);
    _ui.actionOpen->setShortcuts(QKeySequence::Open);

    // setup connections
    QObject::connect(_ui.actionOpen, &QAction::triggered, this, &SimpleJsonViewer::onOpenNew);
    QObject::connect(_ui.actionOpenLast, &QAction::triggered, this, &SimpleJsonViewer::onOpenLast);
    QObject::connect(_ui.actionOpenFromClipboard, &QAction::triggered, this, &SimpleJsonViewer::onOpenFromClipboard);
    QObject::connect(_ui.actionExit, &QAction::triggered, this, &QApplication::quit);
    QObject::connect(_ui.actionPreferences, &QAction::triggered, this, [this](){ PreferencesDialog dlg(this); dlg.exec(); });
    QObject::connect(&Preferences::getInstance(), &Preferences::showRawJsonChanged, [this](bool value){ _ui.tabWidget->setTabEnabled(1, value); });

    // create model
    _jsonItemModel = new JsonItemModel(this);

    // setup json treeview
    setupJsonTreeView();

    // enable/disable OpenLastFile action
    _ui.actionOpenLast->setEnabled(!lastFileName().isEmpty());

    // enable/disable raw data tab
    _ui.tabWidget->setTabEnabled(1, Preferences::getInstance().showRawJson());
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
    _jsonMenu->addAction("Expand item", this, &SimpleJsonViewer::onExpandJsonItems);
    _jsonMenu->addAction("Collapse item", this, &SimpleJsonViewer::onCollapseJsonItems);

    _ui.tvJson->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_ui.tvJson, &QWidget::customContextMenuRequested, this, &SimpleJsonViewer::onJsonContextMenu);

    // set view model
    _ui.tvJson->setModel(_jsonItemModel);
}

void SimpleJsonViewer::onOpenNew()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open json file", QString(), "Json Files (*.*)");
    if (!fileName.isNull() && open(fileName))
    {
        QSettings settings;
        settings.setValue("LastFileName", fileName);
        _ui.actionOpenLast->setEnabled(true);
    }
}

void SimpleJsonViewer::onOpenLast()
{
    open(lastFileName());
}

void SimpleJsonViewer::onOpenFromClipboard()
{
    QString clipboardText = QApplication::clipboard()->text();
    if (!clipboardText.isEmpty())
        open(clipboardText.toLatin1());
}

void SimpleJsonViewer::onJsonContextMenu(const QPoint& point)
{
    QModelIndex index = _ui.tvJson->indexAt(point);
    if (index.isValid())
    {
        _jsonMenuIndex = &index;
        _jsonMenu->exec(_ui.tvJson->mapToGlobal(point));
    }
}

void SimpleJsonViewer::onExpandJsonItems()
{
    if (_jsonMenuIndex)
    {
        qApp->setOverrideCursor(Qt::WaitCursor);
        expandJsonItem(*_jsonMenuIndex);
        qApp->restoreOverrideCursor();
    }
}

void SimpleJsonViewer::onCollapseJsonItems()
{
    if (_jsonMenuIndex)
    {
        qApp->setOverrideCursor(Qt::WaitCursor);
        collapseJsonItem(*_jsonMenuIndex);
        qApp->restoreOverrideCursor();
    }
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
        if (Preferences::getInstance().showRawJson())
        {
            QString rawData = _jsonItemModel->rawData();
            _ui.teJson->blockSignals(true);
            _ui.teJson->setPlainText(rawData);
            _ui.teJson->blockSignals(false);
        }
        return true;
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to load json data: " + error);
        return false;
    }
}

void SimpleJsonViewer::expandJsonItem(QModelIndex& index)
{
    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; ++i)
        expandJsonItem(index.child(i, 0));
    _ui.tvJson->expand(index);
}

void SimpleJsonViewer::collapseJsonItem(QModelIndex& index)
{
    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; ++i)
        collapseJsonItem(index.child(i, 0));
    _ui.tvJson->collapse(index);
}
