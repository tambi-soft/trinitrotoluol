#ifndef DONATIONSMAPEDIT_H
#define DONATIONSMAPEDIT_H


#include <QMessageBox>
#include <QDialog>

#include "lib/grid_widget.h"
#include "people_selector.h"
#include "lib/growing_text_edit.h"

class DonationsMapEdit : public GridWidget
{
    Q_OBJECT
public:
    explicit DonationsMapEdit(DbAdapter *db, GridWidget *parent = nullptr);
    
    void showData();
    
protected:
    void hideEvent(QHideEvent */**event**/);
    void showEvent(QShowEvent */**event**/);
    
private:
    DbAdapter *db;
    QDialog *dialog_select_person = new QDialog;
    QString tnt_name;
    
    GrowingTextEdit *help;
    
private slots:
    void onDeleteButtonClicked(qlonglong rowid_person, QString name);
    void onEditButtonClicked(QString tnt_name);
    void onPersonSelected(qlonglong rowid, QString name);
};

#endif // DONATIONSMAPEDIT_H
