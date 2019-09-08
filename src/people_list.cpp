#include "people_list.h"

PeopleList::PeopleList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table_widget (new QTableWidget)
    , combo_groups (new QComboBox)
    , line_name_filter (new QLineEdit)
    , line_mail_filter (new QLineEdit)
{
    this->db = db;
    
    this->layout = new QVBoxLayout;
    setLayout(this->layout);
    
    this->line_name_filter->setClearButtonEnabled(true);
    this->line_name_filter->setPlaceholderText("type a name here to search");
    connect(this->line_name_filter, &QLineEdit::textChanged, this, &PeopleList::onNameFilterChanged);
    
    this->line_mail_filter->setClearButtonEnabled(true);
    this->line_mail_filter->setPlaceholderText("type a mail address here to search");
    connect(this->line_mail_filter, &QLineEdit::textChanged, this, &PeopleList::onNameFilterChanged);
    
    // build together line_name_filter, line_mail_filter and combo_groups
    QHBoxLayout *hbox_filters = new QHBoxLayout();
    hbox_filters->setMargin(0);
    hbox_filters->addWidget(this->line_name_filter);
    hbox_filters->addWidget(this->line_mail_filter);
    hbox_filters->addWidget(this->combo_groups);
    QWidget *widget_filters = new QWidget();
    widget_filters->setLayout(hbox_filters);
    
    this->layout->addWidget(widget_filters);
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
    for (int i=0; i < groups.length(); ++i)
    {
        gr.append(groups.at(i)["name"].toString());
    }
    
    this->combo_groups->addItems(gr);
}

void PeopleList::showPeople()
{
    // if no filter set, just use the wildcard '%' for the sql query
    QString str_filter_group = this->combo_groups->currentText();
    if (str_filter_group == "[ALL]")
    {
        str_filter_group = "%";
    }
    QString str_filter_name = this->line_name_filter->text();
    if (str_filter_name == "")
    {
        str_filter_name = "%";
    }
    QString str_filter_mail = this->line_mail_filter->text();
    if (str_filter_mail == "")
    {
        str_filter_mail = "%";
    }
    QList<QMap<QString,QVariant>> people = this->db->selectAllPersonsFiltered(str_filter_group,
                                                                              "%"+str_filter_name+"%",
                                                                              "%"+str_filter_mail+"%");
    
    this->table_widget->setRowCount(people.length());
    if (people.length() > 0)
    {
        this->table_widget->setColumnCount(people.at(0).keys().length()+1);
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
        
        qlonglong rowid = person["rowid"].toLongLong();
        QString name = person["name"].toString();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        button_delete->setMaximumWidth(40);
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onDeletePersonButtonClicked(rowid, name); });
        
        QPushButton *button_edit = new QPushButton("edit");
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onEditPersonButtonClicked(rowid, name); });
        
        QPushButton *button_donations = new QPushButton("donations");
        button_donations->setMaximumWidth(90);
        connect(button_donations, &QPushButton::clicked, this, []{  });
        
        this->table_widget->setCellWidget(i, 0, button_delete);
        this->table_widget->setCellWidget(i, 1, button_edit);
        this->table_widget->setCellWidget(i, 2, button_donations);
        this->table_widget->setItem(i, 3, new QTableWidgetItem(person["name"].toString()));
        this->table_widget->setItem(i, 4, new QTableWidgetItem(person["group"].toString()));
        this->table_widget->setItem(i, 5, new QTableWidgetItem(person["email"].toString()));
        this->table_widget->setItem(i, 6, mail);
        this->table_widget->setItem(i, 7, prayer);
        this->table_widget->setItem(i, 8, new QTableWidgetItem(person["agreement"].toString()));
    }
    
    QStringList labels;
    labels << "" << "" << "" << "name" << "group" << "email" << "m" << "p" << "agreement";
    this->table_widget->setHorizontalHeaderLabels(labels);
    this->table_widget->resizeColumnsToContents();
    this->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    this->table_widget->verticalHeader()->setVisible(false);
}

void PeopleList::clear()
{
    this->combo_groups->clear();
    this->table_widget->clear();
}

void PeopleList::onNameFilterChanged()
{
    this->table_widget->clear();
    showPeople();
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
    emit addNewPersonSignal();
}

void PeopleList::onEditPersonButtonClicked(qlonglong rowid, QString name)
{
    emit editPersonSignal(rowid, name);
}

void PeopleList::onShowDonationsButtonClicked(qlonglong rowid)
{
    emit showDonationsForPersonSignal(rowid);
}

void PeopleList::onDeletePersonButtonClicked(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Delete "+name, "Really delete "+name+"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deletePerson(rowid);
        
        this->table_widget->clear();
        showPeople();
    }
}
