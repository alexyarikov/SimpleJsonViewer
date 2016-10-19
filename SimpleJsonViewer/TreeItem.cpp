#include "stdafx.h"
#include "TreeItem.h"

TreeItem::TreeItem(const QVariant& data, std::shared_ptr<TreeItem> parent) :
    _data(data),
    _parent(parent)
{
}

TreeItem::~TreeItem()
{
}

int TreeItem::row()
{
    auto parent = _parent.lock();
    if (!parent)
    {
        for (size_t i = 0; i < parent->_children.size(); ++i)
        {
            if (parent->_children[i].get() == this)
                return i;
        }
        return -1;
    }
    else
        return 0;
}
