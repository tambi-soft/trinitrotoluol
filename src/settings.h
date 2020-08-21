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
    
    // edit_path is the only entity here not saved in the db but the ini-file!
    QLineEdit *edit_path;
    
    void loadEmailParams();
    
    QSpinBox *edit_email_port;
    QComboBox *combo_connection_security;
    QComboBox *combo_authentication_method;
    
    
    
private slots:
    void saveEmailParams();
    void savePort(int just_for_compatibility);
    
    void synchroniseCalDavNow();
    void synchroniseCardDavNow();
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
