#include "db_access.h"

DbAccess::DbAccess(QObject *parent) : QObject(parent)
{
    
}


// maps the selected field name to the fetched value
QList<QMap<QString,QVariant>> DbAccess::dbIteratorToMapList(QSqlQuery query)
{
    QList<QMap<QString,QVariant>> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        QMap<QString,QVariant> map;
        for (int i = 0; i <= rec.count(); ++i)
        {
            QString name = rec.fieldName(i);
            QString value = query.value(i).toString();
            
            map[name] = value;
        }
        
        result.append(map);
    }
    
    return result;
}

QMap<QString,QVariant> DbAccess::dbIteratorToMap(QSqlQuery query)
{
    QMap<QString,QVariant> result;
    
    QSqlRecord rec = query.record();
    while (query.next())
    {
        //result[ query.value(0).toString() ] = query.value(1).toString();
        for (int i=0; i <= rec.count(); ++i)
        {
            QString name = rec.fieldName(i);
            QVariant value = query.value(i);
            
            result[name] = value;
        }
    }
    
    return result;
}
