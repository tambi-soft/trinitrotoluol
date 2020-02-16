#include "person_relations.h"

PersonRelations::PersonRelations(DbAdapter *db, qlonglong rowid, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    this->rowid_people = rowid;
    
    QPushButton *button_new = new QPushButton("Add Relation");
    
    
    QPushButton *button_edit = new QPushButton("Edit Relations");
    connect(button_new, &QPushButton::clicked, this, &PersonRelations::onNewButtonClicked);
    
    QHBoxLayout *layout_controls = new QHBoxLayout;
    layout_controls->addWidget(button_new);
    layout_controls->addWidget(button_edit);
    
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
         qlonglong rowid_peoples_relations = data.at(i)["rowid_peoples_relations"].toLongLong();
         QString name = "";
         if (rowid_a == this->rowid_people)
         {
             name = data.at(i)["name_b"].toString();
         }
         else
         {
             name = data.at(i)["name_a"].toString();
         }
         QString name_relation = data.at(i)["label"].toString();
         
         QPushButton *button_delete = new QPushButton();
         button_delete->setIcon(QIcon::fromTheme("edit-delete"));
         connect(button_delete, &QPushButton::clicked, this, [this, rowid_a, rowid_b, rowid_peoples_relations, name, name_relation]{ onDeleteButtonClicked(rowid_a, rowid_b, rowid_peoples_relations, name, name_relation); });
         
         
         
         QLabel *label_relation = new QLabel(data.at(i)["label"].toString());
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
        this->db->relationDelete(rowid_a, rowid_b, rowid_peoples_relations);
        showData();
    }
}

void PersonRelations::onEditButtonClicked(qlonglong rowid)
{
    /*
    JourneyVisitsEdit *edit = new JourneyVisitsEdit(rowid, this->rowid_person, this->db, QDate::currentDate().toString("yyyy-MM-dd"));
    connect(edit, &JourneyVisitsEdit::signalReload, this, &PersonVisits::showData);
    
    QDialog *dialog = new QDialog();
    QVBoxLayout *layout_dialog = new QVBoxLayout;
    layout_dialog->setMargin(0);
    dialog->setLayout(layout_dialog);
    layout_dialog->addWidget(edit);
    
    dialog->exec();
    */
}

void PersonRelations::onNewButtonClicked()
{
    qlonglong rowid_visits = this->db->relationInsert();
    
    onEditButtonClicked(rowid_visits);
}
