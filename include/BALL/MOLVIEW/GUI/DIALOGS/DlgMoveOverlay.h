// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DlgMoveOverlay.h,v 1.4 2002/02/27 12:19:16 sturm Exp $

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
