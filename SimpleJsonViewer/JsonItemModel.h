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
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE { return 1; };
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<TreeItem> _rootItem;

    // load json data into model
    void loadJsonDocument(QJsonDocument &jsonDoc);
    void loadJsonArray(QJsonArray &jsonArray, QSharedPointer<TreeItem> parentItem);
    void loadJsonObject(QJsonObject &jsonObject, QSharedPointer<TreeItem> parentItem);
    void loadJsonValue(QJsonValue &jsonValue, QSharedPointer<TreeItem> parentItem);
};
