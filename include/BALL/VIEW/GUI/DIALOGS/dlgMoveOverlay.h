// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dlgMoveOverlay.h,v 1.3 2002/12/18 16:00:31 sturm Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H
#define BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H 

#ifndef DlgMoveOverlayData_included
# include "dlgMoveOverlayData.h"
#endif

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
