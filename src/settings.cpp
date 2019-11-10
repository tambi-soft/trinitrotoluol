#include "settings.h"

SettingsWidget::SettingsWidget(Config *config, DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->config = config;
    this->db = db;
    
    setLayout(this->layout);
    
    addGeneralSettingsArea();
    addDatabasePathSettingsArea();
    addEmailSettingsArea();
    
    this->layout->addStretch();
}

void SettingsWidget::addGeneralSettingsArea()
{
    QGroupBox *group = new QGroupBox("General");
    QVBoxLayout *layout = new QVBoxLayout;
    group->setLayout(layout);
    this->layout->addWidget(group);
    
    this->edit_name = new QLineEdit;
    this->edit_name->setText(this->db->selectSettings("username"));
    connect(this->edit_name, &QLineEdit::textChanged, this, &SettingsWidget::saveGeneralParams);
    
    layout->addWidget(new QLabel("Your Name (will be shown on bills / emails)"));
    layout->addWidget(this->edit_name);
}

void SettingsWidget::addDatabasePathSettingsArea()
{
    QGroupBox *group = new QGroupBox("Path for the Database");
    QVBoxLayout *layout_outer = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout();
    
    this->edit_path = new QLineEdit;
    
    layout->addWidget(this->edit_path);
    
    edit_path->setText(this->config->getDbPath());
    
    QPushButton *button = new QPushButton("select Folder");
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, &SettingsWidget::showFolderSelectDialog);
    
    QWidget *edit_and_button_widget = new QWidget;
    layout->setMargin(0);
    edit_and_button_widget->setLayout(layout);
    
    QLabel *hint = new QLabel("<b>Hint:</b> You have to restart <i>trinitrotoluol</i> for a changed path to take effect!");
    layout_outer->addWidget(edit_and_button_widget);
    layout_outer->addWidget(hint);
    
    group->setLayout(layout_outer);
    this->layout->addWidget(group);
}

void SettingsWidget::addEmailSettingsArea()
{
    QGroupBox *group = new QGroupBox("Email Settings");
    QVBoxLayout *layout = new QVBoxLayout;
    group->setLayout(layout);
    this->layout->addWidget(group);
    
    layout->addWidget(new QLabel("From-Address"));
    this->edit_email_from_address = new QLineEdit;
    this->edit_email_from_address->setPlaceholderText("more often than not, this has to be the same as the \"Username\"");
    layout->addWidget(this->edit_email_from_address);
    
    layout->addWidget(new QLabel("Reply-To"));
    this->edit_email_reply = new QLineEdit;
    this->edit_email_reply->setPlaceholderText("The Reply-To address. Leave empty for none");
    layout->addWidget(this->edit_email_reply);
    
    layout->addWidget(new QLabel("SMTP Server Address"));
    this->edit_email_server_address = new QLineEdit;
    layout->addWidget(this->edit_email_server_address);
    
    layout->addWidget(new QLabel("SMTP Port"));
    this->edit_email_port = new QSpinBox;
    this->edit_email_port->setRange(1, 65535); // The port number is an unsigned 16-bit integer, so 65535
    this->edit_email_port->setValue(587);
    layout->addWidget(this->edit_email_port);
    
    layout->addWidget(new QLabel("Connection Security"));
    this->combo_connection_security = new QComboBox;
    QStringList connection_security;
    connection_security << "None" << "STARTTLS" << "SSL/TLS";
    this->combo_connection_security->addItems(connection_security);
    layout->addWidget(this->combo_connection_security);
    
    layout->addWidget(new QLabel("Authentication Method"));
    this->combo_authentication_method = new QComboBox;
    QStringList authentication_medhod;
    authentication_medhod << "No Authentication" << "Normal Password" << "Encrypted Password" << "Kerberos / GSSAPI" << "NTLM" << "OAuth2";
    this->combo_authentication_method->addItems(authentication_medhod);
    layout->addWidget(this->combo_authentication_method);
    
    layout->addWidget(new QLabel("Username"));
    this->edit_email_username = new QLineEdit;
    layout->addWidget(this->edit_email_username);
    
    layout->addWidget(new QLabel("Password"));
    this->edit_email_password = new QLineEdit;
    this->edit_email_password->setEchoMode(QLineEdit::Password);
    layout->addWidget(this->edit_email_password);
    
    loadEmailParams();
    
    connect(this->edit_email_from_address, &QLineEdit::textChanged, this, &SettingsWidget::saveEmailParams);
    
    connect(this->edit_email_server_address, &QLineEdit::textChanged, this, &SettingsWidget::saveEmailParams);
    
    connect(this->edit_email_reply, &QLineEdit::textChanged, this, &SettingsWidget::saveEmailParams);
    
    connect(this->edit_email_port, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::savePort);
    
    connect(this->edit_email_username, &QLineEdit::textChanged, this, &SettingsWidget::saveEmailParams);
    connect(this->edit_email_password, &QLineEdit::textChanged, this, &SettingsWidget::saveEmailParams);
    
    connect(this->combo_connection_security, &QComboBox::currentTextChanged, this, &SettingsWidget::saveEmailParams);
    connect(this->combo_authentication_method, &QComboBox::currentTextChanged, this, &SettingsWidget::saveEmailParams);
}

void SettingsWidget::showFolderSelectDialog()
{
    QString dir_old = this->edit_path->text();
    QString dir_new = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
        dir_old,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    //qDebug() << "DIR:" << dir_new;
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
    QString email_from_address = this->edit_email_from_address->text();
    QString email_reply = this->edit_email_reply->text();
    QString email_server = this->edit_email_server_address->text();
    int email_port = this->edit_email_port->value();
    QString email_security = this->combo_connection_security->currentText();
    QString email_auth = this->combo_authentication_method->currentText();
    QString email_username = this->edit_email_username->text();
    QString email_password = this->edit_email_password->text();
    
    //SimpleCrypt crypto(Q_UINT64_C(0xbe890cb92bce0900)); //some random number
    //QString encryptToString(const QString& plaintext);
    
    SimpleCrypt processSimpleCrypt(KEY);
    QString email_pw_enc = processSimpleCrypt.encryptToString(email_password);
    
    this->db->insertSettings("email_from_address", email_from_address);
    this->db->insertSettings("email_server", email_server);
    this->db->insertSettings("email_port", email_port);
    this->db->insertSettings("email_security", email_security);
    this->db->insertSettings("email_auth", email_auth);
    this->db->insertSettings("email_username", email_username);
    this->db->insertSettings("email_password", email_pw_enc);
    this->db->insertSettings("email_reply", email_reply);
}

void SettingsWidget::loadEmailParams()
{
    QString email_from_address = this->db->selectSettings("email_from_address");
    QString email_reply = this->db->selectSettings("email_reply");
    QString email_server = this->db->selectSettings("email_server");
    int email_port = this->db->selectSettings("email_port").toInt();
    QString email_security = this->db->selectSettings("email_security");
    QString email_auth = this->db->selectSettings("email_auth");
    QString email_username = this->db->selectSettings("email_username");
    QString email_password = this->db->selectSettings("email_password");
    
    this->edit_email_from_address->setText(email_from_address);
    this->edit_email_reply->setText(email_reply);
    this->edit_email_server_address->setText(email_server);
    this->edit_email_port->setValue(email_port);
    this->combo_connection_security->setCurrentText(email_security);
    this->combo_authentication_method->setCurrentText(email_auth);
    this->edit_email_username->setText(email_username);
    
    SimpleCrypt processSimpleCrypt(KEY);
    QString email_pw_dec = processSimpleCrypt.decryptToString(email_password);
    this->edit_email_password->setText(email_pw_dec);
}

void SettingsWidget::saveGeneralParams()
{
    QString username = this->edit_name->text();
    
    this->db->insertSettings("username", username);
}
