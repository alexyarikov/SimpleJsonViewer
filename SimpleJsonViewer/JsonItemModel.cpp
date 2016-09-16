#include "stdafx.h"
#include "JsonItemModel.h"
#include "TreeItem.h"

JsonItemModel::JsonItemModel(QObject *parent) : QAbstractItemModel(parent)
{
}

JsonItemModel::~JsonItemModel()
{
}

bool JsonItemModel::loadData(const QByteArray &jsonData, QString &error)
{
    QJsonParseError jsonError;
    _jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        loadJsonDocument();
        return true;
    }
    else
    {
        error = jsonError.errorString();
        return false;
    }
}

QModelIndex JsonItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem = getParentItem(parent);
    if (!parentItem)
        return QModelIndex();

    QSharedPointer<TreeItem> childItem = parentItem->child(row);
    if (!childItem.isNull())
        return createIndex(row, column, childItem.data());
    else
        return QModelIndex();
}

QModelIndex JsonItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parent().data();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

bool JsonItemModel::hasChildren(const QModelIndex &parent/* = QModelIndex()*/) const
{
    TreeItem *parentItem = getParentItem(parent);
    if (parentItem)
        return parentItem->childCount() > 0;
    else
        return false;
}

int JsonItemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getParentItem(parent);
    if (parentItem)
        return parentItem->childCount();
    else
        return 0;
}

QVariant JsonItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data();
}

// get parent item
TreeItem* JsonItemModel::getParentItem(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return _rootItem.data();
    else
        return static_cast<TreeItem*>(parent.internalPointer());
}

// load json data into model
void JsonItemModel::loadJsonDocument()
{
    beginResetModel();

    _rootItem = QSharedPointer<TreeItem>(new TreeItem(""));

    if (_jsonDoc.isArray())
        loadJsonArray(_jsonDoc.array(), _rootItem);
    else if (_jsonDoc.isObject())
        loadJsonObject(_jsonDoc.object(), _rootItem);

    endResetModel();
}

void JsonItemModel::loadJsonArray(const QJsonArray &jsonArray, QSharedPointer<TreeItem> parentItem)
{
    for (int i = 0; i < jsonArray.size(); ++i)
    {
        const QJsonValue jsonValue = jsonArray.at(i);

        auto item = QSharedPointer<TreeItem>(new TreeItem(QString("[%1]").arg(i), parentItem));
        parentItem->appendChild(item);

        if (jsonValue.isArray())
            loadJsonArray(jsonValue.toArray(), item);
        else if (jsonValue.isObject())
            loadJsonObject(jsonValue.toObject(), item);
        else
            loadJsonValue(jsonValue, item);
    }
}

void JsonItemModel::loadJsonObject(const QJsonObject &jsonObject, QSharedPointer<TreeItem> parentItem)
{
    for (const QString& key : jsonObject.keys())
    {
        auto item = QSharedPointer<TreeItem>(new TreeItem(key, parentItem));
        parentItem->appendChild(item);

        QJsonValue jsonValue = jsonObject.value(key);
        if (jsonValue.isArray())
            loadJsonArray(jsonValue.toArray(), item);
        else if (jsonValue.isObject())
            loadJsonObject(jsonValue.toObject(), item);
        else
            loadJsonValue(jsonValue, item);
    }
}

void JsonItemModel::loadJsonValue(const QJsonValue &jsonValue, QSharedPointer<TreeItem> parentItem)
{
    auto item = QSharedPointer<TreeItem>(new TreeItem(jsonValue.toVariant(), parentItem));
    parentItem->appendChild(item);
}
