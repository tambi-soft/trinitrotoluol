#include "people_list.h"

PeopleList::PeopleList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table_widget (new QTableWidget)
    , combo_groups (new QComboBox)
    , line_edit_filter (new QLineEdit)
{
    this->db = db;
    
    this->layout = new QVBoxLayout;
    setLayout(this->layout);
    this->layout->addWidget(this->combo_groups);
    this->layout->addWidget(this->table_widget);
    
    QPushButton *button_add_new_person = new QPushButton("add new person");
    layout->addWidget(button_add_new_person);
    
    connect(combo_groups, &QComboBox::currentTextChanged, this, &PeopleList::onGroupsFilterChanged);
    connect(button_add_new_person, &QPushButton::clicked, this, &PeopleList::onNewPersonButtonClicked);
    
    showGroupsFilterCombo();
    showPeople();
    
    //this->db->insertNewPerson("name", "group", "email", "add", "phone");
}

void PeopleList::showGroupsFilterCombo()
{
    QList<QMap<QString,QVariant>> groups = this->db->selectGroups();
    QList<QString> gr;
    gr.append("[ALL]");
    for (int i=1; i < groups.length(); ++i)
    {
        gr.append(groups.at(i)["group"].toString());
    }
    
    this->combo_groups->addItems(gr);
}

void PeopleList::showPeople()
{
    QList<QMap<QString,QVariant>> people;
    if (this->combo_groups->currentText() != "[ALL]")
    {
        people = this->db->selectAllPersonsFiltered(this->combo_groups->currentText());
    }
    else
    {
        people = this->db->selectAllPersons();
    }
    
    this->table_widget->setRowCount(people.length());
    if (people.length() > 0)
    {
        this->table_widget->setColumnCount(people.at(0).keys().length());
    }
    
    for (int i=0; i < people.length(); ++i)
    {
        QMap<QString,QVariant> person = people.at(i);
        
        QTableWidgetItem *mail = new QTableWidgetItem();
        QTableWidgetItem *prayer = new QTableWidgetItem();
        if (person["agreed_mail"] == 1)
        {
            mail->setBackgroundColor(Qt::black);
        }
        
        if (person["agreed_prayer"] == 1)
        {
            prayer->setBackgroundColor(Qt::black);
        }
        
        QPushButton *button_edit = new QPushButton("edit");
        button_edit->setMaximumWidth(40);
        qlonglong rowid = person["rowid"].toLongLong();
        QString name = person["name"].toString();
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onEditPersonButtonClicked(rowid, name); });
        
        this->table_widget->setCellWidget(i, 0, button_edit);
        this->table_widget->setItem(i, 1, new QTableWidgetItem(person["name"].toString()));
        this->table_widget->setItem(i, 2, new QTableWidgetItem(person["group"].toString()));
        this->table_widget->setItem(i, 3, new QTableWidgetItem(person["email"].toString()));
        this->table_widget->setItem(i, 4, mail);
        this->table_widget->setItem(i, 5, prayer);
        this->table_widget->setItem(i, 6, new QTableWidgetItem(person["agreement"].toString()));
    }
    
    QStringList labels;
    labels << "" << "name" << "group" << "email" << "m" << "p" << "agreement";
    this->table_widget->setHorizontalHeaderLabels(labels);
    this->table_widget->resizeColumnsToContents();
    this->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void PeopleList::clear()
{
    this->combo_groups->clear();
    this->table_widget->clear();
}

void PeopleList::onGroupsFilterChanged()
{
    /*
    this->combo_groups->blockSignals(true);
    clear();
    showGroupsFilterCombo();
    this->combo_groups->blockSignals(false);
    */
    
    this->table_widget->clear();
    showPeople();
}

void PeopleList::onNewPersonButtonClicked()
{
    
}

void PeopleList::onEditPersonButtonClicked(qlonglong rowid, QString name)
{
    emit editPersonSignal(rowid, name);
}
