#include "donations_receipt_generator.h"

DonationsReceiptGenerator::DonationsReceiptGenerator(DbAdapter *db, QWidget *parent)
    : QWidget{parent}
{
    this->db = db;
    
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    QString date_db_from = this->db->selectSettings("donations_receipt_date_from");
    QString date_db_to = this->db->selectSettings("donations_receipt_date_to");
    QString tex_path_input = this->db->selectSettings("donations_receipt_tex_input");
    QString tex_path_output = this->db->selectSettings("donations_receipt_tex_output");
    
    this->date_from = new QDateEdit;
    this->date_from->setCalendarPopup(true);
    this->date_from->setDate(QDate::fromString(date_db_from, "yyyy-MM-dd"));
    
    this->date_to = new QDateEdit;
    this->date_to->setCalendarPopup(true);
    this->date_to->setDate(QDate::fromString(date_db_to, "yyyy-MM-dd"));
    
    QPushButton *button_fetch = new QPushButton("Fetch Data from DataBase");
    connect(button_fetch, &QPushButton::clicked, this, &DonationsReceiptGenerator::selectDonorsFromDB);
    
    this->table = new QTableWidget;
    
    this->line_tex_input = new QLineEdit();
    this->line_tex_input->setPlaceholderText("Input LaTeX File");
    this->line_tex_input->setText(tex_path_input);
    connect(this->line_tex_input, &QLineEdit::textChanged, this, &DonationsReceiptGenerator::lineTeXInputChanged);
    
    this->line_tex_output = new QLineEdit();
    this->line_tex_output->setPlaceholderText("Output LaTeX File");
    this->line_tex_output->setText(tex_path_output);
    connect(this->line_tex_output, &QLineEdit::textChanged, this, &DonationsReceiptGenerator::lineTeXOutputChanged);
    
    QPushButton *button_tex_input = new QPushButton("Select Input LaTeX File");
    connect(button_tex_input, &QPushButton::clicked, this, &DonationsReceiptGenerator::inputFileSelect);
    QPushButton *button_tex_output = new QPushButton("Select Output LaTeX File");
    connect(button_tex_output, &QPushButton::clicked, this, &DonationsReceiptGenerator::outputFolderSelect);
    
    QPushButton *button_generate = new QPushButton("Generate LaTeX Files");
    connect(button_generate, &QPushButton::clicked, this, &DonationsReceiptGenerator::generateTexFiles);
    
    grid->addWidget(this->date_from, 0, 0, 1, 1);
    grid->addWidget(this->date_to, 0, 1, 1, 1);
    grid->addWidget(button_fetch, 0, 2, 1, 1);
    grid->addWidget(this->table, 1, 0, 1, 3);
    grid->addWidget(this->line_tex_input, 2, 0, 1, 2);
    grid->addWidget(this->line_tex_output, 3, 0, 1, 2);
    grid->addWidget(button_tex_input, 2, 2);
    grid->addWidget(button_tex_output, 3, 2);
    grid->addWidget(button_generate, 4, 0, 1, 3);
}

void DonationsReceiptGenerator::lineTeXInputChanged(QString text)
{
    this->db->insertSettings("donations_receipt_tex_input", text);
}
void DonationsReceiptGenerator::lineTeXOutputChanged(QString text)
{
    this->db->insertSettings("donations_receipt_tex_output", text);
}

void DonationsReceiptGenerator::inputFileSelect()
{
    QString tex_path_input = this->db->selectSettings("donations_receipt_tex_input");
    
    QString filepath = QFileDialog::getOpenFileName(this, "Select TeX File", tex_path_input, "LaTeX Files (*.tex)");
    this->line_tex_input->setText(filepath);
    
    this->db->insertSettings("donations_receipt_tex_input", filepath);
}
void DonationsReceiptGenerator::outputFolderSelect()
{
    QString tex_path_output = this->db->selectSettings("donations_receipt_tex_output");
    
    QString folderpath = QFileDialog::getExistingDirectory(this, "Open Output Directory", tex_path_output, QFileDialog::ShowDirsOnly);
    this->line_tex_output->setText(folderpath);
    
    this->db->insertSettings("donations_receipt_tex_output", folderpath);
}

void DonationsReceiptGenerator::selectDonorsFromDB()
{
    this->table->clearContents();
    
    QString date_from = this->date_from->date().toString("yyyy-MM-dd");
    QString date_to = this->date_to->date().toString("yyyy-MM-dd");
    this->data = this->db->selectDonationsForReceipt(date_from, date_to);
    
    this->db->insertSettings("donations_receipt_date_from", date_from);
    this->db->insertSettings("donations_receipt_date_to", date_to);
    
    if (this->data.length() >= 1)
    {
        QStringList header = this->data.at(0).keys();
        // for unknown reasons there is an empty set at the beginning; we just remove that here
        if (header.at(0) == "")
            header.removeFirst();
        header.append("amount_sum_string");
        
        this->table->setColumnCount(header.length());
        this->table->setRowCount(this->data.length()+1);
        this->table->setHorizontalHeaderLabels(header);
        
        float sum = 0;
        for (int x=0; x < this->data.length(); x++)
        {
            // adding SUM line to this->data
            sum += data.at(x)["amount_sum"].toFloat();
            
            for (int y=0; y < header.length(); y++)
            {
                // adding field "amount_sum_string" to this->data
                QString amount = data.at(x)["amount_sum"].toString().split(".").at(0);
                this->data[x]["amount_sum_string"] = QVariant(numberToStringGerman(amount));
                
                QTableWidgetItem *item = new QTableWidgetItem(this->data.at(x)[header.at(y)].toString());
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                this->table->setItem(x, y, item);
            }
        }
        
        qDebug() << this->data.length()+1;
        qDebug() << header.indexOf("amount_sum");
        this->table->setItem(
                    this->data.length(),
                    header.indexOf("amount_sum"),
                    new QTableWidgetItem("Σ: "+QString::number(sum)));
        
        this->table->resizeColumnsToContents();
    }
}

void DonationsReceiptGenerator::generateTexFiles()
{
    if (this->data.length() <= 0)
    {
        QMessageBox *msgbox = new QMessageBox(this);
        msgbox->setWindowTitle("ERROR");
        msgbox->setText("ERROR: You need to fetch some Data first by using the button 'Fetch Data from DataBase'.");
        msgbox->open();
    }
    else if (this->line_tex_input->text().length() <= 0 || this->line_tex_output->text().length() <= 0)
    {
        QMessageBox *msgbox = new QMessageBox(this);
        msgbox->setWindowTitle("ERROR");
        msgbox->setText("ERROR: You need to select the input TeX-File and the output Folder first.");
        msgbox->open();
    }
    else
    {
        QString date_from = this->date_from->date().toString("yyyy-MM-dd");
        QString date_to = this->date_to->date().toString("yyyy-MM-dd");
        
        for (int i=0; i < this->data.length(); i++)
        {
            QMap<QString,QVariant> line = this->data.at(i);
            
            qlonglong rowid = line["rowid"].toLongLong();
            
            QList<QMap<QString,QVariant>> list = this->db->selectDonationsForPerson(rowid, date_from, date_to);
            
            generator(
                    line["donorid"].toString(),
                    line["name"].toString(),
                    line["address"].toString(),
                    line["geo"].toString(),
                    line["amount_sum"].toString(),
                    line["amount_sum_string"].toString(),
                    date_from,
                    date_to,
                    list);
        }
        
        QMessageBox *msgbox = new QMessageBox(this);
        msgbox->setWindowTitle("Done");
        msgbox->setText("Generate LaTeX Files Completed");
        msgbox->open();
    }
}
void DonationsReceiptGenerator::generator(QString donorid, QString name, QString address, QString location, QString amount_sum, QString amount_string, QString date_from, QString date_to, QList<QMap<QString, QVariant> > list_details)
{
    QString filename = name.toLower().replace(" ", "_").replace(".", "_").append(".tex");
    qDebug() << filename;
    
    QStringList infile = openInputTeXFile();
    
    QFile file(this->line_tex_output->text() + "/" + filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.errorString();
    }
    else
    {
        QTextStream out(&file);
        
        QStringList loop;
        bool in_loop = false;
        
        for (int i=0; i < infile.length(); i++)
        {
            QString line = infile.at(i);
            
            line.replace("<NAME>", name);
            line.replace("<ADDRESS>", address);
            line.replace("<LOCATION>", location);
            line.replace("<SUM>", amount_sum);
            line.replace("<SUMSTRING>", amount_string);
            line.replace("<DATEFROM>", date_from);
            line.replace("<DATETO>", date_to);
            line.replace("<DONORID>", donorid);
            
            if (line.startsWith("%<LOOP_BEGIN>"))
            {
                in_loop = true;
            }
            else if (line.startsWith("%<LOOP_END>"))
            {
                in_loop = false;
                
                for (int l=0; l < list_details.length(); l++)
                {
                    QStringList loop_current = loop;
                    for (int o=0; o < loop_current.length(); o++)
                    {
                        loop_current[o].replace("<CURRENTDATE>", list_details[l]["date"].toString());
                        loop_current[o].replace("<CURRENTAMMOUNT>", list_details[l]["amount"].toString());
                        out << loop_current[o];
                    }
                }
            }
            else
            {
                if (in_loop)
                {
                    loop.append(line);
                }
                else
                {
                    out << line;
                }
            }
        }
    }
}
QStringList DonationsReceiptGenerator::openInputTeXFile()
{
    QStringList result;
    
    QFile file(this->line_tex_input->text());
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
    }
    else
    {
        while (! file.atEnd())
        {
            QByteArray line_input = file.readLine();
            result.append(QString(line_input));
        }
    }
    
    return result;
}



QString DonationsReceiptGenerator::numberToStringGerman(QString number)
{
    for (int i=0; i < number.size(); i++)
    {
        if (! number.at(i).isDigit())
        {
            return "Zeichenkette enthält unzulässige Zeichen";
        }
    }
    
    number.remove( QRegExp("^0+") ); // Remove any number of leading 0's
    if (number.length() == 0)
    {
        return "";
    }
    else if (number.length() > 6)
    {
        return "mehr als " + numberToStringGerman("999999");
    }
    
    QMap<QString, QString> einer;
    einer.insert("1", "ein");
    einer.insert("2", "zwei");
    einer.insert("3", "drei");
    einer.insert("4", "vier");
    einer.insert("5", "fünf");
    einer.insert("6", "sechs");
    einer.insert("7", "sieben");
    einer.insert("8", "acht");
    einer.insert("9", "neun");
    
    QMap<QString, QString> zehner;
    zehner.insert("10", "zehn");
    zehner.insert("20", "zwanzig");
    zehner.insert("30", "dreißig");
    zehner.insert("40", "vierzig");
    zehner.insert("50", "fünfzig");
    zehner.insert("60", "sechzig");
    zehner.insert("70", "siebzig");
    zehner.insert("80", "achtzig");
    zehner.insert("90", "neunzig");
    
    if (number.length() == 1)
    {
        return einer[number];
    }
    else if (number.length() == 2)
    {
        if (number[1] == "0")
        {
            return zehner[number];
        }
        else if (number[0] == "1")
        {
            if (number[1] == "1")
            {
                return "elf";
            }
            else if (number[1] == "2")
            {
                return "zwölf";
            }
            else
            {
                QString result = einer[number.at(1)];
                result.append(zehner[number[0] + "0"]);
                return result;
            }
        }
        else
        {
            QString result = einer[number.at(1)];
            result.append("und");
            result.append(zehner[number[0] + "0"]);
            return result;
        }
    }
    else if (number.length() == 3)
    {
        QString removed = number;
        if (removed.remove(0, 1) == "00")
        {
            QString result = einer[number.at(0)];
            result.append("hundert");
            return result;
        }
        else
        {
            QString result = einer[number.at(0)];
            result.append("hundert");
            result.append(numberToStringGerman(number.remove(0, 1)));
            return result;
        }
    }
    else if (number.length() == 4)
    {
        QString removed = number;
        if (removed.remove(0, 1) == "000")
        {
            QString result = einer[number.at(0)];
            result.append("tausend");
            return result;
        }
        else
        {
            QString result = einer[number.at(0)];
            result.append("tausend");
            result.append(numberToStringGerman(number.remove(0, 1)));
            return result;
        }
    }
    else if (number.length() == 5)
    {
        QString removed = number;
        if (removed.remove(0, 1) == "0000")
        {
            QString result = zehner[number[0] + "0"];
            result.append("tausend");
            return result;
        }
        else
        {
            QString num = number.at(0);
            num.append(number[1]);
            if (num == "11")
            {
                QString result = "elftausend";
                result.append(numberToStringGerman(number.remove(0, 2)));
                return result;
            }
            else if (num == "12")
            {
                QString result = "zwölftausend";
                result.append(numberToStringGerman(number.remove(0, 2)));
                return result;
            }
            else
            {
                QString result = einer[number.at(1)];
                result.append(zehner[number[0] + "0"]);
                result.append("tausend");
                result.append(numberToStringGerman(number.remove(0, 2)));
                return result;
            }
        }
    }
    else if (number.length() == 6)
    {
        QString removed = number;
        QString result = numberToStringGerman(removed.remove(3, 3));
        result.append("tausend");
        result.append(numberToStringGerman(number.remove(0, 3)));
        return result;
    }
}
