#pragma once

class TreeItem
{
public:
    explicit TreeItem(const QVariant &data, QSharedPointer<TreeItem> parentItem = QSharedPointer<TreeItem>());
    ~TreeItem();

    inline void appendChild(QSharedPointer<TreeItem> item) { _children.append(item); };
    inline QSharedPointer<TreeItem> child(const int row) { return _children.value(row); };
    inline QSharedPointer<TreeItem> parent() { return _parent; };
    inline int childCount() const { return _children.count(); };
    inline QVariant data() const { return _data; };
    int row();

private:
    QList<QSharedPointer<TreeItem>> _children;
    QWeakPointer<TreeItem> _parent;
    QVariant _data;
};
