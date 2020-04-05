#include "groups_and_relations_edit.h"

GroupsAndRelationsEdit::GroupsAndRelationsEdit(QString new_button_label, QString label_group, GridWidget *parent) : GridWidget(parent)
{
    //this->db = db;
    this->label_group = label_group;
    
    setMinimumSize(460, 600);

    QPushButton *button_new = new QPushButton(new_button_label);
    connect(button_new, &QPushButton::clicked, this, &GroupsAndRelationsEdit::onNewGroupButtonClicked);
    this->layout->addWidget(button_new);

    //showData();
}

void GroupsAndRelationsEdit::reloadData()
{
    this->scroll_widget->deleteLater();
    showData();
}

void GroupsAndRelationsEdit::showData()
{
    recreateView();
    
    this->grid->addWidget(new QLabel(this->label_group), 0, 3);
    this->grid->addWidget(new QLabel("<b>Number of Group Members</b>"), 0, 4);

    //QList<QMap<QString,QVariant>> data = this->db->selectGroups();
    QList<QMap<QString,QVariant>> data = selectData();

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
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, group_name, people_count]{ GroupsAndRelationsEdit::onDeleteButtonClicked(rowid, group_name, people_count); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setToolTip("edit this group");
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, group_name, color]{ GroupsAndRelationsEdit::onGroupEditNameButton(rowid, group_name, color); });
        
        QPushButton *button_color = new QPushButton();
        button_color->setIcon(QIcon(":color"));
        button_color->setToolTip("change color");
        connect(button_color, &QPushButton::clicked, this, [this, rowid, group_name, color]{ GroupsAndRelationsEdit::onGroupColorButton(rowid, group_name, color); });
        
        QLabel *label_name = new QLabel(data.at(i)["name"].toString());
        label_name->setStyleSheet("QLabel { color : "+color.name()+"; }");
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        this->grid->addWidget(button_color, i+1, 2);
        this->grid->addWidget(label_name, i+1, 3);
        this->grid->addWidget(new QLabel(data.at(i)["count_people"].toString()), i+1, 4);
    }
}

void GroupsAndRelationsEdit::onDeleteButtonClicked(qlonglong group_id, QString group_name, int people_count)
{
    // TODO: what to do with people already assigned to this group?
    
    if (people_count == 0)
    {
        int reply = QMessageBox::question(this, "Delete "+group_name, "Really delete \""+group_name+"\"?", QMessageBox::Yes, QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            //this->db->deleteGroup(group_id);
            deleteTableEntry(group_id);
            showData();
        }
    }
    else
    {
        QMessageBox::question(this, "Delete "+group_name, "Group \""+group_name+"\" is not empty. Please remove this Group-Assignment from all People first!", QMessageBox::Ok);
    }
}

void GroupsAndRelationsEdit::onNewGroupButtonClicked()
{
    //this->rowid_new_group = this->db->insertNewGroup();
    this->rowid_new_group = insertTableEntry();
    this->color_new_group = QColor("#000000");
    
    showData();
    
    showNewGroupDialog("");
}

void GroupsAndRelationsEdit::showNewGroupDialog(QString name)
{
    // put together dialog content
    QWidget *edit = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout;
    edit->setLayout(layout);
    QLineEdit *line = new QLineEdit();
    line->setText(name);
    layout->addWidget(line);
    connect(line, &QLineEdit::textChanged, this, &GroupsAndRelationsEdit::onGroupNameChanged);
    connect(line, &QLineEdit::textChanged, this, &GroupsAndRelationsEdit::reloadData);

    // put together the dialog itself and show
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);

    dialog->exec();
}

void GroupsAndRelationsEdit::onGroupNameChanged(QString name)
{
    //this->db->updateGroup(this->rowid_new_group, name, this->color_new_group.name());
    updateTableEntry(this->rowid_new_group, name, this->color_new_group.name());
}

void GroupsAndRelationsEdit::onGroupEditNameButton(qlonglong rowid_groups, QString name, QColor color_current)
{
    this->rowid_new_group = rowid_groups;
    this->color_new_group = color_current;
    
    showNewGroupDialog(name);
}

void GroupsAndRelationsEdit::onGroupColorButton(qlonglong rowid_groups, QString name, QColor color_current)
{
    QColor color = QColorDialog::getColor(color_current, this );
    if( color.isValid() )
    {
        //this->db->updateGroup(rowid_groups, name, color.name());
        updateTableEntry(rowid_groups, name, color.name());
        showData();
    }
}

// to be overloaded
qlonglong GroupsAndRelationsEdit::insertTableEntry()
{
    return -1;
}

// to be overloaded
void GroupsAndRelationsEdit::updateTableEntry(qlonglong /*rowid*/, QString /*name*/, QColor /*color*/)
{
    
}

// to be overloaded
void GroupsAndRelationsEdit::deleteTableEntry(qlonglong /*rowid*/)
{
    
}

// to be overloaded
QList<QMap<QString,QVariant>> GroupsAndRelationsEdit::selectData()
{
    QList<QMap<QString,QVariant>> result;
    return result;
}
