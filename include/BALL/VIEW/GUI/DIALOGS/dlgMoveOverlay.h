// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dlgMoveOverlay.h,v 1.4 2003/03/26 13:08:49 sturm Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H
#define BALL_VIEW_GUI_DIALOGS_DLGMOVEOVERLAY_H 

#ifndef DlgMoveOverlayData_included
# include "dlgMoveOverlayData.h"
#endif

/** DlgMoveOverlayData
\ingroup ViewGuiDialogs
*/

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
