/****************************************************************************
** Form interface generated from reading ui file 'contourSurfaceDialog.ui'
**
** Created: Thu Sep 5 09:25:51 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef CONTOURSURFACEDIALOG_H
#define CONTOURSURFACEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/string.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

namespace BALL
{
 namespace MOLVIEW
   {

class ContourSurfaceDialog : public QDialog
{ 
    Q_OBJECT

public:
    ContourSurfaceDialog( QWidget* parent = 0, const char* name = 0);
    ~ContourSurfaceDialog();

    QLabel* TextLabel1_2_2;
    QFrame* Line1;
    QFrame* Line1_2;
    QLabel* TextLabel1;
    QLineEdit* threshold_;
    QLabel* TextLabel1_2;
    QLineEdit* Loadfile_;
    QLineEdit* Savefile_;
    QPushButton* Compute_;
    QPushButton* Cancel_;
    QPushButton* Browseload;
    QPushButton* Browsesave;
    void setSaveName(const String& filename);
    void setLoadName(const String& filename);
    const String& getSaveName() const;
    const String& getLoadName() const;
    //double getThreshold() const;

public slots:
    virtual void new_slot();

 protected slots: 
     virtual void browseLoadFiles(); 
     virtual void browseSaveFiles();

};
}
}

#endif // CONTOURSURFACEDIALOG_H
