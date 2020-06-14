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
    
    this->layout = new QVBoxLayout(this);
    setLayout(this->layout);

    this->scroll_area->setWidgetResizable(true);
    connect(this->scroll_area->verticalScrollBar(), &QScrollBar::rangeChanged, this, &PeopleList::scrollBarRangeChanged);
    
    this->check_todo->setTristate(true);
    this->check_todo->setCheckState(Qt::PartiallyChecked);
    this->check_todo->setToolTip("Shows all People Who has set the <i>ToDo</i>-Flag, none, or both");
    this->check_waiting->setTristate(true);
    this->check_waiting->setCheckState(Qt::PartiallyChecked);
    this->check_waiting->setToolTip("Shows all People Who has set the <i>Waiting</i>-Flag, none, or both");
    this->check_donating->setTristate(true);
    this->check_donating->setCheckState(Qt::PartiallyChecked);
    this->check_donating->setToolTip("Shows all People Who has ever donated, not yet donated, or both");
    this->check_deactivated->setTristate(true);
    this->check_deactivated->setCheckState(Qt::Unchecked);
    this->check_deactivated->setToolTip("Shows all People Who are deactivated, activaded, or both");
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
    connect(this->line_mail_filter, &QLineEdit::textChanged, this, &PeopleList::onMailFilterChanged);
    
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
    
    this->layout->addLayout(hbox_filters);
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
    setUpdatesEnabled(false);
    
    this->grid = new QGridLayout(this);
    this->scroll_widget = new QWidget(this);
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
    
    QIcon icon_delete = QIcon::fromTheme("edit-delete");
    QIcon icon_edit = QIcon::fromTheme("document-properties");
    for (int i=0; i < people.length(); ++i)
    {
        QMap<QString,QVariant> person = people.at(i);
        
        qlonglong rowid = person["rowid"].toLongLong();
        QString name = person["name"].toString();
        
        QPushButton *button_delete = new QPushButton(this);
        button_delete->setIcon(icon_delete);
        button_delete->setMaximumWidth(40);
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onDeletePersonButtonClicked(rowid, name); });
        
        QPushButton *button_edit = new QPushButton(this);
        button_edit->setIcon(icon_edit);
        button_edit->setMaximumWidth(40);
        connect(button_edit, &QPushButton::clicked, this, [this, rowid, name]{ PeopleList::onEditPersonButtonClicked(rowid, name); });
        
        this->grid->addWidget(button_delete, i, 0);
        this->grid->addWidget(button_edit, i, 1);
        
        // BEGIN Flags Area
        QHBoxLayout *layout_flags = new QHBoxLayout;
        layout_flags->addStretch();
        if (person["flag_todo"] == 1)
        {
            QLabel *label_todo = new QLabel("[todo]", this);
            label_todo->setStyleSheet("QLabel { color : red; }");
            
            //this->grid->addWidget(label_todo, i, 2);
            layout_flags->addWidget(label_todo);
        }
        if (person["flag_waiting"] == 1)
        {
            QLabel *label_waiting = new QLabel("[waiting]", this);
            label_waiting->setStyleSheet("QLabel { color: orange; }");
            
            //this->grid->addWidget(label_waiting, i, 3);
            layout_flags->addWidget(label_waiting);
        }
        if (person["agreed_prayer"] == 1)
        {
            QLabel *label_prayer = new QLabel("[prayer]", this);
            label_prayer->setStyleSheet("QLabel { color: darkgreen; }");
            
            //this->grid->addWidget(label_prayer, i, 5);
            layout_flags->addWidget(label_prayer);
        }
        if (person["donations_monthly_promised"] > 0)
        {
            QLabel *label_donating = new QLabel("[donation promised]", this);
            label_donating->setStyleSheet("QLabel { color : blue; }");
            
            //this->grid->addWidget(label_donating, i, 6);
            layout_flags->addWidget(label_donating);
        }
        if (person["donations_received"] > 0)
        {
            QLabel *label_donating = new QLabel("[donating]", this);
            label_donating->setStyleSheet("QLabel { color : darkblue; }");
            
            //this->grid->addWidget(label_donating, i, 7);
            layout_flags->addWidget(label_donating);
        }
        if (person["agreed_mail"] == 1)
        {
            QLabel *label_agreed = new QLabel("[mail]", this);
            label_agreed->setStyleSheet("QLabel { color : green; }");
            
            //this->grid->addWidget(label_agreed, i, 4);
            layout_flags->addWidget(label_agreed);
        }
        layout_flags->setAlignment(Qt::AlignRight);
        this->grid->addLayout(layout_flags, i, 5);
        // END Flags Area
        
        this->grid->addWidget(new QLabel(person["name"].toString(), this), i, 8);
        //this->grid->addWidget(new QLabel(person["group"].toString(), this), i, 9);
        
        // BEGIN Groups Area
        QString groups_names = person["groups_names"].toString();
        QString groups_colors = person["groups_colors"].toString();
        QStringList groups_names_splitted = groups_names.split(",");
        QStringList groups_colors_splitted = groups_colors.split(",");
        
        QHBoxLayout *layout_groups = new QHBoxLayout;
        layout_groups->setMargin(0);
        for (int j=0; j < groups_names_splitted.length(); ++j)
        {
            QLabel *label_group = new QLabel(groups_names_splitted.at(j));
            if (groups_names_splitted.length() == groups_colors_splitted.length())
            {
                label_group->setStyleSheet("QLabel { color : "+QColor(groups_colors_splitted.at(j)).name()+"; }");
            }
            
            layout_groups->addWidget(label_group);
        }
        layout_groups->addStretch();
        layout_groups->setAlignment(Qt::AlignLeft);
        this->grid->addLayout(layout_groups, i, 9);
        // END Groups Area
    }
    // push all columns to the left for getting the table a bit more compact
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(people.length(), 100);
    
    setUpdatesEnabled(true);
}

void PeopleList::refresh()
{
    //this->combo_groups->clear();

    // store the scrollbar position first to be restored in scrollBarRangeChanged afterwards
    this->scrollbar_pos = this->scroll_area->verticalScrollBar()->sliderPosition();

    this->scroll_widget->deleteLater();
    this->grid = nullptr;
    showPeople();
}

void PeopleList::onFilterChanged()
{
    refresh();
}

void PeopleList::onNameFilterChanged()
{
    // only start to search, if search pattern at least 3 chars long
    // == 0, for if clear button pressed
    if (this->line_name_filter->text().length() >= 3 || this->line_name_filter->text().length() == 0)
    {
        refresh();
    }
}
void PeopleList::onMailFilterChanged()
{
    // only start to search, if search pattern at least 3 chars long
    // == 0, for if clear button pressed
    if (this->line_mail_filter->text().length() >= 3 || this->line_mail_filter->text().length() == 0)
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
    int reply;
    if (this->check_deactivated->checkState() == Qt::Checked)
    {
        reply = QMessageBox::question(this, "Delete "+name, "Really delete \""+name+"\"?\nThis action can not be undone!", QMessageBox::Yes, QMessageBox::No);
    }
    else
    {
        reply = QMessageBox::question(this, "Deactivate "+name, "Really deactivate \""+name+"\"?", QMessageBox::Yes, QMessageBox::No);
    }
    if (reply == QMessageBox::Yes)
    {
        if (this->check_deactivated->checkState() == Qt::Checked)
        {
            this->db->deletePerson(rowid);
        }
        else
        {
            this->db->deactivatePerson(rowid);
        }
        
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
