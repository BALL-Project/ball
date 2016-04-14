// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/VIEW/WIDGETS/propertyEditor.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

namespace BALL
{
	namespace VIEW
	{

CompositeProperties::CompositeProperties(Composite* composite, QWidget* parent, 
																				 const char* name, bool, Qt::WindowFlags fl )
    : QDialog(parent, fl),
			Ui_CompositePropertiesData(),
			composite_(composite)
{
	setupUi(this);

	connect(buttonBox, SIGNAL(accepted()), named, SLOT(apply()));
	connect(buttonBox, SIGNAL(rejected()), named, SLOT(reset()));

	tabWidget->setTabText(0, tr("Standard"));	
	tabWidget->setTabText(1, tr("Named"));	
	named->setPropertyManager(dynamic_cast<PropertyManager*>(composite));

	setObjectName(name);
    if (RTTI::isKindOf<AtomContainer>(composite))
	{
		name_edit->setText(((AtomContainer*)composite)->getName().c_str());

        if (RTTI::isKindOf<Residue>(composite))
		{
			id_edit->setText(((Residue*)composite)->getID().c_str());
		}
	}

    if (!RTTI::isKindOf<Residue>(composite))
	{
		residues_box->setEnabled(false);
	}

    if (!RTTI::isKindOf<Atom>(composite))
	{
		atoms_box->setEnabled(false);
		return;
	}

	Atom* atom = (Atom*)composite;
	name_edit->setText(((Atom*)composite)->getName().c_str());
	
	type_edit->setText(String(atom->getType()).c_str());
	type_name_edit->setText(atom->getTypeName().c_str());
	charge_edit->setText(String(atom->getCharge()).c_str());
	formal_charge_edit->setText(String(atom->getFormalCharge()).c_str());
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

	vector<String> elements;
	Element element;

	for (Position nr = 1; ; nr++) 
	{
		element = PTE.getElement(nr);
		if (element == Element::UNKNOWN) break;

		elements.push_back(element.getSymbol());
	}

	sort(elements.begin(), elements.end());

	for (Position nr = 1; nr < elements.size(); nr++)
	{
		element_box->addItem(elements[nr].c_str());
	}

	String symb = atom->getElement().getSymbol();
	element_box->setCurrentIndex(element_box->findText(symb.c_str()));

}

String CompositeProperties::getString_(float data) const
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
        if (RTTI::isKindOf<Atom>(composite_))
		{
			Atom* atom = (Atom*) composite_;
			
			// if the atomtype has changed, we also want the atom name to be changed
			if (    (atom->getElement() != PTE[(ascii(element_box->currentText()))])
					 && (atom->getName() == ascii(name_edit->text())) 
				 )
			{
				String new_name = PTE[(ascii(element_box->currentText()))].getSymbol();
				// get the old atomnumber
				String old_name = atom->getName();
				old_name.toUpper();
				new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ ");
				atom->setName(new_name);
				//getMainControl()->update(*atom);
			}
			else
			{
				atom->setName(ascii(name_edit->text()));
			}
			atom->setType(ascii(type_edit->text()).toShort());
			atom->setTypeName(ascii(type_name_edit->text()));
			atom->setCharge(ascii(charge_edit->text()).toFloat());
			atom->setFormalCharge(ascii(formal_charge_edit->text()).toInt());
			atom->setRadius(ascii(radius_edit->text()).toFloat());
			atom->setElement(PTE[(ascii(element_box->currentText()))]);
			String text = ascii(position_edit->text());
			vector<String> fields;
			text.split(fields, "()|, ");
			if (fields.size() != 3)
			{
				Log.error() << (String)tr("Invalid values for position!") << std::endl;
				return;
			}

			atom->setPosition(Vector3(fields[0].toFloat(),
																fields[1].toFloat(),
																fields[2].toFloat()));

			text = ascii(velocity_edit->text());
			text.split(fields, "(),| ");
			if (fields.size() != 3)
			{
				Log.error() << (String)tr("Invalid values for velocity!") << std::endl;
				return;
			}

			atom->setVelocity(Vector3(fields[0].toFloat(),
																fields[1].toFloat(),
																fields[2].toFloat()));

			text = ascii(force_edit->text());
			text.split(fields, "(),| ");
			if (fields.size() != 3)
			{
				Log.error() << (String)tr("Invalid values for force!") << std::endl;
				return;
			}

			atom->setForce(Vector3(fields[0].toFloat(),
														 fields[1].toFloat(),
														 fields[2].toFloat()));
		}
	}
	catch(Exception::InvalidFormat&)
	{
		Log.error() << (String)tr("Invalid value, try a floating point number!") << std::endl;
		return;
	}

    if (RTTI::isKindOf<AtomContainer>(composite_))
	{
		((AtomContainer*)composite_)->setName(ascii(name_edit->text()));
	}

    if (RTTI::isKindOf<Residue>(composite_))
	{
		Residue* residue = (Residue*) composite_;
		residue->setID(ascii(id_edit->text()));
	}
	
	QDialog::accept();

	// Sending of messages to update Scene is done in MolecularControl
}

	}
}
