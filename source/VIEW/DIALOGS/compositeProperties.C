// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qgroupbox.h>

namespace BALL
{
	namespace VIEW
	{

CompositeProperties::CompositeProperties(Composite* composite, QWidget* parent, 
																				 const char* name, bool modal, WFlags fl )
    : CompositePropertiesData( parent, name, modal, fl ),
			composite_(composite)
{
	if (RTTI::isKindOf<AtomContainer>(*composite))
	{
		name_edit->setText(((AtomContainer*)composite)->getName().c_str());

		if (RTTI::isKindOf<Residue>(*composite))
		{
			id_edit->setText(((Residue*)composite)->getID().c_str());
		}
	}

	if (!RTTI::isKindOf<Residue>(*composite))
	{
		residues_box->setEnabled(false);
	}

	if (!RTTI::isKindOf<Atom>(*composite))
	{
		atoms_box->setEnabled(false);
		return;
	}

	Atom* atom = (Atom*)composite;
	name_edit->setText(((Atom*)composite)->getName().c_str());
	
	type_edit->setText(String(atom->getType()).c_str());
	type_name_edit->setText(atom->getTypeName().c_str());
	charge_edit->setText(String(atom->getCharge()).c_str());
	radius_edit->setText(String(atom->getRadius()).c_str());
	position_edit->setText((String("(") + 
													getString_(atom->getPosition().x) + String("|") +
												 	getString_(atom->getPosition().y) + String("|") +
												 	getString_(atom->getPosition().z) + String(")")).c_str());

	force_edit->setText((String("(") + 
											 getString_(atom->getForce().x * pow(10., 12)) + String("|") +
											 getString_(atom->getForce().y * pow(10., 12)) + String("|") +
											 getString_(atom->getForce().z * pow(10., 12)) + String(")")).c_str());

	velocity_edit->setText((String("(") + 
													getString_(atom->getVelocity().x) + String("|") +
													getString_(atom->getVelocity().y) + String("|") +
													getString_(atom->getVelocity().z) + String(")")).c_str());

	Element element;
	for (Position nr = 0; ; nr++) 
	{
		element = PTE.getElement(nr);
		if (element == Element::UNKNOWN) break;

		element_box->insertItem(element.getSymbol().c_str());
	}

	element_box->setCurrentText(atom->getElement().getSymbol().c_str());
}

String CompositeProperties::getString_(float data) const
	throw()
{
	String result(data);
	result.trimRight("0");
	if (result.hasSuffix(".")) result.trimRight(".");
	return result;
}

CompositeProperties::~CompositeProperties()
{
  // no need to delete child widgets, Qt does it all for us
}

void CompositeProperties::accept()
{
	try
	{
		if (RTTI::isKindOf<Atom>(*composite_))
		{
			Atom* atom = (Atom*) composite_;
			atom->setName(String(name_edit->text().latin1()));
			atom->setType(String(type_edit->text().latin1()).toShort());
			atom->setTypeName(String(type_name_edit->text().latin1()));
			atom->setCharge(String(charge_edit->text().latin1()).toFloat());
			atom->setRadius(String(radius_edit->text().latin1()).toFloat());
			atom->setElement(PTE[(String(element_box->currentText().latin1()))]);
		}
	}
	catch(Exception::InvalidFormat)
	{
		Log.error() << "Invalid value, try a floating point number!" << std::endl;
		return;
	}

	if (RTTI::isKindOf<AtomContainer>(*composite_))
	{
		((AtomContainer*)composite_)->setName(name_edit->text().latin1());
	}

	if (RTTI::isKindOf<Residue>(*composite_))
	{
		Residue* residue = (Residue*) composite_;
		residue->setID(id_edit->text().latin1());
	}
	
	Log.info() << "Values applied." << std::endl;
	close();

	// Sending of messages to update Scene is done in MolecularControl
}

	}
}
