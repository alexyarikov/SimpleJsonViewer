#include "stdafx.h"
#include "TreeItem.h"

TreeItem::TreeItem(const QVariant &data, QSharedPointer<TreeItem> parent) :
    _parent(parent),
    _data(data)
{
}

TreeItem::~TreeItem()
{
}

int TreeItem::row()
{
    QSharedPointer<TreeItem> parent = _parent.toStrongRef();
    if (!parent.isNull())
    {
        for (int i = 0; i < parent->_children.size(); ++i)
        {
            if (parent->_children[i].data() == this)
                return i;
        }
        return -1;
    }
    else
        return 0;
}
