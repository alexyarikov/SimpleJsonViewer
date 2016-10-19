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

    QString lastFileName() const;
    void setupJsonTreeView();

    void onOpenNew();
    void onOpenLast();
    void onOpenFromClipboard();
    void onJsonContextMenu(const QPoint& point);
    void onExpandJsonItems();
    void onCollapseJsonItems();

    bool open(const QString& fileName);
    bool open(const QByteArray& jsonData);
    void expandJsonItem(QModelIndex& index);
    void collapseJsonItem(QModelIndex& index);
};
