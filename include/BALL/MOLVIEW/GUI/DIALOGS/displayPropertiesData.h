/****************************************************************************
** Form interface generated from reading ui file 'displayProperties.ui'
**
** Created: Sun Nov 17 18:25:49 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DLGDISPLAYPROPERTIES_H
#define DLGDISPLAYPROPERTIES_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QPushButton;

class DisplayPropertiesData : public QDialog
{ 
    Q_OBJECT

public:
    DisplayPropertiesData( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DisplayPropertiesData();

    QGroupBox* GroupBox_9_3;
    QLabel* color_sample_selection;
    QPushButton* edit_selection;
    QGroupBox* model_groupbox;
    QLabel* model_type_label;
    QLabel* model_resolution_label;
    QComboBox* model_type_combobox_static;
    QComboBox* mode_resolution_combobox_static;
    QGroupBox* model_groupbox_2;
    QLabel* model_resolution_label_2;
    QLabel* model_type_label_2;
    QComboBox* mode_resolution_combobox_dynamic;
    QComboBox* model_type_combobox_dynamic;
    QPushButton* close_button;
    QPushButton* apply_button;
    QGroupBox* color_groupbox;
    QGroupBox* static_custom;
    QPushButton* edit_button;
    QLabel* color_sample;
    QComboBox* coloring_type_combobox;


public slots:
    virtual void applyButtonClicked() {};
    virtual void editColor() {};
    virtual void selectColoringMethod( const QString & string ) {};
    virtual void selectPrecisionStatic( const QString & string ) {};
    virtual void selectModelStatic( const QString & string ) {};
    virtual void editSelectionColor() {};
    virtual void select() {};
    virtual void selectModelDynamic( const QString & ) {}; 
    virtual void selectPrecisionDynamic( const QString & ) {} ; 

};

#endif // DLGDISPLAYPROPERTIES_H
