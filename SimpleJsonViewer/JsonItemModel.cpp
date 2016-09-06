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
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        loadJsonDocument(jsonDoc);
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

    TreeItem* parentItem;
    if (!parent.isValid())
        parentItem = _rootItem.data();
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

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

int JsonItemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = _rootItem.data();
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

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

// load json data into model
void JsonItemModel::loadJsonDocument(QJsonDocument& jsonDoc)
{
    beginResetModel();

    _rootItem = QSharedPointer<TreeItem>(new TreeItem(""));

    if (jsonDoc.isArray())
        loadJsonArray(jsonDoc.array(), _rootItem);
    else if (jsonDoc.isObject())
        loadJsonObject(jsonDoc.object(), _rootItem);

    endResetModel();
}

void JsonItemModel::loadJsonArray(QJsonArray &jsonArray, QSharedPointer<TreeItem> parentItem)
{
    for (QJsonValue jsonValue : jsonArray)
    {
        if (jsonValue.isArray())
            loadJsonArray(jsonValue.toArray(), parentItem);
        else if (jsonValue.isObject())
            loadJsonObject(jsonValue.toObject(), parentItem);
        else
            loadJsonValue(jsonValue, parentItem);
    }
}

void JsonItemModel::loadJsonObject(QJsonObject &jsonObject, QSharedPointer<TreeItem> parentItem)
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

void JsonItemModel::loadJsonValue(QJsonValue &jsonValue, QSharedPointer<TreeItem> parentItem)
{
    auto item = QSharedPointer<TreeItem>(new TreeItem(jsonValue.toVariant(), parentItem));
    parentItem->appendChild(item);
}
