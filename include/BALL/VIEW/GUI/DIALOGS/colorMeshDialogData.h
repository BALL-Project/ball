/****************************************************************************
** Form interface generated from reading ui file 'colormeshdialog.ui'
**
** Created: Tue Sep 24 19:06:56 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef COLORMESHDIALOGDATA_H
#define COLORMESHDIALOGDATA_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

class colorMeshDialogData : public QDialog
{ 
    Q_OBJECT

public:
    colorMeshDialogData( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~colorMeshDialogData();

    QPushButton* Compute_;
    QPushButton* Cancel_;
    QFrame* Line3;
    QLabel* TextLabel1;
    QLineEdit* Loadfile_;
    QPushButton* Browseload;
    QLabel* TextLabel2;
    QSpinBox* numberOfColors;
    QLabel* TextLabel3;
    QLabel* TextLabel3_2;
    QLineEdit* MinVal;
    QLineEdit* MaxVal;


protected:
    QHBoxLayout* Layout2;
};

#endif // COLORMESHDIALOGDATA_H
