#include "stdafx.h"
#include "JsonItemModel.h"
#include "TreeItem.h"
#include "Preferences.h"

JsonItemModel::JsonItemModel(QObject* parent) : QAbstractItemModel(parent)
{
}

JsonItemModel::~JsonItemModel()
{
}

bool JsonItemModel::loadData(const QByteArray& jsonData, QString& error)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        _rawData.clear();
        error = jsonError.errorString();
        return false;
    }

    if (Preferences::getInstance().showRawJson())
        _rawData = jsonDoc.toJson(QJsonDocument::Indented);
    return loadJsonDocument(jsonDoc);
}

QModelIndex JsonItemModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parentItem = getParentItem(parent);
    if (!parentItem)
        return QModelIndex();

    std::shared_ptr<TreeItem> childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem.get());
    else
        return QModelIndex();
}

QModelIndex JsonItemModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parent().lock().get();

    if (parentItem == _rootItem.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

bool JsonItemModel::hasChildren(const QModelIndex& parent/* = QModelIndex()*/) const
{
    TreeItem* parentItem = getParentItem(parent);
    if (parentItem)
        return parentItem->childCount() > 0;
    else
        return false;
}

int JsonItemModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem = getParentItem(parent);
    if (parentItem)
        return parentItem->childCount();
    else
        return 0;
}

QVariant JsonItemModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    return item->data();
}

// get parent item
TreeItem* JsonItemModel::getParentItem(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return _rootItem.get();
    else
        return static_cast<TreeItem*>(parent.internalPointer());
}

// load json data into model
bool JsonItemModel::loadJsonDocument(const QJsonDocument& jsonDoc)
{
    bool res = false;

    beginResetModel();

    _rootItem = make_shared_nothrow<TreeItem>("");
    if (!_rootItem)
        return false;

    if (jsonDoc.isArray())
        res = loadJsonArray(jsonDoc.array(), _rootItem);
    else if (jsonDoc.isObject())
        res = loadJsonObject(jsonDoc.object(), _rootItem);

    endResetModel();

    return res;
}

bool JsonItemModel::loadJsonArray(const QJsonArray& jsonArray, std::shared_ptr<TreeItem> parentItem)
{
    bool res = true;

    for (int i = 0; res && i < jsonArray.size(); ++i)
    {
        const QJsonValue jsonValue = jsonArray.at(i);

        auto item = make_shared_nothrow<TreeItem>(QString("[%1]").arg(i), parentItem);
        if (!item)
            return false;

        parentItem->appendChild(item);

        if (jsonValue.isArray())
            res = loadJsonArray(jsonValue.toArray(), item);
        else if (jsonValue.isObject())
            res = loadJsonObject(jsonValue.toObject(), item);
        else
            res = loadJsonValue(jsonValue, item);
    }

    return res;
}

bool JsonItemModel::loadJsonObject(const QJsonObject& jsonObject, std::shared_ptr<TreeItem> parentItem)
{
    bool res = true;

    auto keys = jsonObject.keys();
    for (int i = 0; res && i < keys.size(); ++i)
    {
        const QString& key = keys.at(i);

        auto item = make_shared_nothrow<TreeItem>(key, parentItem);
        if (!item)
            return false;

        parentItem->appendChild(item);

        QJsonValue jsonValue = jsonObject.value(key);
        if (jsonValue.isArray())
            res = loadJsonArray(jsonValue.toArray(), item);
        else if (jsonValue.isObject())
            res = loadJsonObject(jsonValue.toObject(), item);
        else
            res = loadJsonValue(jsonValue, item);
    }

    return res;
}

bool JsonItemModel::loadJsonValue(const QJsonValue& jsonValue, std::shared_ptr<TreeItem> parentItem)
{
    auto item = make_shared_nothrow<TreeItem>(jsonValue.toVariant(), parentItem);
    if (item)
    {
        parentItem->appendChild(item);
        return true;
    }
    else
        return false;
}
