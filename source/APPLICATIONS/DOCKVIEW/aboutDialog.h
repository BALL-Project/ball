/****************************************************************************
** Form interface generated from reading ui file 'aboutDialog.ui'
**
** Created: Thu Jan 6 16:25:30 2005
**      by: The User Interface Compiler ($Id: aboutDialog.h,v 1.1.2.2 2005/01/06 15:27:55 leonhardt Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AboutDialog();

    QLabel* PixmapLabel1;
    QLabel* TextLabel1;
    QPushButton* PushButton1;
    QLabel* qt_version_label;
    QLabel* BALL_version_label;
    QLabel* BALLView_version_label;

protected:

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // ABOUTDIALOG_H
