// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moveOverlayDialog.h,v 1.3 2004/02/26 08:41:34 anhi Exp $

#ifndef BALL_VIEW_DIALOGS_MOVEOVERLAYDIALOG_H
#define BALL_VIEW_DIALOGS_MOVEOVERLAYDIALOG_H 

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/moveOverlayDialogData.h>

/** MoveOverlayDialog
		\ingroup ViewDialogs
*/
class BALL_EXPORT MoveOverlayDialog 
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
