#include "stockmanager.h"

#include <QDebug>

StockManager::StockManager()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    if(!db.open())
    {
        qDebug() << "Failed to open connexion with database!";
    }

    QString cmd = "CREATE TABLE IF NOT EXISTS powersupply("
                  "model varchar primary key, "
                  "location int, "
                  "trademark varchar, "
                  "part_num varchar, "
                  "voltage varchar, "
                  "amperage varchar, "
                  "quantity int, "
                  "image blob);";
    QSqlQuery query;
    if(!query.exec(cmd))
    {
        qDebug() << "Failed to execute query: " + query.lastError().text();
    }

    db.close();
}

QString StockManager::loadStock(std::vector<powersupply>& list)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open())
    {
        return "Failed to open database!";
    }

    QSqlQuery query("SELECT * FROM powersupply");
    if(!query.exec())
    {
        return query.lastError().text();
    }

    while(query.next())
    {
        powersupply item;
        item.model = query.value(0).toString();
        item.location = query.value(1).toInt();
        item.trademark = query.value(2).toString();
        item.part_num = query.value(3).toString();
        item.voltage = query.value(4).toString();
        item.amperage = query.value(5).toString();
        item.quantity = query.value(6).toInt();
        item.image = QImage::fromData(query.value(7).toByteArray(), "PNG");

        list.push_back(item);
    }

    db.close();
    return "SUCCESS";
}

void StockManager::addItem(powersupply item)
{
    std::vector<powersupply> item_list;
    loadStock(item_list);

    int exist = -1;
    for(size_t i = 0; i < item_list.size(); i++)
    {
        if(QString::compare(item_list[i].model, item.model, Qt::CaseInsensitive) == 0)
        {
            qWarning("already exist");
            exist = static_cast<int>(i);
            break;
        }
    }

    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open())
    {
        qDebug() << "Failed to open database!";
    }


    QSqlQuery query;
    if(exist != -1)
    {
        query.prepare("UPDATE powersupply SET quantity = :quantity WHERE model=:model");

        query.bindValue(":quantity", item_list[static_cast<size_t>(exist)].quantity + 1);
        query.bindValue(":model", item_list[static_cast<size_t>(exist)].model);
    }
    else
    {
        query.prepare("INSERT INTO powersupply(model, location, trademark, part_num, voltage, amperage, quantity, image) "
                      "VALUES(:model, :location, :trademark, :part_num, :voltage, :amperage, :quantity, :image)");

        query.bindValue(":model", item.model);
        query.bindValue(":location", item.location);
        query.bindValue(":trademark", item.trademark);
        query.bindValue(":part_num", item.part_num);
        query.bindValue(":voltage", item.voltage);
        query.bindValue(":amperage", item.amperage);
        query.bindValue(":quantity", item.quantity);

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        item.image.save(&buffer, "PNG");
        query.bindValue(":image", ba);

        qDebug() << item.model;
    }

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }

    db.close();
}

void StockManager::removeItem(QString model)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open())
    {
        qDebug() << "Failed to open database!";
    }

    QSqlQuery query;
    query.prepare("DELETE FROM powersupply WHERE model=:model");
    query.bindValue(":model", model);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }

    db.close();
}

void StockManager::updateItem(powersupply item, QString old_model)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open())
    {
        qDebug() << "Failed to open database!";
    }

    QSqlQuery query;
    query.prepare("UPDATE powersupply SET model=:model, location=:location, trademark=:trademark, voltage=:voltage, amperage=:amperage, part_num=:part_num "
                  "WHERE model=:old_model");
    query.bindValue(":model", item.model);
    query.bindValue(":location", item.location);
    query.bindValue(":trademark", item.trademark);
    query.bindValue(":voltage", item.voltage);
    query.bindValue(":amperage", item.amperage);
    query.bindValue(":part_num", item.part_num);
    query.bindValue(":old_model", old_model);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }

    db.close();
}

void StockManager::findItem(const QString& arg, const datatype& type, std::vector<powersupply>& items_list)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open())
    {
        qDebug() << "Failed to open database!";
    }

    QSqlQuery query;
    if(type == datatype::MODEL)
    {
        query.prepare("SELECT * FROM powersupply WHERE UPPER(model) LIKE UPPER(:arg)");
    }
    else if(type == datatype::PART_NUM)
    {
        query.prepare("SELECT * FROM powersupply WHERE UPPER(part_num) LIKE UPPER(:arg)");
    }
    else if(type == datatype::TRADEMARK)
    {
        query.prepare("SELECT * FROM powersupply WHERE UPPER(trademark) LIKE UPPER(:arg)");
    }
    else
    {
        db.close();
        qDebug() << "Unable to fetch : unknown data type";
        return;
    }

    query.bindValue(":arg", '%'+arg+'%');

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }

    items_list.clear();
    while(query.next())
    {
        powersupply item;
        item.model = query.value(0).toString();
        item.location = query.value(1).toInt();
        item.trademark = query.value(2).toString();
        item.part_num = query.value(3).toString();
        item.voltage = query.value(4).toString();
        item.amperage = query.value(5).toString();
        item.quantity = query.value(6).toInt();
        item.image = QImage::fromData(query.value(7).toByteArray(), "PNG");

        items_list.push_back(item);
    }

    db.close();
}

void StockManager::loadItem(const QString& model, powersupply& item)
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.open())
    {
        qDebug() << "Failed to open database!";
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM powersupply WHERE model=:model");
    query.bindValue(":model", model);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }

    while(query.next())
    {
        item.model = query.value(0).toString();
        item.location = query.value(1).toInt();
        item.trademark = query.value(2).toString();
        item.part_num = query.value(3).toString();
        item.voltage = query.value(4).toString();
        item.amperage = query.value(5).toString();
        item.quantity = query.value(6).toInt();
        item.image = QImage::fromData(query.value(7).toByteArray(), "PNG");
    }

    db.close();
}
