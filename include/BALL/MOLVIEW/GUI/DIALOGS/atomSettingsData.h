/****************************************************************************
** Form interface generated from reading ui file 'atomsettings.ui'
**
** Created: Thu Nov 28 20:38:28 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ATOMSETTINGSDATA_H
#define ATOMSETTINGSDATA_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

class AtomSettingsData : public QDialog
{ 
    Q_OBJECT

public:
    AtomSettingsData( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AtomSettingsData();

    QPushButton* apply_button;
    QPushButton* close_button;
    QFrame* Frame6;
    QLabel* TextLabel2;
    QLineEdit* name_edit;
    QFrame* Frame6_2;
    QLabel* TextLabel2_2;
    QLineEdit* charge_edit;
    QFrame* Frame6_3;
    QLabel* TextLabel2_3;
    QLineEdit* radius_edit;
    QFrame* Frame3_3;
    QLabel* TextLabel5_3;
    QLineEdit* position_edit_1;
    QLineEdit* position_edit_2;
    QLineEdit* position_edit_3;
    QFrame* Frame3_2;
    QLabel* TextLabel5_2;
    QLineEdit* force_edit_1;
    QLineEdit* force_edit_2;
    QLineEdit* force_edit_3;
    QFrame* Frame3;
    QLabel* TextLabel5;
    QLineEdit* velocity_edit_1;
    QLineEdit* velocity_edit_2;
    QLineEdit* velocity_edit_3;


public slots:
    virtual void changed( const QString & );
    virtual void applyClicked();
    virtual void closeClicked();

protected:
    QHBoxLayout* Layout12;
};

#endif // ATOMSETTINGSDATA_H
