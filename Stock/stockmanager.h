#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QString>
#include <QImage>
#include <QLineEdit>
#include <QBuffer>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QPushButton>
#include <QFileDialog>

struct powersupply
{
    int location;
    QString trademark;
    QString model;
    QString part_num;
    QString voltage;
    QString amperage;
    int quantity = 1;
    QImage image;
};

enum datatype
{
    MODEL,
    PART_NUM,
    TRADEMARK
};

class StockManager
{
public:
    StockManager();

    QString loadStock(std::vector<powersupply>& list);
    void addItem(powersupply item);
    void removeItem(QString model);
    void updateItem(powersupply item, QString old_model);
    void loadItem(const QString& model, powersupply& item);

    void findItem(const QString& arg, const datatype& type, std::vector<powersupply>& items_list);
private:

};

#endif // STOCKMANAGER_H
