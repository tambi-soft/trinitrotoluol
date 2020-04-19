#include "firstrun.h"

FirstRun::FirstRun(QDialog *parent) : QDialog(parent)
{
    SelectDatabase *select_database = new SelectDatabase;
    connect(select_database, &SelectDatabase::databasePathSelected, this, &FirstRun::emitDatabaseSelected);
    QWidget *setup_name = new SetupName;
    
    QStackedLayout *stacked_layout = new QStackedLayout;
    stacked_layout->addWidget(select_database);
    stacked_layout->addWidget(setup_name);
    
    setLayout(stacked_layout);
}

void FirstRun::emitDatabaseSelected(QString db_path)
{
    emit this->databasePathSelected(db_path);
    
    close();
}



SelectDatabase::SelectDatabase(QWidget *parent) : QWidget(parent)
{
    setLayout(new QHBoxLayout);
    
    QPushButton *button_select_db = new QPushButton("select / create new Database");
    connect(button_select_db, &QPushButton::clicked, this, &SelectDatabase::showFileSelectDialog);
    layout()->addWidget(button_select_db);
}

void SelectDatabase::showFileSelectDialog()
{
    /* We use this for opening a new or existing database */
    QString dir_new = QFileDialog::getSaveFileName(this,
                                                   "Select Database",
                                                   QDir::homePath(),
                                                   "",
                                                   new QString(),
                                                   QFileDialog::DontConfirmOverwrite);
    
    // if the user hits "cancel", we have an empty string here
    if (!dir_new.isEmpty() || !dir_new.isNull())
    {
        emit this->databasePathSelected(dir_new);
    }
}



SetupName::SetupName(QWidget *parent) : QWidget(parent)
{
    
}
