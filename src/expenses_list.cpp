#include "expenses_list.h"

ExpensesList::ExpensesList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    setLayout(this->layout);
    
    QPushButton *button_new = new QPushButton("add new expenses");
    connect(button_new, &QPushButton::clicked, this, &ExpensesList::addNew);
    
    this->layout->addWidget(this->table);
    this->layout->addWidget(button_new);
}

void ExpensesList::loadData()
{
    QStringList headers;
    headers << "" << "" << "name" << "amount" << "cost one" << "currency" << "sum" << "date";
    
    QList<QMap<QString,QVariant>> data = this->db->selectExpenses();
    this->table->setRowCount(data.length());
    this->table->setColumnCount(headers.length());
    this->table->setHorizontalHeaderLabels(headers);
    
    for (int i=0; i < data.length(); ++i)
    {
        this->table->setItem(i, 2, new QTableWidgetItem(data.at(i)["name"].toString()));
        this->table->setItem(i, 3, new QTableWidgetItem(data.at(i)["amount"].toString()));
        this->table->setItem(i, 4, new QTableWidgetItem(data.at(i)["cost_one"].toString()));
        this->table->setItem(i, 5, new QTableWidgetItem(data.at(i)["currency_code"].toString()));
        
        double sum = data.at(i)["cost_one"].toDouble() *
                data.at(i)["amount"].toDouble();
        this->table->setItem(i, 6, new QTableWidgetItem(
                                 QString::number(sum)
                                 ));
        
        this->table->setItem(i, 7, new QTableWidgetItem(data.at(i)["date"].toString()));
    }
    
    this->table->resizeColumnsToContents();
}

void ExpensesList::addNew()
{
    
}

void ExpensesList::updateView()
{
    this->table->clear();
    loadData();
}

void ExpensesList::showEvent(QShowEvent */** event **/)
{
    updateView();
}
