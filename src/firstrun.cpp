#include "firstrun.h"

FirstRun::FirstRun(QString message, QDialog *parent) : QDialog(parent)
{
    SelectDatabase *select_database = new SelectDatabase;
    select_database->setMessage(message);
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
    setLayout(new QVBoxLayout);
    
    QPushButton *button_select_db = new QPushButton("select existing Database");
    connect(button_select_db, &QPushButton::clicked, this, [this]{ selectExistingFileDialog(); });
    
    QPushButton *button_create_db = new QPushButton("create new Database");
    connect(button_create_db, &QPushButton::clicked, this, [this]{ selectNewFileDialog(); });
    
    layout()->addWidget(this->label_message);
    layout()->addWidget(button_select_db);
    layout()->addWidget(button_create_db);
}

void SelectDatabase::setMessage(QString message)
{
    this->label_message->setText(message);
}

void SelectDatabase::selectNewFileDialog()
{
    QString dir_new = QFileDialog::getSaveFileName(this,
                                                   "Select Database",
                                                   QDir::homePath() + "/" + "trinitrotoluol.sqlite",
                                                   "",
                                                   new QString());
    
    // if the user hits "cancel", we have an empty string here
    if (!dir_new.isEmpty() || !dir_new.isNull())
    {
        emit this->databasePathSelected(dir_new);
    }
}

void SelectDatabase::selectExistingFileDialog()
{
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
