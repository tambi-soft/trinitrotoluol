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
#include <QPalette>
#include <QTabWidget>

#include <QStringList>

#include "db_adapter.h"
#include "config.h"
#include "simplecrypt.h"
#include "lib/growing_text_edit.h"

class SettingsWidget : public QWidget
{
public:
    SettingsWidget(Config *config, DbAdapter *db, QWidget *parent = nullptr);
    
private:
    Config *config;
    DbAdapter *db;
    QVBoxLayout *layout;
    
    QWidget* addGeneralSettingsArea();
    QWidget* addEmailSettingsArea();
    QWidget* addWebDavSettingsArea();
    
    void showFileSelectDialog();
    QLineEdit *edit_name;
    QLineEdit *edit_path;
    
    void loadEmailParams();
    
    QLineEdit *edit_email_from_address;
    QLineEdit *edit_email_reply;
    QLineEdit *edit_email_server_address;
    QSpinBox *edit_email_port;
    QComboBox *combo_connection_security;
    QComboBox *combo_authentication_method;
    QLineEdit *edit_email_username;
    QLineEdit *edit_email_password;
    quint64 KEY = 95839583;
    
    QLineEdit *edit_caldav_address;
    QLineEdit *edit_carddav_address;
    QLineEdit *edit_caldav_update_interval;
    QLineEdit *edit_carddav_update_interval;
    QLineEdit *edit_caldav_username;
    QLineEdit *edit_caldav_password;
    QLineEdit *edit_carddav_username;
    QLineEdit *edit_carddav_password;
    
private slots:
    void saveGeneralParams();
    void saveEmailParams();
    void savePort(int just_for_compatibility);
};





class SettingsLineEdit : public QLineEdit
{
public:
    SettingsLineEdit(DbAdapter *db, QString setting_name, EchoMode mode, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QString setting_name;
    quint64 KEY = 95839583;
    
    void textChanged();
};

#endif // SETTINGS_H
