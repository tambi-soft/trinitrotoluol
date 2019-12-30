#include "people_list.h"

PeopleList::PeopleList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , combo_groups (new QComboBox)
    , check_todo (new QCheckBox)
    , check_waiting (new QCheckBox)
    , check_donating (new QCheckBox)
    , check_deactivated (new QCheckBox)
    , check_mail (new QCheckBox)
    , line_name_filter (new QLineEdit)
    , line_mail_filter (new QLineEdit)
{
    this->db = db;
    
    this->layout = new QVBoxLayout;
    setLayout(this->layout);

    this->scroll_area->setWidgetResizable(true);
    connect(this->scroll_area->verticalScrollBar(), &QScrollBar::rangeChanged, this, &PeopleList::scrollBarRangeChanged);
    
    this->check_todo->setTristate(true);
    this->check_todo->setCheckState(Qt::PartiallyChecked);
    this->check_waiting->setTristate(true);
    this->check_waiting->setCheckState(Qt::PartiallyChecked);
    this->check_donating->setTristate(true);
    this->check_donating->setCheckState(Qt::PartiallyChecked);
    this->check_deactivated->setTristate(true);
    this->check_deactivated->setCheckState(Qt::Unchecked);
    this->check_mail->setTristate(true);
    this->check_mail->setCheckState(Qt::PartiallyChecked);
    connect(this->check_todo, &QCheckBox::stateChanged, this, &PeopleList::onFilterChanged);
    connect(this->check_waiting, &QCheckBox::stateChanged, this, &PeopleList::onFilterChanged);
    connect(this->check_donating, &QCheckBox::stateChanged, this, &PeopleList::onFilterChanged);
    connect(this->check_deactivated, &QCheckBox::stateChanged, this, &PeopleList::onFilterChanged);
    connect(this->check_mail, &QCheckBox::stateChanged, this, &PeopleList::onFilterChanged);
    
    this->line_name_filter->setClearButtonEnabled(true);
    this->line_name_filter->setPlaceholderText("type a NAME here to search");
    connect(this->line_name_filter, &QLineEdit::textChanged, this, &PeopleList::onNameFilterChanged);
    
    this->line_mail_filter->setClearButtonEnabled(true);
    this->line_mail_filter->setPlaceholderText("type a MAIL address here to search");
    connect(this->line_mail_filter, &QLineEdit::textChanged, this, &PeopleList::onNameFilterChanged);
    
    // we want to have the initialization of the separator-lines structured in a bit more compact way
    QList<QFrame*> lines;
    for (int i=0; i<5; i++)
    {
        QFrame *line = new QFrame;
        line->setFrameShape(QFrame::VLine);

        lines.append(line);
    }

    // build together the filter controls
    QHBoxLayout *hbox_filters = new QHBoxLayout();
    hbox_filters->setMargin(0);
    hbox_filters->addWidget(new QLabel("ToDo:"));
    hbox_filters->addWidget(this->check_todo);
    hbox_filters->addWidget(lines.takeFirst());
    hbox_filters->addWidget(new QLabel("Waiting:"));
    hbox_filters->addWidget(this->check_waiting);
    hbox_filters->addWidget(lines.takeFirst());
    hbox_filters->addWidget(new QLabel("Mail:"));
    hbox_filters->addWidget(this->check_mail);
    hbox_filters->addWidget(lines.takeFirst());
    hbox_filters->addWidget(new QLabel("Donating:"));
    hbox_filters->addWidget(this->check_donating);
    hbox_filters->addWidget(lines.takeFirst());
    hbox_filters->addWidget(new QLabel("Deactivated:"));
    hbox_filters->addWidget(this->check_deactivated);
    hbox_filters->addWidget(lines.takeFirst());
    
    hbox_filters->addWidget(this->line_name_filter);
    hbox_filters->addWidget(this->line_mail_filter);
    
    hbox_filters->addWidget(this->combo_groups);
    QWidget *widget_filters = new QWidget();
    widget_filters->setLayout(hbox_filters);
    
    this->layout->addWidget(widget_filters);
    this->layout->addWidget(this->scroll_area);
    
    QPushButton *button_add_new_person = new QPushButton("add new person");
    layout->addWidget(button_add_new_person);
    
    connect(combo_groups, &QComboBox::currentTextChanged, this, &PeopleList::onGroupsFilterChanged);
    connect(button_add_new_person, &QPushButton::clicked, this, &PeopleList::onNewPersonButtonClicked);
    
    showGroupsFilterCombo();
}

void PeopleList::showGroupsFilterCombo()
{
    QList<QMap<QString,QVariant>> groups = this->db->selectGroups();
    
    QList<QString> gr;
    gr.append("[ALL]");
    for (int i=0; i < groups.length(); ++i)
    {
        gr.append(groups.at(i)["name"].toString());
    }
    
    this->combo_groups->blockSignals(true);
    this->combo_groups->addItems(gr);
    this->combo_groups->blockSignals(false);
}

void PeopleList::showPeople()
{
    this->grid = new QGridLayout;
    this->scroll_widget = new QWidget;
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(this->scroll_widget);

    // -1 is the value for partially checked
    int filter_todo = -1;
    if (this->check_todo->checkState() == Qt::Checked)
    {
        filter_todo = 1;
    }
    else if (this->check_todo->checkState() == Qt::Unchecked)
    {
        filter_todo = 0;
    }
    
    int filter_waiting = -1;
    if (this->check_waiting->checkState() == Qt::Checked)
    {
        filter_waiting = 1;
    }
    else if (this->check_waiting->checkState() == Qt::Unchecked)
    {
        filter_waiting = 0;
    }
    
    int filter_donating = -1;
    if (this->check_donating->checkState() == Qt::Checked)
    {
        filter_donating = 1;
    }
    else if (this->check_donating->checkState() == Qt::Unchecked)
    {
        filter_donating = 0;
    }
    
    int filter_deactivated = -1;
    if (this->check_deactivated->checkState() == Qt::Checked)
    {
        filter_deactivated = 1;
    }
    else if (this->check_deactivated->checkState() == Qt::Unchecked)
    {
        filter_deactivated = 0;
    }
    
    int filter_mail = -1;
    if (this->check_mail->checkState() == Qt::Checked)
    {
        filter_mail = 1;
    }
    else if (this->check_mail->checkState() == Qt::Unchecked)
    {
        filter_mail = 0;
    }
    
    QString str_filter_group = this->combo_groups->currentText();
    if (str_filter_group == "[ALL]")
    {
        str_filter_group = "%";
    }
    QString str_filter_name = this->line_name_filter->text();
    if (str_filter_name == "")
    {
        str_filter_name = "%";
    }
    QString str_filter_mail = this->line_mail_filter->text();
    if (str_filter_mail == "")
    {
        str_filter_mail = "%";
    }

    QList<QMap<QString,QVariant>> people = this->db->selectAllPersonsFiltered(filter_todo,
         filter_waiting,
         filter_donating,
         filter_deactivated,
         filter_mail,
         str_filter_group,
         "%"+str_filter_name+"%",
         "%"+str_filter_mail+"%");
    
    for (int i=0; i < people.length(); ++i)
    {
        QMap<QString,QVariant> person = people.at(i);
        
        qlonglong rowid = person["rowid"].toLongLong();
        QString name = person["name"].toString();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        button_delete->setMaximumWidth(40);
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onDeletePersonButtonClicked(rowid, name); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onEditPersonButtonClicked(rowid, name); });
        
        QPushButton *button_donations = new QPushButton();
        button_donations->setIcon(QIcon(":money_receive"));
        button_donations->setMaximumWidth(90);
        connect(button_donations, &QPushButton::clicked, this, []{  });
        
        QLabel *label_agreed = new QLabel("[mail]");
        label_agreed->setStyleSheet("QLabel { color : green; }");

        QLabel *label_prayer = new QLabel("[prayer]");
        label_prayer->setStyleSheet("QLabel { color: darkgreen; }");

        QLabel *label_donating = new QLabel("[donating]");
        label_donating->setStyleSheet("QLabel { color : blue; }");

        QLabel *label_todo = new QLabel("[todo]");
        label_todo->setStyleSheet("QLabel { color : red; }");

        QLabel *label_waiting = new QLabel("[waiting]");
        label_waiting->setStyleSheet("QLabel { color: orange; }");

        this->grid->addWidget(button_delete, i, 0);
        this->grid->addWidget(button_edit, i, 1);
        if (person["flag_todo"] == 1)
        {
            this->grid->addWidget(label_todo, i, 2);
        }
        if (person["flag_waiting"] == 1)
        {
            this->grid->addWidget(label_waiting, i, 3);
        }
        if (person["agreed_mail"] == 1)
        {
            this->grid->addWidget(label_agreed, i, 4);
        }
        if (person["agreed_prayer"] == 1)
        {
            this->grid->addWidget(label_prayer, i, 5);
        }
        if (person["donations_monthly"] > 0 || person["donations_monthly_promised"] > 0)
        {
            this->grid->addWidget(label_donating, i, 6);
        }
        this->grid->addWidget(new QLabel(person["name"].toString()), i, 7);
        this->grid->addWidget(new QLabel(person["group"].toString()), i, 8);
        /*
        this->table_widget->setCellWidget(i, 0, button_delete);
        this->table_widget->setCellWidget(i, 1, button_edit);
        this->table_widget->setCellWidget(i, 2, button_donations);
        this->table_widget->setItem(i, 3, flag_todo);
        this->table_widget->setItem(i, 4, flag_waiting);
        this->table_widget->setItem(i, 5, flag_donations);
        this->table_widget->setItem(i, 6, new QTableWidgetItem(person["name"].toString()));
        this->table_widget->setItem(i, 7, new QTableWidgetItem(person["group"].toString()));
        this->table_widget->setItem(i, 8, new QTableWidgetItem(person["email"].toString()));
        this->table_widget->setItem(i, 9, mail);
        this->table_widget->setItem(i, 10, prayer);
        this->table_widget->setItem(i, 11, new QTableWidgetItem(person["agreement"].toString()));
        */
    }
    // push all columns to the left for getting the table a bit more compact
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(people.length(), 100);
}

void PeopleList::refresh()
{
    //this->combo_groups->clear();

    // store the scrollbar position first to be restored in scrollBarRangeChanged afterwards
    this->scrollbar_pos = this->scroll_area->verticalScrollBar()->sliderPosition();

    this->scroll_widget->deleteLater();
    showPeople();
}

void PeopleList::onFilterChanged()
{
    refresh();
}

void PeopleList::onNameFilterChanged()
{
    // only start to search, if search pattern at least 3 chars long
    if (this->line_name_filter->text().length() >= 3 || this->line_mail_filter->text().length() >= 3)
    {
        refresh();
    }
}

void PeopleList::onGroupsFilterChanged()
{
    /*
    this->combo_groups->blockSignals(true);
    clear();
    showGroupsFilterCombo();
    this->combo_groups->blockSignals(false);
    */
    
    refresh();
}

void PeopleList::onNewPersonButtonClicked()
{
    this->data_changed = true;
    
    qlonglong rowid = this->db->insertNewPerson();
    emit editPersonSignal(rowid, "new");
}

void PeopleList::onEditPersonButtonClicked(qlonglong rowid, QString name)
{
    emit editPersonSignal(rowid, name);
}

void PeopleList::onShowDonationsButtonClicked(qlonglong rowid)
{
    emit showDonationsForPersonSignal(rowid);
}

void PeopleList::onDeletePersonButtonClicked(qlonglong rowid, QString name)
{
    int reply = QMessageBox::question(this, "Deactivate "+name, "Really deactivate \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        //this->db->deletePerson(rowid);
        this->db->deactivatePerson(rowid);
        
        refresh();
    }
}

void PeopleList::scrollBarRangeChanged(int /*min*/, int max)
{
    if (this->scrollbar_pos <= max)
    {
        this->scroll_area->verticalScrollBar()->setSliderPosition(this->scrollbar_pos);
    }
}

void PeopleList::dataChanged()
{
    this->data_changed = true;
}

void PeopleList::showEvent(QShowEvent */*event*/)
{
    if (this->data_changed)
    {
        refresh();
    }
    this->data_changed = false;
}
