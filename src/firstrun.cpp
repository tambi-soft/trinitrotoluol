#include "firstrun.h"

FirstRun::FirstRun(QString message, QDialog *parent) : QDialog(parent)
{
    //setWindowFlags(Qt::WindowTitleHint);
    //setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    
    SelectDatabase *select_database = new SelectDatabase;
    select_database->setMessage(message);
    connect(select_database, &SelectDatabase::databasePathSelected, this, &FirstRun::onDatabaseSelected);
    connect(select_database, &SelectDatabase::signalAbort, this, [this]{ emit signalAbort(); } );
    
    QWidget *setup_name = new SetupName;
    
    QWidget *setup_currency = new SetupCurrency;
    
    this->stacked_layout = new QStackedLayout;
    this->stacked_layout->addWidget(select_database);
    this->stacked_layout->addWidget(setup_name);
    this->stacked_layout->addWidget(setup_currency);
    
    this->layers["SELECT_DATABASE"] = 0;
    this->layers["SETUP_NAME"] = 1;
    this->layers["SETUP_CURRENCY"] = 2;
    
    setLayout(this->stacked_layout);
}

void FirstRun::onDatabaseSelected(QString db_path)
{
    emit this->databasePathSelected(db_path);
    
    //close();
    // switch to the next Widget
    this->stacked_layout->setCurrentIndex(this->layers["SETUP_CURRENCY"]);
}



SelectDatabase::SelectDatabase(QWidget *parent) : QWidget(parent)
{
    setLayout(new QVBoxLayout);
    
    QPushButton *button_select_db = new QPushButton("select existing Database");
    connect(button_select_db, &QPushButton::clicked, this, [this]{ selectExistingFileDialog(); });
    
    QPushButton *button_create_db = new QPushButton("create new Database");
    connect(button_create_db, &QPushButton::clicked, this, [this]{ selectNewFileDialog(); });
    
    QPushButton *button_abort = new QPushButton("Abort");
    connect(button_abort, &QPushButton::clicked, this, [this]{emit signalAbort();} );
    
    layout()->addWidget(this->label_message);
    layout()->addWidget(button_select_db);
    layout()->addWidget(button_create_db);
    layout()->addWidget(button_abort);
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



SetupCurrency::SetupCurrency(QWidget *parent) : QWidget(parent)
{
    setLayout(new QVBoxLayout);
    
    //GrowingTextEdit *help = new GrowingTextEdit;
    //help->loadTextFromAssets(":help_currencies");
    //layout()->addWidget(help);
    
    
}
