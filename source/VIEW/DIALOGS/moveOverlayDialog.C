// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moveOverlayDialog.C,v 1.1 2003/08/28 16:43:12 amoll Exp $

#include <BALL/VIEW/DIALOGS/moveOverlayDialog.h>

MoveOverlayDialog::MoveOverlayDialog(QWidget* parent, const char* name)
	: MoveOverlayDialogData( parent, name )
{
}

MoveOverlayDialog::~MoveOverlayDialog()
{
}

void MoveOverlayDialog::slotLeft()
{
  emit(sigMove(1));
}

void MoveOverlayDialog::slotRight()
{
  emit(sigMove(2));
}

void MoveOverlayDialog::slotOK()
{
  destroy();
}

void MoveOverlayDialog::slotUp()
{
  emit(sigMove(3));
}

void MoveOverlayDialog::slotDown()
{
  emit(sigMove(4));
}
