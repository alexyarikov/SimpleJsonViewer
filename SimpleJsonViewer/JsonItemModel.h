#pragma once

class TreeItem;

class JsonItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit JsonItemModel(QObject *parent = Q_NULLPTR);
    ~JsonItemModel();

    // load json data into model
    bool loadData(const QByteArray &jsonData, QString &error);

    // overridden model methods
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE { return 1; };
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QByteArray rawData(const QJsonDocument::JsonFormat format) const { return _jsonDoc.toJson(format); }

private:
    QJsonDocument _jsonDoc;
    QSharedPointer<TreeItem> _rootItem;

    // get parent item
    TreeItem* getParentItem(const QModelIndex &parent) const;

    // load json data into model
    void loadJsonDocument();
    void loadJsonArray(const QJsonArray &jsonArray, QSharedPointer<TreeItem> parentItem);
    void loadJsonObject(const QJsonObject &jsonObject, QSharedPointer<TreeItem> parentItem);
    void loadJsonValue(const QJsonValue &jsonValue, QSharedPointer<TreeItem> parentItem);
};
