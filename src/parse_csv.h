#ifndef PARSECSV_H
#define PARSECSV_H

#include <QObject>
#include <QFile>
#include <QIODevice>

#include <QDebug>

class ParseCSV : public QObject
{
    Q_OBJECT
public:
    explicit ParseCSV(QObject *parent = nullptr);
    
    QList<QMap<QString,QString>> processCSVFile(QString importfile_path);
    QList<QMap<QString,QString>> processCSVStringList(QStringList csv);
    QStringList processCSVLine(QString line_input);
    
signals:
    
};

#endif // PARSECSV_H
