/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgMoveOverlay.cpp
	Last generated: Mon Dec 4 00:54:10 2000

 *********************************************************************/

#include <BALL/MOLVIEW/GUI/DIALOGS/DlgMoveOverlay.h>

#define Inherited DlgMoveOverlayData

DlgMoveOverlay::DlgMoveOverlay
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
    setCaption( "Move Overlay" );
}

DlgMoveOverlay::~DlgMoveOverlay()
{
}

void DlgMoveOverlay::slotLeft()
{
  emit(sigMove(1));
}

void DlgMoveOverlay::slotRight()
{
  emit(sigMove(2));
}

void DlgMoveOverlay::slotOK()
{
  destroy();
}

void DlgMoveOverlay::slotUp()
{
  emit(sigMove(3));
}

void DlgMoveOverlay::slotDown()
{
  emit(sigMove(4));
}
