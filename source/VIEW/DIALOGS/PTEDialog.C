// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PTEDialog.C,v 1.1 2005/02/07 13:21:21 anne Exp $

#include <BALL/VIEW/DIALOGS/PTEDialog.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <qtooltip.h>
#include <qbuttongroup.h>

namespace BALL
{
	namespace VIEW
	{
		PTEDialog::PTEDialog(QWidget* parent, const char* name, WFlags fl)
			: PTEDialogData(parent, name, fl),
				ModularWidget(name)
		{
			registerWidget(this);
			
			// iterate over all buttons in the button group
			int i=1;
			int highest_id = Element::NUMBER_OF_ELEMENTS; // the maximal id in the button group
			// we can't use count here, since the id's in the group are
			// not contingent, since we use the atomic number as index
			// and since the side groups (???) are left out
			for (;i<=highest_id;i++)
			{
				if (buttonGroup1->find(i) != (QButton*)-1)
				{
					QToolTip::add((QWidget*)buttonGroup1->find(i), atomProperties_(i));
				}
			}
		}

		PTEDialog::~PTEDialog()
			throw()
		{
		}

// ------------------------- SLOTS ------------------------------------------------
// --------------------------------------------------------------------------------
	 void PTEDialog::newElementType(int elementNumber)
	 {
		 EditableScene* scene = EditableScene::getInstance(0);

		 if (scene == 0)
		 {
			 Log.error() << "Expected an EditableScene, but found none!" << std::endl;
		 }
		 else
		 {
			 scene->setEditElementType(elementNumber);
		 }
	 }

	 void PTEDialog::onNotify(Message *message)
		 throw()
	 {
#ifdef BALL_VIEW_DEBUG
			Log.error() << "PTEDialog " << this << " onNotify " << message << std::endl;
#endif
	 }

	 QString PTEDialog::atomProperties_(int number)
	 {
		 Element e = PTE[number];
		 QString result("Element: ");
		 result += e.getName();
		 result += "\nSymbol: ";
		 result += e.getSymbol();
		 result += "\nAtomic Weight: ";
		 result += String(e.getAtomicWeight()).c_str();
		 result += "\nAtomic Radius: ";
		 result += String(e.getAtomicRadius()).c_str();
		 result += "\nCovalent Radius: ";
		 result += String(e.getCovalentRadius()).c_str();
		 result += "\nVan der Waals Radius: ";
		 result += String(e.getVanDerWaalsRadius()).c_str();
		 result += "\nElectronegativity: ";
		 result += String(e.getElectronegativity()).c_str();
		 result += "\n"; 
		 
		 return result;
	 }
	}
}
