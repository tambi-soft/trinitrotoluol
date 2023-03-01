#include "parse_csv.h"

ParseCSV::ParseCSV(QObject *parent) : QObject(parent)
{
    
}

QList<QMap<QString,QString>> ParseCSV::processCSVFile(QString importfile_path)
{
    QStringList data;
    
    QFile file(importfile_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
    }
    else
    {
        while (! file.atEnd())
        {
            QByteArray line_input_b = file.readLine();
            QString line_input = QString(line_input_b);
            //qDebug() << line_input.length();
            data.append(line_input);
        }
    }
    
    return processCSVStringList(data);
}

QList<QMap<QString,QString>> ParseCSV::processCSVStringList(QStringList csv)
{
    QList<QMap<QString,QString>> data;
    QStringList headers;
    
    QString line_last;
    bool line_with_linebreak = false;
    for (int i=0; i < csv.length(); i++)
    {
        QString line_input = csv.at(i);
        
        QStringList splitted = processCSVLine(line_input);
        
        //qDebug() << "#";
        //qDebug() << line_input;
        //qDebug() << splitted.length() << "|" << headers.length() << "|" << splitted;
        //qDebug() << "$";
        
        if (i == 0)
        {
            headers = splitted;
            //qDebug() << headers;
        }
        else
        {
            QMap<QString,QString> line;
            for (int y=0; y < splitted.length(); y++)
            {
                if (headers.length() > y)
                    line[headers.at(y)] = splitted.at(y);
            }
            data.append(line);
        }
        
    }
    
    return data;
}

QStringList ParseCSV::processCSVLine(QString line_input)
{
    // problem: csv-data can look like
    // "bla, blub", "aaa, bbb" 
    // so before splitting at ',' we have to deal with this situation first
    
    QStringList result;
    bool in_quotation = false;
    QString line;
    for (int i=0; i < line_input.length(); i++)
    {
        QString cur = line_input.at(i);
        
        if (! in_quotation)
        {
            if (cur == "\"")
            {
                in_quotation = true;
            }
            else if (cur == ",")
            {
                result.append(line);
                line = "";
            }
            else
            {
                line.append(cur);
            }
        }
        else
        {
            if (cur == "\"")
            {
                in_quotation = false;
            }
            else
            {
                line.append(cur);
            }
        }
    }
    //result.append(line);
    
    return result;
}
