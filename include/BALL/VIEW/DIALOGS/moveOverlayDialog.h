// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moveOverlayDialog.h,v 1.2 2003/09/04 22:17:46 amoll Exp $

#ifndef BALL_VIEW_DIALOGS_MOVEOVERLAYDIALOG_H
#define BALL_VIEW_DIALOGS_MOVEOVERLAYDIALOG_H 

#include <BALL/VIEW/UIC/moveOverlayDialogData.h>

/** MoveOverlayDialog
		\ingroup ViewDialogs
*/
class MoveOverlayDialog 
	: public MoveOverlayDialogData
{
  Q_OBJECT

	public:

    MoveOverlayDialog(QWidget *parent = NULL, const char *name = NULL );

    virtual ~MoveOverlayDialog();

    protected slots:

    virtual void slotLeft();
		
    virtual void slotRight();
		
    virtual void slotOK();
    
		virtual void slotUp();
    
		virtual void slotDown();

 signals:
    void sigMove(int i);
};

#endif //BALL_VIEW_DIALOGS_MOVEOVERLAYDIALOG_H
