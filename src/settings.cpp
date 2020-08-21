#include "settings.h"

SettingsWidget::SettingsWidget(Config *config, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->config = config;
    this->db = db;
    
    setLayout(this->layout);
    
    QTabWidget *tab_widget = new QTabWidget;
    this->layout->addWidget(tab_widget);
    
    tab_widget->addTab(addGeneralSettingsArea(), "General");
    tab_widget->addTab(addEmailSettingsArea(), "Email");
    tab_widget->addTab(addWebDavSettingsArea(), "CardDav/CalDav");
    
    
}

QWidget* SettingsWidget::addGeneralSettingsArea()
{
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    widget->setLayout(layout);
    
    SettingsLineEdit *edit_name = new SettingsLineEdit(this->db, "username", QLineEdit::Normal);
    edit_name->setToolTip("Your Name (will be shown on bills / emails)");
    
    layout->addWidget(new QLabel("Your Name"), 0, 0);
    layout->addWidget(edit_name, 0, 1);
    
    layout->addWidget(new QLabel("Database Path"), 1, 0);
    this->edit_path = new QLineEdit;
    layout->addWidget(this->edit_path, 1, 1);
    edit_path->setText(this->config->getDbPath());
    
    QPushButton *button = new QPushButton("select Folder");
    layout->addWidget(button, 1, 2);
    connect(button, &QPushButton::clicked, this, &SettingsWidget::showFileSelectDialog);
    
    QLabel *hint = new QLabel("<b>Hint:</b> You have to restart <i>trinitrotoluol</i> for a changed path to take effect!");
    layout->addWidget(hint, 2, 1);
    
    layout->setRowStretch(100, 100);
    
    return widget;
}

QWidget *SettingsWidget::addEmailSettingsArea()
{
    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    widget->setLayout(layout);
    
    layout->addWidget(new QLabel("From-Address"), 0, 0);
    SettingsLineEdit *edit_email_from_address = new SettingsLineEdit(this->db, "email_from_address", QLineEdit::Normal);
    edit_email_from_address->setPlaceholderText("more often than not, this has to be the same as the \"Username\"");
    layout->addWidget(edit_email_from_address, 0, 1);
    
    layout->addWidget(new QLabel("Reply-To"), 1, 0);
    SettingsLineEdit *edit_email_reply = new SettingsLineEdit(this->db, "email_reply", QLineEdit::Normal);
    edit_email_reply->setPlaceholderText("The Reply-To address. Leave empty for none");
    layout->addWidget(edit_email_reply, 1, 1);
    
    layout->addWidget(new QLabel("SMTP Server Address"), 2, 0);
    SettingsLineEdit *edit_email_server_address = new SettingsLineEdit(this->db, "email_server", QLineEdit::Normal);
    layout->addWidget(edit_email_server_address, 2, 1);
    
    layout->addWidget(new QLabel("SMTP Port"), 3, 0);
    this->edit_email_port = new QSpinBox;
    this->edit_email_port->setRange(1, 65535); // The port number is an unsigned 16-bit integer, so 65535
    this->edit_email_port->setValue(587);
    layout->addWidget(this->edit_email_port, 3, 1);
    
    layout->addWidget(new QLabel("Connection Security"), 4, 0);
    this->combo_connection_security = new QComboBox;
    QStringList connection_security;
    connection_security << "None" << "STARTTLS" << "SSL/TLS";
    this->combo_connection_security->addItems(connection_security);
    layout->addWidget(this->combo_connection_security, 4, 1);
    
    layout->addWidget(new QLabel("Authentication Method"), 5, 0);
    this->combo_authentication_method = new QComboBox;
    QStringList authentication_medhod;
    authentication_medhod << "No Authentication" << "Normal Password" << "Encrypted Password" << "Kerberos / GSSAPI" << "NTLM" << "OAuth2";
    this->combo_authentication_method->addItems(authentication_medhod);
    layout->addWidget(this->combo_authentication_method, 5, 1);
    
    layout->addWidget(new QLabel("Username"), 6, 0);
    SettingsLineEdit *edit_email_username = new SettingsLineEdit(this->db, "email_username", QLineEdit::Normal);
    layout->addWidget(edit_email_username, 6, 1);
    
    layout->addWidget(new QLabel("Password"), 7, 0);
    SettingsLineEdit *edit_email_password = new SettingsLineEdit(this->db, "email_password", QLineEdit::Password);
    edit_email_password->setEchoMode(QLineEdit::Password);
    layout->addWidget(edit_email_password, 7, 1);
    
    loadEmailParams();
    connect(this->edit_email_port, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::savePort);
    connect(this->combo_connection_security, &QComboBox::currentTextChanged, this, &SettingsWidget::saveEmailParams);
    connect(this->combo_authentication_method, &QComboBox::currentTextChanged, this, &SettingsWidget::saveEmailParams);
    
    layout->setRowStretch(100, 100);
    
    return widget;
}

QWidget *SettingsWidget::addWebDavSettingsArea()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);
    
    QGroupBox *group_caldav = new QGroupBox("CalDav Settings");
    QGridLayout *layout_caldav = new QGridLayout;
    group_caldav->setLayout(layout_caldav);
    layout->addWidget(group_caldav);
    
    GrowingTextEdit *help_caldav = new GrowingTextEdit;
    help_caldav->loadTextFromAssets(":help_caldav");
    layout_caldav->addWidget(help_caldav, 0, 0, 1, 2);
    
    layout_caldav->addWidget(new QLabel("CalDav-Address"), 1, 0);
    SettingsLineEdit *edit_caldav_address = new SettingsLineEdit(this->db, "caldav-server", QLineEdit::Normal);
    edit_caldav_address->setPlaceholderText("URL to your CalDav-Server");
    layout_caldav->addWidget(edit_caldav_address, 1, 1);
    
    layout_caldav->addWidget(new QLabel("CalDav Update Interval [Minutes]"), 2, 0);
    SettingsLineEdit *edit_caldav_update_interval = new SettingsLineEdit(this->db, "caldav-update-interval", QLineEdit::Normal);
    edit_caldav_update_interval->setValidator(new QIntValidator(1, 1500));
    edit_caldav_update_interval->setPlaceholderText("Default or Empty Value: every 5 Minutes");
    layout_caldav->addWidget(edit_caldav_update_interval, 2, 1);
    
    layout_caldav->addWidget(new QLabel("CalDav Username"), 3, 0);
    SettingsLineEdit *edit_caldav_username = new SettingsLineEdit(this->db, "caldav-username", QLineEdit::Normal);
    edit_caldav_username->setPlaceholderText("Username for your CalDav-Server");
    layout_caldav->addWidget(edit_caldav_username, 3, 1);
    
    layout_caldav->addWidget(new QLabel("CalDav Password"), 4, 0);
    SettingsLineEdit *edit_caldav_password = new SettingsLineEdit(this->db, "caldav-password", QLineEdit::Password);
    edit_caldav_password->setPlaceholderText("Password for your CalDav-Server");
    layout_caldav->addWidget(edit_caldav_password, 4, 1);
    
    QPushButton *button_caldav_sync = new QPushButton("Synchronize Now");
    connect(button_caldav_sync, &QPushButton::clicked, this, &SettingsWidget::syncronizeCalDavNow);
    layout_caldav->addWidget(button_caldav_sync);
    
    
    QGroupBox *group_carddav = new QGroupBox("CardDav Settings");
    QGridLayout *layout_carddav = new QGridLayout;
    group_carddav->setLayout(layout_carddav);
    layout->addWidget(group_carddav);
    
    GrowingTextEdit *help_carddav = new GrowingTextEdit;
    help_carddav->loadTextFromAssets(":help_carddav");
    layout_carddav->addWidget(help_carddav, 0, 0, 1, 2);
    
    layout_carddav->addWidget(new QLabel("CardDav-Address"), 1, 0);
    SettingsLineEdit *edit_carddav_address = new SettingsLineEdit(this->db, "carddav-server", QLineEdit::Normal);
    edit_carddav_address->setPlaceholderText("URL to your CardDav-Server");
    layout_carddav->addWidget(edit_carddav_address, 1, 1);
    
    layout_carddav->addWidget(new QLabel("CardDav Update Interval [Minutes]"), 2, 0);
    SettingsLineEdit *edit_carddav_update_interval = new SettingsLineEdit(this->db, "carddav-update-interval", QLineEdit::Normal);
    edit_carddav_update_interval->setValidator(new QIntValidator(1, 1500));
    edit_carddav_update_interval->setPlaceholderText("Default or Empty Value: every 5 Minutes");
    layout_carddav->addWidget(edit_carddav_update_interval, 2, 1);
    
    layout_carddav->addWidget(new QLabel("CardDav Username"), 3, 0);
    SettingsLineEdit *edit_carddav_username = new SettingsLineEdit(this->db, "carddav-username", QLineEdit::Normal);
    edit_carddav_username->setPlaceholderText("Username to your CardDav-Server");
    layout_carddav->addWidget(edit_carddav_username, 3, 1);
    
    layout_carddav->addWidget(new QLabel("CardDav Password"), 4, 0);
    SettingsLineEdit *edit_carddav_password = new SettingsLineEdit(this->db, "carddav-password", QLineEdit::Password);
    edit_carddav_password->setPlaceholderText("Password for your CardDav-Server");
    layout_carddav->addWidget(edit_carddav_password, 4, 1);
    
    QPushButton *button_carddav_sync = new QPushButton("Synchronize Now");
    connect(button_carddav_sync, &QPushButton::clicked, this, &SettingsWidget::syncronizeCardDavNow);
    layout_carddav->addWidget(button_carddav_sync);
    
    layout->addStretch();
    
    return widget;
}

void SettingsWidget::showFileSelectDialog()
{
    QString dir_old = this->edit_path->text();
    QString dir_new = QFileDialog::getOpenFileName(this, tr("Select Directory"), dir_old);
    
    // if the user hit "cancel", we have an empty string here
    if (!dir_new.isEmpty() || !dir_new.isNull())
    {
        this->edit_path->setText(dir_new);
        this->config->setDbPath(dir_new);
    }
}

void SettingsWidget::savePort(int /*just_for_compatibility*/)
{
    saveEmailParams();
}

void SettingsWidget::saveEmailParams()
{
    int email_port = this->edit_email_port->value();
    QString email_security = this->combo_connection_security->currentText();
    QString email_auth = this->combo_authentication_method->currentText();
    
    this->db->insertSettings("email_port", email_port);
    this->db->insertSettings("email_security", email_security);
    this->db->insertSettings("email_auth", email_auth);
}

void SettingsWidget::loadEmailParams()
{
    int email_port = this->db->selectSettings("email_port").toInt();
    QString email_security = this->db->selectSettings("email_security");
    QString email_auth = this->db->selectSettings("email_auth");
    
    this->edit_email_port->setValue(email_port);
    this->combo_connection_security->setCurrentText(email_security);
    this->combo_authentication_method->setCurrentText(email_auth);
    
}

void SettingsWidget::syncronizeCalDavNow()
{
    
}

void SettingsWidget::syncronizeCardDavNow()
{
    
}





SettingsLineEdit::SettingsLineEdit(DbAdapter *db, QString setting_name, EchoMode mode, QWidget *parent) : QLineEdit(parent)
{
    this->db = db;
    this->setting_name = setting_name;
    setEchoMode(mode);
    
    if (this->echoMode() == QLineEdit::Password)
    {
        SimpleCrypt processSimpleCrypt(KEY);
        QString pw_dec = processSimpleCrypt.decryptToString(this->db->selectSettings(this->setting_name));
        
        this->setText(pw_dec);
    }
    else
    {
        this->setText(this->db->selectSettings(this->setting_name));
    }
    
    connect(this, &QLineEdit::textChanged, this, &SettingsLineEdit::textChanged);
}

void SettingsLineEdit::textChanged()
{
    if (this->echoMode() == QLineEdit::Password)
    {
        SimpleCrypt processSimpleCrypt(KEY);
        QString pw_enc = processSimpleCrypt.encryptToString(this->text());
        
        this->db->insertSettings(this->setting_name, pw_enc);
    }
    else
    {
        this->db->insertSettings(this->setting_name, this->text());
    }
}
