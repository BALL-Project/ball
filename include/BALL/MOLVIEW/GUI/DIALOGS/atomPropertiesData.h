/****************************************************************************
** Form interface generated from reading ui file 'atomProperties.ui'
**
** Created: Tue Dec 10 17:33:58 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ATOMPROPERTIESDATA_H
#define ATOMPROPERTIESDATA_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;

class AtomPropertiesData : public QDialog
{ 
    Q_OBJECT

public:
    AtomPropertiesData( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AtomPropertiesData();

    QLabel* TextLabel1;
    QLabel* TextLabel4;
    QLabel* TextLabel8;
    QLabel* TextLabel3;
    QPushButton* apply_button;
    QPushButton* close_button;
    QLabel* TextLabel6;
    QLabel* TextLabel5;
    QLabel* TextLabel7;
    QLabel* TextLabel2;
    QLabel* TextLabel2_2;
    QLineEdit* name_edit;
    QLineEdit* type_name_edit;
    QLineEdit* type_edit;
    QComboBox* element_box;
    QLineEdit* radius_edit;
    QLineEdit* charge_edit;
    QLineEdit* position_edit;
    QLineEdit* velocity_edit;
    QLineEdit* force_edit;

public slots:
    virtual void applyClicked();
    virtual void closeClicked();
    virtual void changed();

};

#endif // ATOMPROPERTIESDATA_H
