// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/residue.h>
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
	if (atom->countBonds() == 0)
	{
		((ModularWidget*)parent)->setStatusbarText("Atom has no bonds");
		return;
	}
	parent_ = parent;
	String text;
	if (atom_->getParent() != 0 &&
			RTTI::isKindOf<Residue>(*atom_->getParent()))
	{
		text = ((Residue*) atom_->getParent())->getID();
	}
	text += atom_->getFullName();
	atom_edit->setText(text.c_str()); 
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

	String text;
	Atom* partner = bond->getPartner(*atom_);
	if (partner->getParent() != 0 &&
			RTTI::isKindOf<Residue>(*partner->getParent()))
	{
		text = ((Residue*) partner->getParent())->getID();
	}
	text += partner->getFullName();

	partner_edit->setText(text.c_str());
	Vector3 v = atom_->getPosition() - bond->getPartner(*atom_)->getPosition();
	length_edit->setText((String(v.getLength()) + " A").c_str());

	switch(bond->getOrder())
	{
		case Bond::ORDER__UNKNOWN:
			text = "Unknown";
			break;
		case Bond::ORDER__SINGLE:
			text = "Single";
			break;
		case Bond::ORDER__DOUBLE:
			text = "Double";
			break;
		case Bond::ORDER__TRIPLE:
			text = "Triple";
			break;
		case Bond::ORDER__QUADRUPLE: 
			text = "Quadruple";
			break;
		case Bond::ORDER__AROMATIC: 
			text = "Aromatic";
			break;
		case Bond::ORDER__ANY:
			text = "Any";
			break;
	}
	order_edit->setText(text.c_str());

	switch (bond->getType())
	{
		case Bond::TYPE__UNKNOWN:
			text = "Unknown";
			break;
		case Bond::TYPE__COVALENT:
			text = "Covalent";
			break;
		case Bond::TYPE__HYDROGEN:
			text = "Hydrogen";
			break;
		case Bond::TYPE__DISULPHIDE_BRIDGE: 
			text = "Disulphide Bridge";
			break;
		case Bond::TYPE__SALT_BRIDGE:
			text = "Salt Bridge";
			break;
		case Bond::TYPE__PEPTIDE:
			text = "Peptide";
			break;
	}
	type_edit->setText(text.c_str());
}

void BondProperties::focusAtom()
{
	if (!RTTI::isKindOf<MolecularControl>(*parent_) ||
			bond_box->currentItem() == -1) 
	{
		return;
	}
	MolecularControl& mc = *(MolecularControl*) parent_;
	mc.context_composite_ = atom_;
	mc.selectedComposite_(atom_, !atom_->isSelected());
	mc.centerCamera();
}

void BondProperties::focusPartner()
{
	if (!RTTI::isKindOf<MolecularControl>(*parent_) ||
			bond_box->currentItem() == -1) 
	{
		return;
	}
	MolecularControl& mc = *(MolecularControl*) parent_;
	Atom* partner = atom_->getBond(bond_box->currentItem())->getPartner(*atom_);
	mc.context_composite_ = partner;
	mc.selectedComposite_(partner, !partner->isSelected());
	mc.centerCamera();
}

	}
}
