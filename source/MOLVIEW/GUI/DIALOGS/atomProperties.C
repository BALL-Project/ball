#include <BALL/MOLVIEW/GUI/DIALOGS/atomProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>

namespace BALL
{
	namespace MOLVIEW
	{
/* 
 *  Constructs a AtomProperties which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AtomProperties::AtomProperties( Atom* atom, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : AtomPropertiesData( parent, name, modal, fl ),
			atom_(atom)
{
	name_edit->setText(atom_->getName().c_str());
	type_edit->setText(String(atom_->getType()).c_str());
	type_name_edit->setText(atom_->getTypeName().c_str());
	charge_edit->setText(String(atom_->getCharge()).c_str());
	radius_edit->setText(String(atom_->getRadius()).c_str());
	position_edit->setText((String("(") + 
													String(atom_->getPosition().x) + String("|") +
												 	String(atom_->getPosition().y) + String("|") +
												 	String(atom_->getPosition().z) + String(")")).c_str());

	force_edit->setText((String("(") + 
											 String(atom_->getForce().x) + String("|") +
											 String(atom_->getForce().y) + String("|") +
											 String(atom_->getForce().z) + String(")")).c_str());

	velocity_edit->setText((String("(") + 
													String(atom_->getVelocity().x) + String("|") +
													String(atom_->getVelocity().y) + String("|") +
													String(atom_->getVelocity().z) + String(")")).c_str());

	Element element;
	for (Position nr = 0; ; nr++) 
	{
		element = PTE.getElement(nr);
		if (element == Element::UNKNOWN) break;

		element_box->insertItem(element.getSymbol().c_str());
	}

	element_box->setCurrentText(atom->getElement().getSymbol().c_str());

	apply_button->setEnabled(false);
	show();
	raise();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AtomProperties::~AtomProperties()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void AtomProperties::changed()
{
	apply_button->setEnabled(true);
}

/* 
 * public slot
 */
void AtomProperties::applyClicked()
{
	try
	{
		atom_->setName(String(name_edit->text()));
		atom_->setType(String(type_edit->text()).toShort());
		atom_->setTypeName(String(type_name_edit->text()));
		atom_->setCharge(String(charge_edit->text()).toFloat());
		atom_->setRadius(String(radius_edit->text()).toFloat());
		atom_->setElement(PTE[(String(element_box->currentText()))]);
	}
	catch(Exception::InvalidFormat)
	{
		Log.error() << "Invalid value, try a floating point number!" << std::endl;
		return;
	}
	
	Log.info() << "Values applied to atom." << std::endl;
	close();
}

/* 
 * public slot
 */
void AtomProperties::closeClicked()
{
	close();
}


	}
}
