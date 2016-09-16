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
    QPointer<JsonItemModel> _jsonItemModel;
    QPointer<QMenu> _jsonMenu;
    QModelIndex* _jsonMenuIndex = Q_NULLPTR;
    QString _lastFileName;

    void setupJsonTreeView();
    void openNew();
    void openLast();
    void openFromClipboard();
    bool open(const QString& fileName);
    bool open(const QByteArray& jsonData);
    void loadLastFileName();
    void saveLastFileName();
    void jsonContextMenu(const QPoint& point);
    void expandJsonItems();
    void expandJsonItem(QModelIndex& index);
    void collapseJsonItems();
    void collapseJsonItem(QModelIndex& index);
};
