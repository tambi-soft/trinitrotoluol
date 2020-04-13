#include "parse_csv.h"

ParseCSV::ParseCSV(QObject *parent) : QObject(parent)
{
    
}

QList<QMap<QString,QString>> ParseCSV::processCSVFile(QString importfile_path)
{
    QList<QMap<QString,QString>> data;
    QStringList headers;
    
    QFile file(importfile_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }
    else
    {
        int x = -1;
        while (!file.atEnd())
        {
            x++;
            
            QByteArray line_input_b = file.readLine();
            QString line_input = QString(line_input_b);
            
            QStringList splitted = processCSVLine(line_input);
            //QStringList splitted = line_input.split(',');
            
            if (x == 0)
            {
                headers = splitted;
            }
            else
            {
                QMap<QString,QString> line;
                for (int y=0; y < splitted.length(); y++)
                {
                    line[headers.at(y)] = splitted.at(y);
                }
                data.append(line);
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
        if (line_input.at(i) == "\"")
        {
            // flip the boolean
            in_quotation ^= true;
            
            if (! in_quotation)
            {
                // we have just flipped from true to false
                //if (line.length() > 0)
                //{
                    splitted.append(line);
                    //qDebug() << line;
                //}
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
        /*
        else
        {
            if (line_input.at(i) == ",")
            {
                if (line.length() > 0)
                {
                    // this seems never to happen
                    splitted.append(line);
                    //qDebug() << line;
                }
                line = "";
            }
        }
        */
    }
    /*
    if (line.length() > 0)
    {
        // this seems never to happen
        splitted.append(line);
        //qDebug() << line;
    }
    */
    qDebug() << splitted;
    return splitted;
}
