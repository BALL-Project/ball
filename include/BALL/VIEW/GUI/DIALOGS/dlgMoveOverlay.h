// $Id: dlgMoveOverlay.h,v 1.1.2.1 2002/10/21 09:22:36 amoll Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H
#define BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H 

#include "dlgMoveOverlayData.h"

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
