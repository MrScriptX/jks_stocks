#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   m_stock_manager = std::make_unique<StockManager>();

   QAction* new_item = menuBar()->addAction(tr("Nouveau"));
   connect(new_item, &QAction::triggered, this, &MainWindow::addPowerSupply);
   QAction* find_item = menuBar()->addAction(tr("Chercher"));
   connect(find_item, &QAction::triggered, this, &MainWindow::findPowerSupply);
   QAction* refresh = menuBar()->addAction(tr("Rafraichir"));
   connect(refresh, &QAction::triggered, this, [this]{this->buildModel();});


   m_table_view = new QTableView;
   m_model = new QStandardItemModel;
   m_model->setHorizontalHeaderLabels(QStringList({tr("Location"), tr("Modele"), tr("Marque"), tr("Part num"), tr("Voltage"), tr("Amperage"), tr("Quantité")}));

   buildModel();

   m_table_view->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(m_table_view, &QTableView::customContextMenuRequested, [this](const QPoint& pos){
      QModelIndex index = m_table_view->indexAt(pos);

      QAction* del = new QAction(tr("supprimer"), this);
      connect(del, &QAction::triggered, [this, index]{
          m_stock_manager->removeItem(m_model->itemFromIndex(index)->text());
          m_model->removeRow(index.row());
      });

      QAction* view = new QAction(tr("voir"), this);
      connect(view, &QAction::triggered, [this, index]{
          powersupply item;
          m_stock_manager->loadItem(m_model->itemFromIndex(index)->text(), item);
          this->viewPowerSupply(item);
      });

      QMenu* menu = new QMenu(this);
      menu->addAction(view);
      menu->addAction(del);
      menu->popup(m_table_view->viewport()->mapToGlobal(pos));
   });


   connect(this, &MainWindow::dataChanged, this, &MainWindow::buildModel);

   setCentralWidget(m_table_view);
   setFixedSize(1280, 720);
}

void MainWindow::buildModel()
{
    m_model->clear();
    delete m_model;
    m_model = new QStandardItemModel;


    std::vector<powersupply> item_list;
    if(m_stock_manager->loadStock(item_list) != "SUCCESS")
    {
        qDebug() << "Failed to load data";
    }


    std::vector<items_list> data;
    for(size_t i = 0; i < item_list.size(); i++)
    {
        data.push_back(items_list());

        data[i].location = new QStandardItem;
        data[i].location->setText(QString::number(item_list[i].location));
        m_model->setItem(static_cast<int>(i), 0, data[i].location);

        data[i].model_num = new QStandardItem;
        data[i].model_num->setText(item_list[i].model);
        m_model->setItem(static_cast<int>(i), 1, data[i].model_num);

        data[i].trademark = new QStandardItem;
        data[i].trademark->setText(item_list[i].trademark);
        m_model->setItem(static_cast<int>(i), 2, data[i].trademark);

        data[i].part_num = new QStandardItem;
        data[i].part_num->setText(item_list[i].part_num);
        m_model->setItem(static_cast<int>(i), 3, data[i].part_num);

        data[i].voltage = new QStandardItem;
        data[i].voltage->setText(item_list[i].voltage);
        m_model->setItem(static_cast<int>(i), 4, data[i].voltage);

        data[i].amperage = new QStandardItem;
        data[i].amperage->setText(item_list[i].amperage);
        m_model->setItem(static_cast<int>(i), 5, data[i].amperage);

        data[i].quantity = new QStandardItem;
        data[i].quantity->setText(QString::number(item_list[i].quantity));
        m_model->setItem(static_cast<int>(i), 6, data[i].quantity);
    }

    connect(m_model, &QStandardItemModel::itemChanged, [this, item_list, data](QStandardItem* item) mutable {
        QModelIndex index = item->index();

        powersupply old_item = item_list[static_cast<size_t>(index.row())];

        powersupply new_item;
        new_item.model = data[static_cast<size_t>(index.row())].model_num->text();
        new_item.location = data[static_cast<size_t>(index.row())].location->text().toInt();
        new_item.trademark = data[static_cast<size_t>(index.row())].trademark->text();
        new_item.part_num = data[static_cast<size_t>(index.row())].part_num->text();
        new_item.voltage = data[static_cast<size_t>(index.row())].voltage->text();
        new_item.amperage = data[static_cast<size_t>(index.row())].amperage->text();

        m_stock_manager->updateItem(new_item, old_item.model);
    });

    m_table_view->setModel(m_model);
    m_table_view->resizeRowsToContents();
    m_table_view->setColumnWidth(1, 350);
    m_table_view->setColumnWidth(2, 200);
    m_table_view->setColumnWidth(3, 250);
    m_table_view->setColumnWidth(6, 50);
    m_table_view->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::buildModelfromItems(std::vector<powersupply>& items)
{
    m_model->clear();
    delete m_model;
    m_model = new QStandardItemModel;


    std::vector<items_list> data;
    for(size_t i = 0; i < items.size(); i++)
    {
        data.push_back(items_list());

        data[i].location = new QStandardItem;
        data[i].location->setText(QString::number(items[i].location));
        m_model->setItem(static_cast<int>(i), 0, data[i].location);

        data[i].model_num = new QStandardItem;
        data[i].model_num->setText(items[i].model);
        m_model->setItem(static_cast<int>(i), 1, data[i].model_num);

        data[i].trademark = new QStandardItem;
        data[i].trademark->setText(items[i].trademark);
        m_model->setItem(static_cast<int>(i), 2, data[i].trademark);

        data[i].part_num = new QStandardItem;
        data[i].part_num->setText(items[i].part_num);
        m_model->setItem(static_cast<int>(i), 3, data[i].part_num);

        data[i].voltage = new QStandardItem;
        data[i].voltage->setText(items[i].voltage);
        m_model->setItem(static_cast<int>(i), 4, data[i].voltage);

        data[i].amperage = new QStandardItem;
        data[i].amperage->setText(items[i].amperage);
        m_model->setItem(static_cast<int>(i), 5, data[i].amperage);

        data[i].quantity = new QStandardItem;
        data[i].quantity->setText(QString::number(items[i].quantity));
        m_model->setItem(static_cast<int>(i), 6, data[i].quantity);
    }

    connect(m_model, &QStandardItemModel::itemChanged, [this, items, data](QStandardItem* item) mutable {
        QModelIndex index = item->index();

        powersupply old_item = items[static_cast<size_t>(index.row())];

        powersupply new_item;
        new_item.model = data[static_cast<size_t>(index.row())].model_num->text();
        new_item.location = data[static_cast<size_t>(index.row())].location->text().toInt();
        new_item.trademark = data[static_cast<size_t>(index.row())].trademark->text();
        new_item.part_num = data[static_cast<size_t>(index.row())].part_num->text();
        new_item.voltage = data[static_cast<size_t>(index.row())].voltage->text();
        new_item.amperage = data[static_cast<size_t>(index.row())].amperage->text();

        m_stock_manager->updateItem(new_item, old_item.model);
    });

    m_table_view->setModel(m_model);
    m_table_view->resizeRowsToContents();
    m_table_view->setColumnWidth(1, 350);
    m_table_view->setColumnWidth(2, 200);
    m_table_view->setColumnWidth(3, 250);
    m_table_view->setColumnWidth(6, 50);
    m_table_view->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addPowerSupply()
{
    QWidget* window = new QWidget;
    QLineEdit* model = new QLineEdit;
    model->setPlaceholderText(tr("Model"));
    QLineEdit* part_num = new QLineEdit;
    part_num->setPlaceholderText(tr("Part num"));
    QLineEdit* trademark = new QLineEdit;
    trademark->setPlaceholderText(tr("Marque"));
    QLineEdit* voltage = new QLineEdit;
    voltage->setPlaceholderText(tr("Volt"));
    QLineEdit* amperage = new QLineEdit;
    amperage->setPlaceholderText(tr("Amperage"));
    QLineEdit* location = new QLineEdit;
    location->setPlaceholderText(tr("Location"));

    powersupply* item = new powersupply;
    item->image = QImage("path");

    QPushButton* add_image = new QPushButton(tr("Ajouter image"));
    connect(add_image, &QPushButton::clicked, [this, item]{
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        QString path = dialog.getOpenFileName();

        item->image = QImage(path);
    });

    QPushButton* validate = new QPushButton(tr("Valider"));
    connect(validate, &QPushButton::clicked, [this, item, window, model, part_num, trademark, voltage, amperage, location]() mutable {
        item->model = model->text();
        item->part_num = part_num->text();
        item->trademark = trademark->text();
        item->voltage = voltage->text();
        item->amperage = amperage->text();
        item->location = location->text().toInt();

        m_stock_manager->addItem(*item);
        delete item;

        emit this->dataChanged();
        window->close();
    });

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(model);
    layout->addWidget(part_num);
    layout->addWidget(trademark);
    layout->addWidget(voltage);
    layout->addWidget(amperage);
    layout->addWidget(location);
    layout->addWidget(add_image);
    layout->addWidget(validate);

    window->setLayout(layout);
    window->show();
}

void MainWindow::findPowerSupply()
{
    QLineEdit* arg_bar = new QLineEdit;
    QPushButton* model = new QPushButton(tr("Par modele"));
    connect(model, &QPushButton::clicked, [this, arg_bar]{
        std::vector<powersupply> items;
        m_stock_manager->findItem(arg_bar->text(), datatype::MODEL, items);
        this->buildModelfromItems(items);
    });
    QPushButton* part = new QPushButton(tr("Par numéro"));
    connect(part, &QPushButton::clicked, [this, arg_bar]{
        std::vector<powersupply> items;
        m_stock_manager->findItem(arg_bar->text(), datatype::PART_NUM, items);
        this->buildModelfromItems(items);
    });
    QPushButton* trademark = new QPushButton(tr("Par marque"));
    connect(trademark, &QPushButton::clicked, [this, arg_bar]{
        std::vector<powersupply> items;
        m_stock_manager->findItem(arg_bar->text(), datatype::TRADEMARK, items);
        this->buildModelfromItems(items);
    });

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(model);
    buttons->addWidget(part);
    buttons->addWidget(trademark);

    QVBoxLayout* global = new QVBoxLayout;
    global->addWidget(arg_bar);
    global->addLayout(buttons);

    QWidget* window = new QWidget();
    window->setLayout(global);
    window->show();
}

void MainWindow::viewPowerSupply(powersupply& item)
{
    QLabel* label = new QLabel;
    label->setMinimumSize(300, 300);
    label->setPixmap(QPixmap::fromImage(item.image));
    label->resize(label->pixmap()->size());
    label->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(label, &QLabel::customContextMenuRequested, [this, label, item](const QPoint& pos){
        QAction* modify = new QAction(tr("modifier"));
        connect(modify, &QAction::triggered, [this, item, label]() mutable {
            QFileDialog dialog(this);
            dialog.setFileMode(QFileDialog::ExistingFile);
            QString path = dialog.getOpenFileName();

            if(path != "")
            {
                qDebug() << path;
                item.image = QImage(path);
                m_stock_manager->updateItem(item, item.model);

                label->setPixmap(QPixmap::fromImage(item.image));
                label->resize(label->pixmap()->size());
            }
        });

        QMenu* menu = new QMenu(this);
        menu->addAction(modify);
        menu->popup(label->mapToGlobal(pos));
    });

    label->show();
}
