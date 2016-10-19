#pragma once

class TreeItem;

class JsonItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit JsonItemModel(QObject* parent = Q_NULLPTR);
    ~JsonItemModel();

    // load json data into model
    bool loadData(const QByteArray& jsonData, QString& error);

    // overridden model methods
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE { return 1; };
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    QByteArray rawData() const { return _rawData; }

private:
    std::shared_ptr<TreeItem> _rootItem;
    QByteArray _rawData;

    // get parent item
    TreeItem* getParentItem(const QModelIndex &parent) const;

    // load json data into model
    bool loadJsonDocument(const QJsonDocument& jsonDoc);
    bool loadJsonArray(const QJsonArray &jsonArray, std::shared_ptr<TreeItem> parentItem);
    bool loadJsonObject(const QJsonObject &jsonObject, std::shared_ptr<TreeItem> parentItem);
    bool loadJsonValue(const QJsonValue &jsonValue, std::shared_ptr<TreeItem> parentItem);
};
