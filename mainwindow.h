#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPixmap>
#include <QLabel>
#include <QMenuBar>
#include <vector>
#include <memory>

#include "Stock/stockmanager.h"

struct items_list
{
    QStandardItem*  location;
    QStandardItem*  model_num;
    QStandardItem*  trademark;
    QStandardItem*  part_num;
    QStandardItem*  voltage;
    QStandardItem*  amperage;
    QStandardItem*  quantity;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void buildModel();
    void buildModelfromItems(std::vector<powersupply>& items);

    void addPowerSupply();
    void findPowerSupply();
    void viewPowerSupply(powersupply& item);

signals:
    void dataChanged();

public slots:


private:
    std::unique_ptr<StockManager> m_stock_manager;
    QStandardItemModel* m_model;
    QTableView* m_table_view;
};

#endif // MAINWINDOW_H
