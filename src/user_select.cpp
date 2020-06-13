#include "user_select.h"

UserSelect::UserSelect(GridWidget *parent) : GridWidget(parent)
{
    QPushButton *button_add_new = new QPushButton("Add New User");
    connect(button_add_new, &QPushButton::clicked, this, &UserSelect::onAddNewUser);
    
    this->layout->insertWidget(0, new QLabel("Please select or add your Identity!"));
    this->layout->insertWidget(100, button_add_new);
}

void UserSelect::setNames(QStringList users_list)
{
    this->users_list = users_list;
    
    showData();
}

void UserSelect::showData()
{
    recreateView();
    
    for (int i=0; i < this->users_list.length(); ++i)
    {
        QString username = this->users_list.at(i);
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        connect(button_delete, &QPushButton::clicked, this, [this, username]{ onDeleteUser(username); });
        
        QPushButton *button_name = new QPushButton(username);
        connect(button_name, &QPushButton::clicked, this, [this, username]{ onUserSelected(username); });
        
        //QRadioButton *radio_default = new QRadioButton("set as default");
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_name, i+1, 2);
        //this->grid->addWidget(radio_default, i+1, 3);
    }
}

void UserSelect::onUserSelected(QString name)
{
    emit userSelected(name);
}

void UserSelect::onAddNewUser()
{
    QLineEdit *line_user_name = new QLineEdit;
    line_user_name->setPlaceholderText("type here the new user name");
    
    QPushButton *button_save = new QPushButton("save");
    connect(button_save, &QPushButton::clicked, this, [this, line_user_name]{ onSaveNewUser(line_user_name->text()); });
    
    this->dialog_new_user = new QDialog;
    this->dialog_new_user->setLayout(new QVBoxLayout);
    this->dialog_new_user->layout()->addWidget(line_user_name);
    this->dialog_new_user->layout()->addWidget(button_save);
    this->dialog_new_user->exec();
}

void UserSelect::onSaveNewUser(QString user_name)
{
    emit newUserAdded(user_name);
    
    this->dialog_new_user->close();
}

void UserSelect::onDeleteUser(QString user_name)
{
    int reply = QMessageBox::question(this, "Delete User", "Delete User \""+user_name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        emit deleteUser(user_name);
    }
}
