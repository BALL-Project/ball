// $Id: DlgMoveOverlay.h,v 1.3 2001/06/07 01:30:19 amoll Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H
#define BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H 

#include "DlgMoveOverlayData.h"

class DlgMoveOverlay 
	: public DlgMoveOverlayData
{
  Q_OBJECT

	public:

    DlgMoveOverlay(QWidget *parent = NULL, const char *name = NULL );

    virtual ~DlgMoveOverlay();

    protected slots:

    virtual void slotLeft();
		
    virtual void slotRight();
		
    virtual void slotOK();
    
		virtual void slotUp();
    
		virtual void slotDown();

 signals:
    void sigMove(int i);
};

#endif //BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H
