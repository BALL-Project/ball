/****************************************************************************
** Form interface generated from reading ui file 'contourSurfaceDialog.ui'
**
** Created: Fri Nov 29 15:16:22 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef CONTOURSURFACEDIALOGDATA_H
#define CONTOURSURFACEDIALOGDATA_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTabWidget;
class QWidget;

class ContourSurfaceDialogData : public QDialog
{ 
    Q_OBJECT

public:
    ContourSurfaceDialogData( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ContourSurfaceDialogData();

    QPushButton* apply_button;
    QPushButton* cancel_button;
    QTabWidget* surface_tab;
    QWidget* by_file;
    QLabel* TextLabel1;
    QPushButton* browse_button;
    QLineEdit* location_edit;
    QWidget* by_color;
    QLabel* TextLabel5;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLabel* TextLabel10;
    QSpinBox* alpha_box;
    QSpinBox* blue_box;
    QSpinBox* green_box;
    QSpinBox* red_box;
    QPushButton* choose_button;
    QWidget* colormap_tab;
    QLabel* TextLabel7;
    QLabel* TextLabel8;
    QLabel* TextLabel9;
    QLabel* TextLabel14_3;
    QLabel* TextLabel14;
    QLabel* TextLabel14_2;
    QLabel* TextLabel15;
    QLabel* TextLabel13;
    QPushButton* min_button;
    QPushButton* mid_button;
    QPushButton* max_button;
    QPushButton* max_max_button;
    QLineEdit* max_box;
    QSpinBox* levels_box;
    QLineEdit* min_box;
    QPushButton* min_min_button;
    QLabel* gradient_label;

public slots:
    virtual void apply_clicked();
    virtual void browse_clicked();
    virtual void cancel_clicked();
    virtual void choose_clicked();
    virtual void color_boxes_changed();
    virtual void location_changed();
    virtual void max_clicked();
    virtual void mid_clicked();
    virtual void min_clicked();
    virtual void min_min_clicked();
    virtual void max_max_clicked();
    virtual void tab_changed();

};

#endif // CONTOURSURFACEDIALOGDATA_H
