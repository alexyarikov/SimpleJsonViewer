#pragma once

class TreeItem
{
public:
    explicit TreeItem(const QVariant& data, std::shared_ptr<TreeItem> parentItem = std::shared_ptr<TreeItem>());
    ~TreeItem();

    inline void appendChild(std::shared_ptr<TreeItem> item) { _children.push_back(item); };
    inline std::shared_ptr<TreeItem> child(const size_t row) { return row < _children.size() ? _children.at(row) : std::shared_ptr<TreeItem>(); };
    inline std::weak_ptr<TreeItem> parent() { return _parent; };
    inline size_t childCount() const { return _children.size(); };
    inline QVariant data() const { return _data; };
    int row();

private:
    std::deque<std::shared_ptr<TreeItem>> _children;
    std::weak_ptr<TreeItem> _parent;
    QVariant _data;
};
