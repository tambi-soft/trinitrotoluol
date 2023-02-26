#include "donations_receipt_generator.h"

DonationsReceiptGenerator::DonationsReceiptGenerator(DbAdapter *db, QWidget *parent)
    : QWidget{parent}
{
    this->db = db;
    
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    
    QLineEdit *line_test = new QLineEdit();
    connect(line_test, &QLineEdit::textChanged, this, &DonationsReceiptGenerator::lineTestEdited);
    this->label_test = new QLabel();
    
    grid->addWidget(line_test, 0, 0);
    grid->addWidget(label_test, 1, 0);
}

void DonationsReceiptGenerator::lineTestEdited(QString text)
{
    QString str = numberToStringGerman(text);
    this->label_test->setText(str);
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
