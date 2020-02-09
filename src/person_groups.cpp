#include "person_groups.h"

PersonGroups::PersonGroups(DbAdapter *db, qlonglong rowid_people, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    this->rowid_people = rowid_people;
    
    //QPushButton *button_add_group = new QPushButton("Add Person to Group");
    //this->layout->addWidget(button_add_group);
    
    loadGroupsComboData();
    connect(combo_group, qOverload<int>(&QComboBox::currentIndexChanged), this, &PersonGroups::addPersonToGroup);
    
    QPushButton *button_edit_groups = new QPushButton("Edit Groups");
    connect(button_edit_groups, &QPushButton::clicked, this, &PersonGroups::onAddNewGroupButton);
    
    QWidget *widget_controls = new QWidget;
    QHBoxLayout *layout_controls = new QHBoxLayout;
    widget_controls->setLayout(layout_controls);
    layout_controls->setMargin(0);
    
    layout_controls->addWidget(this->combo_group);
    layout_controls->addWidget(button_edit_groups);
    
    this->layout->addWidget(widget_controls);
    
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
    gr << "[Select Groups]";
    for (int i=0; i < group_data.length(); ++i)
    {
        gr.append(group_data.at(i)["name"].toString());
        
        int rowid = group_data.at(i)["rowid"].toInt();
        QString group = group_data.at(i)["name"].toString();
        group_data_map[group] = rowid;
    }
    
    this->combo_group->addItems(gr);
}

void PersonGroups::onAddNewGroupButton()
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



GroupsEdit::GroupsEdit(DbAdapter *db, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    
    setMinimumSize(460, 600);

    QPushButton *button_new = new QPushButton("add new group");
    connect(button_new, &QPushButton::clicked, this, &GroupsEdit::onNewGroupButtonClicked);
    this->layout->addWidget(button_new);

    showData();
}

void GroupsEdit::reloadData()
{
    this->scroll_widget->deleteLater();
    showData();
}

void GroupsEdit::showData()
{
    recreateView();
    
    this->grid->addWidget(new QLabel("<b>Group Name</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Number of Group Members</b>"), 0, 4);

    QList<QMap<QString,QVariant>> data = this->db->selectGroups();

    for (int i=0; i < data.length(); i++)
    {
        qlonglong rowid = data.at(i)["rowid"].toLongLong();
        int people_count = data.at(i)["count_people"].toInt();
        QString group_name = data.at(i)["name"].toString();
        QColor color = QColor(data.at(i)["color"].toString());
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        button_delete->setToolTip("delete name");
        button_delete->setMaximumWidth(25);
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, group_name, people_count]{ GroupsEdit::onDeleteButtonClicked(rowid, group_name, people_count); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setToolTip("edit this group");
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, group_name, color]{ GroupsEdit::onGroupEditNameButton(rowid, group_name, color); });
        
        QPushButton *button_color = new QPushButton();
        button_color->setIcon(QIcon(":color"));
        button_color->setToolTip("change color");
        connect(button_color, &QPushButton::clicked, this, [this, rowid, group_name, color]{ GroupsEdit::onGroupColorButton(rowid, group_name, color); });
        
        QLabel *label_name = new QLabel(data.at(i)["name"].toString());
        label_name->setStyleSheet("QLabel { color : "+color.name()+"; }");
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        this->grid->addWidget(button_color, i+1, 2);
        this->grid->addWidget(label_name, i+1, 3);
        this->grid->addWidget(new QLabel(data.at(i)["count_people"].toString()), i+1, 4);
    }
}

void GroupsEdit::onDeleteButtonClicked(qlonglong group_id, QString group_name, int people_count)
{
    // TODO: what to do with people already assigned to this group?
    
    if (people_count == 0)
    {
        int reply = QMessageBox::question(this, "Delete "+group_name, "Really delete \""+group_name+"\"?", QMessageBox::Yes, QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            this->db->deleteGroup(group_id);
            showData();
        }
    }
    else
    {
        QMessageBox::question(this, "Delete "+group_name, "Group \""+group_name+"\" is not empty. Please remove this Group-Assignment from all People first!", QMessageBox::Ok);
    }
}

void GroupsEdit::onNewGroupButtonClicked()
{
    this->rowid_new_group = this->db->insertNewGroup();

    // put together dialog content
    QWidget *edit = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout;
    edit->setLayout(layout);
    QLineEdit *line = new QLineEdit();
    layout->addWidget(line);
    connect(line, &QLineEdit::textChanged, this, &GroupsEdit::onGroupNameChanged);
    connect(line, &QLineEdit::textChanged, this, &GroupsEdit::reloadData);

    // put together the dialog itself and show
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);

    dialog->exec();
}

void GroupsEdit::onGroupNameChanged(QString name)
{
    this->db->updateGroup(this->rowid_new_group, name, QColor("#000000").name());
}

void GroupsEdit::onGroupEditNameButton(qlonglong rowid_groups, QString name, QColor color_current)
{
    
}

void GroupsEdit::onGroupColorButton(qlonglong rowid_groups, QString name, QColor color_current)
{
    QColor color = QColorDialog::getColor(color_current, this );
    if( color.isValid() )
    {
        this->db->updateGroup(rowid_groups, name, color.name());
        showData();
    }
}
