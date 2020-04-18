#include "person_groups.h"

PersonGroups::PersonGroups(DbAdapter *db, qlonglong rowid_people, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    this->rowid_people = rowid_people;
    
    loadGroupsComboData();
    connect(combo_group, qOverload<int>(&QComboBox::currentIndexChanged), this, &PersonGroups::addPersonToGroup);
    
    QPushButton *button_edit_groups = new QPushButton("Edit Groups");
    connect(button_edit_groups, &QPushButton::clicked, this, &PersonGroups::onEditGroupsButton);
    
    QPushButton *button_help = new QPushButton;
    button_help->setIcon(QIcon::fromTheme("dialog-question"));
    button_help->setFixedWidth(25);
    connect(button_help, &QPushButton::clicked, this, []{ new HelpMessage(":help_groups"); });
    
    QHBoxLayout *layout_controls = new QHBoxLayout;
    
    layout_controls->addWidget(this->combo_group);
    layout_controls->addWidget(button_edit_groups);
    layout_controls->addWidget(button_help);
    
    this->layout->addLayout(layout_controls);
    
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
    int group = this->group_data_map[group_str];
    
    this->db->groupMatrixInsert(this->rowid_people, group);
    
    showData();
    
    blockSignals(true);
    this->combo_group->blockSignals(true);
    this->combo_group->setCurrentIndex(0);
    this->combo_group->blockSignals(false);
    blockSignals(false);
}

void PersonGroups::loadGroupsComboData()
{
    QList<QMap<QString,QVariant>> group_data = this->db->selectGroups();
    
    QList<QString> gr;
    gr << "[Add Group]";
    for (int i=0; i < group_data.length(); ++i)
    {
        gr.append(group_data.at(i)["name"].toString());
        
        int rowid = group_data.at(i)["rowid"].toInt();
        QString group = group_data.at(i)["name"].toString();
        group_data_map[group] = rowid;
    }
    
    this->combo_group->addItems(gr);
}

void PersonGroups::onEditGroupsButton()
{
    GroupsEdit *edit = new GroupsEdit(this->db);
    //connect(edit, &CurrenciesEdit::signalUpdate, this, &CurrenciesList::updateView);
    
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);

    dialog->exec();
}
