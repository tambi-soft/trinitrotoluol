#include "person_groups.h"

PersonGroups::PersonGroups(DbAdapter *db, qlonglong rowid_people, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    this->rowid_people = rowid_people;
    
    //QPushButton *button_add_group = new QPushButton("Add Person to Group");
    //this->layout->addWidget(button_add_group);
    
    loadGroupsComboData();
    connect(combo_group, qOverload<int>(&QComboBox::currentIndexChanged), this, &PersonGroups::addPersonToGroup);
    this->layout->addWidget(this->combo_group);
    
    showData();
}

void PersonGroups::showData()
{
    recreateView();
    
    QList<QMap<QString,QVariant>> data = this->db->groupMatrixSelect(this->rowid_people);
    
    for (int i=0; i < data.length(); ++i)
    {
        qlonglong rowid_groups = data.at(i)["rowid_groups"].toLongLong();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, rowid_groups]{ deletePersonFromGroup(rowid_groups); });
        
        this->grid->addWidget(button_delete, i, 0);
        this->grid->addWidget(new QLabel(data.at(i)["name"].toString()), i, 1);
    }
}

void PersonGroups::deletePersonFromGroup(qlonglong rowid_groups)
{
    this->db->groupMatrixDelete(this->rowid_people, rowid_groups);
    
    showData();
}

void PersonGroups::addPersonToGroup(qlonglong rowid_groups)
{
    QString group_str = this->combo_group->currentText();
    if (group_str != "")
    {
        int group = this->group_data_map[group_str];
        
        this->db->groupMatrixInsert(this->rowid_people, group);
        
        showData();
    }
}

void PersonGroups::loadGroupsComboData()
{
    QList<QMap<QString,QVariant>> group_data = this->db->selectGroups();
    
    QList<QString> gr;
    gr << "";
    for (int i=0; i < group_data.length(); ++i)
    {
        gr.append(group_data.at(i)["name"].toString());
        
        int rowid = group_data.at(i)["rowid"].toInt();
        QString group = group_data.at(i)["name"].toString();
        group_data_map[group] = rowid;
    }
    
    this->combo_group->addItems(gr);
}
