// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>

namespace BALL
{
	namespace VIEW
	{

BondProperties::BondProperties( Atom* atom, QWidget* parent,  const char* name, bool modal, WFlags fl )
  : BondPropertiesData( parent, name, modal, fl ),
		atom_(atom)
{
	atom_edit->setText(atom_->getFullName().c_str()); 
	bond_box->clear();
	for (Position pos = 0; pos < atom_->countBonds(); pos++)
	{
		bond_box->insertItem((String(pos + 1) + String(" . Bond")).c_str());
	}
	bondSelected();
}

BondProperties::~BondProperties()
{
  // no need to delete child widgets, Qt does it all for us
}

void BondProperties::bondSelected()
{
	if (bond_box->currentItem() == -1)
	{
		name_edit->clear();
		partner_edit->clear();
		order_edit->clear();
		type_edit->clear();
		length_edit->clear();
		return;
	}

	Bond* bond = atom_->getBond(bond_box->currentItem());
	name_edit->setText(bond->getName().c_str());
	partner_edit->setText(bond->getPartner(*atom_)->getFullName().c_str());
	Vector3 v = atom_->getPosition() - bond->getPartner(*atom_)->getPosition();
	length_edit->setText(String(v.getLength()).c_str());

	String text = "";
	switch(bond->getOrder())
	{
		case Bond::ORDER__UNKNOWN:
			text = "Unknown";
		case Bond::ORDER__SINGLE:
			text = "Single";
		case Bond::ORDER__DOUBLE:
			text = "Double";
		case Bond::ORDER__TRIPLE:
			text = "Triple";
		case Bond::ORDER__QUADRUPLE: 
			text = "Quadruple";
		case Bond::ORDER__AROMATIC: 
			text = "Aromatic";
		case Bond::ORDER__ANY:
			text = "Any";
	}
	order_edit->setText(text.c_str());

	switch (bond->getType())
	{
		case Bond::TYPE__UNKNOWN:
			text = "Unknown";
		case Bond::TYPE__COVALENT:
			text = "Covalent";
		case Bond::TYPE__HYDROGEN:
			text = "Hydrogen";
		case Bond::TYPE__DISULPHIDE_BRIDGE: 
			text = "Disulphide Bridge";
		case Bond::TYPE__SALT_BRIDGE:
			text = "Salt Bridge";
		case Bond::TYPE__PEPTIDE:
			text = "Peptide";
	}
	type_edit->setText(text.c_str());
}

	}
}
