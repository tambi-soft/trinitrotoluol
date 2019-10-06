#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>

#include <QStringList>

#include "db_adapter.h"
#include "config.h"
#include "simplecrypt.h"

class SettingsWidget : public QWidget
{
public:
    SettingsWidget(Config *config, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    Config *config;
    DbAdapter *db;
    QVBoxLayout *layout;
    
    void addDatabasePathSettingsArea();
    void addEmailSettingsArea();
    
    void showFolderSelectDialog();
    QLineEdit *edit_path;
    
    void loadEmailParams();
    QLineEdit *edit_email_address;
    QSpinBox *edit_email_port;
    QComboBox *combo_connection_security;
    QComboBox *combo_authentication_method;
    QLineEdit *edit_email_username;
    QLineEdit *edit_email_password;
    quint64 KEY = 95839583;
    
private slots:
    void saveEmailParams();
    void savePort(int just_for_compatibility);
};

#endif // SETTINGS_H
