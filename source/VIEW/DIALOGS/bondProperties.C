// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/residue.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

namespace BALL
{
	namespace VIEW
	{

BondProperties::BondProperties( Atom* atom, QWidget* parent,  const char* name, bool, Qt::WindowFlags fl )
  : QDialog(parent, fl),
		Ui_BondPropertiesData(),
		atom_(atom)
{
	setupUi(this);

	setObjectName(name);

	if (atom->countBonds() == 0)
	{
		((ModularWidget*)parent)->setStatusbarText((String)tr("Atom has no bonds"));
		return;
	}
	parent_ = parent;
	String text;
	if (atom_->getParent() != 0 &&
            RTTI::isKindOf<Residue>(atom_->getParent()))
	{
		text = ((Residue*) atom_->getParent())->getID();
	}
	text += atom_->getFullName();
	atom_edit->setText(text.c_str()); 
	bond_box->clear();
	for (Position pos = 0; pos < atom_->countBonds(); pos++)
	{
		bond_box->addItem((String(pos + 1) + String(" . Bond")).c_str());
	}
	bondSelected();
}

BondProperties::~BondProperties()
{
  // no need to delete child widgets, Qt does it all for us
}

void BondProperties::bondSelected()
{
	if (bond_box->currentIndex() == -1)
	{
		name_edit->clear();
		partner_edit->clear();
		order_edit->clear();
		type_edit->clear();
		length_edit->clear();
		return;
	}

	Bond* bond = atom_->getBond(bond_box->currentIndex());
	name_edit->setText(bond->getName().c_str());

	String text;
	Atom* partner = bond->getPartner(*atom_);
	if (partner->getParent() != 0 &&
            RTTI::isKindOf<Residue>(partner->getParent()))
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
			text = (String)tr("Unknown");
			break;
		case Bond::ORDER__SINGLE:
			text = (String)tr("Single");
			break;
		case Bond::ORDER__DOUBLE:
			text = (String)tr("Double");
			break;
		case Bond::ORDER__TRIPLE:
			text = (String)tr("Triple");
			break;
		case Bond::ORDER__QUADRUPLE: 
			text = (String)tr("Quadruple");
			break;
		case Bond::ORDER__AROMATIC: 
			text = (String)tr("Aromatic");
			break;
		case Bond::ORDER__ANY:
			text = (String)tr("Any");
			break;
	}
	order_edit->setText(text.c_str());

	switch (bond->getType())
	{
		case Bond::TYPE__UNKNOWN:
			text = (String)tr("Unknown");
			break;
		case Bond::TYPE__COVALENT:
			text = (String)tr("Covalent");
			break;
		case Bond::TYPE__HYDROGEN:
			text = (String)tr("Hydrogen");
			break;
		case Bond::TYPE__DISULPHIDE_BRIDGE: 
			text = (String)tr("Disulphide Bridge");
			break;
		case Bond::TYPE__SALT_BRIDGE:
			text = (String)tr("Salt Bridge");
			break;
		case Bond::TYPE__PEPTIDE:
			text = (String)tr("Peptide");
			break;
	}
	type_edit->setText(text.c_str());
}

void BondProperties::focusAtom()
{
    if (!RTTI::isKindOf<MolecularControl>(parent_) ||
			bond_box->currentIndex() == -1) 
	{
		return;
	}

	MolecularControl& mc = *(MolecularControl*) parent_;
	list<Composite*> list;
	list.push_back(atom_);
	mc.highlight(list);
	if (atom_->isSelected()) mc.deselect();
	else 									   mc.select();
	mc.context_composite_ = atom_;
	mc.centerCamera();
}

void BondProperties::focusPartner()
{
    if (!RTTI::isKindOf<MolecularControl>(parent_) ||
			bond_box->currentIndex() == -1) 
	{
		return;
	}

	Atom* partner = atom_->getBond(bond_box->currentIndex())->getPartner(*atom_);
	MolecularControl& mc = *(MolecularControl*) parent_;
	list<Composite*> list;
	list.push_back(partner);
	mc.highlight(list);
	if (partner->isSelected()) mc.deselect();
	else 									     mc.select();
	mc.context_composite_ = partner;
	mc.centerCamera();
}

	}
}
