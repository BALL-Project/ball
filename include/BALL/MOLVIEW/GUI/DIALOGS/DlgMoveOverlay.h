// $Id: DlgMoveOverlay.h,v 1.2 2001/01/24 12:32:03 amoll Exp $
/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgMoveOverlay.h
	Last generated: Mon Dec 4 00:54:10 2000

 *********************************************************************/

#ifndef DlgMoveOverlay_included
#define DlgMoveOverlay_included

#include "DlgMoveOverlayData.h"


class DlgMoveOverlay : public DlgMoveOverlayData
{
    Q_OBJECT

public:

    DlgMoveOverlay(
        QWidget *parent = NULL,
        const char *name = NULL );

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
#endif // DlgMoveOverlay_included
