#ifndef DONATIONSIMPORTMATRIX_H
#define DONATIONSIMPORTMATRIX_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QPushButton>

#include <QFile>

#include <QDebug>

class DonationsImportMatrix : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImportMatrix(QString importfile_path, QWidget *parent = nullptr);
    
private:
    QGridLayout *layout = new QGridLayout;
    
    void processCSVFile(QString importfile_path);
    QStringList processCSVLine(QString line_input);
    
signals:
    
};

#endif // DONATIONSIMPORTMATRIX_H
