#include "parse_csv.h"

ParseCSV::ParseCSV(QObject *parent) : QObject(parent)
{
    
}

QList<QMap<QString,QString>> ParseCSV::processCSVFile(QString importfile_path)
{
    QList<QMap<QString,QString>> data;
    QStringList headers;
    QStringList line_last;
    
    QFile file(importfile_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }
    else
    {
        QString line_last;
        bool line_with_linebreak = false;
        int x = -1;
        while (! file.atEnd())
        {
            x++;
            
            QByteArray line_input_b = file.readLine();
            QString line_input = QString(line_input_b);
            
            QStringList splitted = processCSVLine(line_input);
            
            if (line_with_linebreak)
            {
                splitted = processCSVLine(line_last.replace("\n", "") + line_input);
            }
            
            if (x == 0)
            {
                headers = splitted;
            }
            else
            {
                if (splitted.length() == headers.length())
                {
                    // everything (hopefully) OK
                    QMap<QString,QString> line;
                    for (int y=0; y < splitted.length(); y++)
                    {
                        line[headers.at(y)] = splitted.at(y);
                    }
                    data.append(line);
                    
                    line_with_linebreak = false;
                }
                // problem: there can be line-breaks in the data fooling the "line-by-line" logic
                // we assume we do not have such an issue at the header, and take this length as the correct one
                else if (splitted.length() < headers.length())
                {
                    line_with_linebreak = true;
                    line_last = line_input;
                    continue;
                }
                else
                {
                    qDebug() << "ERROR: csv is really messed up!";
                    qDebug() << splitted;
                    break;
                }
            }
        }
    }
    
    return data;
}

QStringList ParseCSV::processCSVLine(QString line_input)
{
    // problem: csv-data can look like
    // "bla, blub", "aaa, bbb" 
    // so before splitting at ',' we have to deal with this situation first
    
    QStringList splitted;
    bool in_quotation = false;
    QString line;
    for (int i=0; i < line_input.length(); ++i)
    {
        // assuming we have a linebreak in midst of the payload
        if (i == 0 && line_input.at(0) != "\"")
        {
            //linebreak_in_the_middle = true;
            in_quotation = true;
        }
        
        if (line_input.at(i) == "\"")
        {
            // flip the boolean
            in_quotation ^= true;
            
            if (! in_quotation)
            {
                splitted.append(line);
                line = "";
            }
        }
        
        if (in_quotation)
        {
            if (line_input.at(i) != "\"")
            {
                line.append(line_input.at(i));
            }
        }
    }
    
    // maybe the line ends with an inner linebreak, so just to be shure, we dont miss anything
    splitted.append(line);
    //qDebug() << splitted;
    return splitted;
}
