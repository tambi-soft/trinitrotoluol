#include "person_relations.h"

PersonRelations::PersonRelations(DbAdapter *db, qlonglong rowid, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    this->rowid_people = rowid;
    
    loadRelationsComboData();
    connect(combo_relations, qOverload<int>(&QComboBox::currentIndexChanged), this, &PersonRelations::addPersonToRelation);
    
    QPushButton *button_edit = new QPushButton("Edit Relations");
    connect(button_edit, &QPushButton::clicked, this, &PersonRelations::onEditRelationsButton);
    
    QPushButton *button_help = new QPushButton;
    button_help->setIcon(QIcon::fromTheme("dialog-question"));
    button_help->setFixedWidth(25);
    connect(button_help, &QPushButton::clicked, this, []{ new HelpMessage(":help_relations"); });
    
    QHBoxLayout *layout_controls = new QHBoxLayout;
    //layout_controls->addWidget(button_new);
    layout_controls->addWidget(this->combo_relations);
    layout_controls->addWidget(button_edit);
    layout_controls->addWidget(button_help);
    
    this->layout->addLayout(layout_controls);
    
    showData();
}

void PersonRelations::showData()
{
     recreateView();
     
     QList<QMap<QString,QVariant>> data = this->db->relationsMatrixSelect(this->rowid_people);
     
     QLabel *label_relations = new QLabel("<b>Relation</b>");
     QLabel *label_people = new QLabel("<b>Person</b>");
     
     this->grid->addWidget(label_relations, 0, 2);
     this->grid->addWidget(label_people, 0, 3);
     
     for (int i=0; i < data.length(); ++i)
     {
         qlonglong rowid_a = data.at(i)["rowid_people_a"].toLongLong();
         qlonglong rowid_b = data.at(i)["rowid_people_b"].toLongLong();
         qlonglong rowid_peoples_relations = data.at(i)["rowid_people_relations"].toLongLong();
         QString name = "";
         if (rowid_a == this->rowid_people)
         {
             name = data.at(i)["name_b"].toString();
         }
         else
         {
             name = data.at(i)["name_a"].toString();
         }
         QString name_relation = data.at(i)["name"].toString();
         
         QPushButton *button_delete = new QPushButton();
         button_delete->setIcon(QIcon::fromTheme("edit-delete"));
         connect(button_delete, &QPushButton::clicked, this, [this, rowid_a, rowid_b, rowid_peoples_relations, name, name_relation]{ onDeleteButtonClicked(rowid_a, rowid_b, rowid_peoples_relations, name, name_relation); });
         
         
         
         QLabel *label_relation = new QLabel(data.at(i)["name"].toString());
         QLabel *label_name = new QLabel(name);
         
         this->grid->addWidget(button_delete, i+1, 0);
         
         this->grid->addWidget(label_relation, i+1, 2);
         this->grid->addWidget(label_name, i+1, 3);
     }
}

void PersonRelations::onDeleteButtonClicked(qlonglong rowid_a, qlonglong rowid_b, qlonglong rowid_peoples_relations, QString name, QString relation)
{
    int reply = QMessageBox::question(this, "Delete "+relation, "Really delete relation \""+relation+"\" to \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->relationMatrixDelete(rowid_a, rowid_b, rowid_peoples_relations);
        showData();
    }
}

void PersonRelations::addPersonToRelation(qlonglong rowid_relations)
{
    QString relation_str = this->combo_relations->currentText();
    this->relation_selected = this->group_data_map[relation_str];
    
    PeopleSelector *selector = new PeopleSelector(this->db);
    connect(selector, &PeopleSelector::personSelected, this, &PersonRelations::onSaveRelation);
    
    this->dialog_select_spouse = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    this->dialog_select_spouse->setLayout(layout_dialog);
    layout_dialog->addWidget(selector);
    
    this->dialog_select_spouse->exec();
    
    blockSignals(true);
    this->combo_relations->blockSignals(true);
    this->combo_relations->setCurrentIndex(0);
    this->combo_relations->blockSignals(false);
    blockSignals(false);
}

void PersonRelations::onSaveRelation(qlonglong rowid, QString /*name*/)
{
    this->dialog_select_spouse->close();
    
    this->db->relationMatrixInsert(this->rowid_people, rowid, this->relation_selected);
    
    showData();
}

void PersonRelations::loadRelationsComboData()
{
    QList<QMap<QString,QVariant>> data = this->db->selectRelations();
    
    QList<QString> gr;
    gr << "[Add Relation]";
    for (int i=0; i < data.length(); ++i)
    {
        gr.append(data.at(i)["name"].toString());
        
        int rowid = data.at(i)["rowid"].toInt();
        QString group = data.at(i)["name"].toString();
        group_data_map[group] = rowid;
    }
    
    this->combo_relations->addItems(gr);
}

void PersonRelations::onEditRelationsButton()
{
    //GroupsEdit *edit = new GroupsEdit(this->db);
    RelationsEdit *edit = new RelationsEdit(this->db);
    //connect(edit, &CurrenciesEdit::signalUpdate, this, &CurrenciesList::updateView);

    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);

    dialog->exec();
}
