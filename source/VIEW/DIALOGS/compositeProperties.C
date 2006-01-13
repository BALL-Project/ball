// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <qlineedit.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

CompositeProperties::CompositeProperties(Composite* composite, QWidget* parent, 
																				 const char* name, bool, Qt::WFlags fl )
    : QDialog(parent, fl),
			Ui_CompositePropertiesData(),
			composite_(composite)
{
	setupUi(this);
	
  // signals and slots connections
  connect( ok_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( cancel_button, SIGNAL( clicked() ), this, SLOT( close() ) );

	setObjectName(name);
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

		element_box->addItem(element.getSymbol().c_str());
	}

	element_box->setCurrentIndex(atom->getElement().getAtomicNumber() - 1);
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
			atom->setName(ascii(name_edit->text()));
			atom->setType(ascii(type_edit->text()).toShort());
			atom->setTypeName(ascii(type_name_edit->text()));
			atom->setCharge(ascii(charge_edit->text()).toFloat());
			atom->setRadius(ascii(radius_edit->text()).toFloat());
			atom->setElement(PTE[(ascii(element_box->currentText()))]);
		}
	}
	catch(Exception::InvalidFormat)
	{
		Log.error() << "Invalid value, try a floating point number!" << std::endl;
		return;
	}

	if (RTTI::isKindOf<AtomContainer>(*composite_))
	{
		((AtomContainer*)composite_)->setName(ascii(name_edit->text()));
	}

	if (RTTI::isKindOf<Residue>(*composite_))
	{
		Residue* residue = (Residue*) composite_;
		residue->setID(ascii(id_edit->text()));
	}
	
	Log.info() << "Values applied." << std::endl;
	close();

	// Sending of messages to update Scene is done in MolecularControl
}

	}
}
