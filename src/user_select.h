#ifndef USERSELECT_H
#define USERSELECT_H

#include <QObject>
#include <QWidget>

#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QRadioButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>

#include "lib/grid_widget.h"
#include "help_message.h"

class UserSelect : public GridWidget
{
    Q_OBJECT
public:
    explicit UserSelect(GridWidget *parent = nullptr);
    
    void setNames(QStringList users_list);
    void showData();
    
private:
    QStringList users_list;
    
    QDialog *dialog_new_user;
    
    void onUserSelected(QString name);
    void onAddNewUser();
    void onSaveNewUser(QString user_name);
    void onDeleteUser(QString user_name);
    
signals:
    void userSelected(QString name);
    void newUserAdded(QString name);
    void deleteUser(QString name);
    
};

#endif // USERSELECT_H
