#include <BALL/MOLVIEW/GUI/DIALOGS/atomSettings.h>
#include <qlineedit.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace MOLVIEW
	{
/* 
 *  Constructs a AtomSettings which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AtomSettings::AtomSettings( Atom* atom, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : AtomSettingsData( parent, name, modal, fl ),
			atom_(atom)
{
	name_edit->setText(atom_->getName().c_str());
	charge_edit->setText(String(atom_->getCharge()).c_str());
	radius_edit->setText(String(atom_->getRadius()).c_str());
	position_edit_1->setText(String(atom_->getPosition().x).c_str());
	position_edit_2->setText(String(atom_->getPosition().y).c_str());
	position_edit_3->setText(String(atom_->getPosition().z).c_str());
	force_edit_1->setText(String(atom_->getForce().x).c_str());
	force_edit_2->setText(String(atom_->getForce().y).c_str());
	force_edit_3->setText(String(atom_->getForce().z).c_str());
	velocity_edit_1->setText(String(atom_->getVelocity().x).c_str());
	velocity_edit_2->setText(String(atom_->getVelocity().y).c_str());
	velocity_edit_3->setText(String(atom_->getVelocity().z).c_str());

	apply_button->setEnabled(false);
	show();
	raise();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AtomSettings::~AtomSettings()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void AtomSettings::changed( const QString & )
{
	apply_button->setEnabled(true);
}

/* 
 * public slot
 */
void AtomSettings::applyClicked()
{
	try
	{
		atom_->setName(String(name_edit->text()));
		atom_->setCharge(String(charge_edit->text()).toFloat());
		atom_->setRadius(String(radius_edit->text()).toFloat());
		Vector3 position;
		position.x = String(position_edit_1->text()).toFloat();
		position.y = String(position_edit_2->text()).toFloat();
		position.z = String(position_edit_3->text()).toFloat();
		atom_->setPosition(position);
		Vector3 force;
		force.x = String(force_edit_1->text()).toFloat();
		force.y = String(force_edit_2->text()).toFloat();
		force.z = String(force_edit_3->text()).toFloat();
		atom_->setForce(force);
		Vector3 velo;	
		velo.x = String(velocity_edit_1->text()).toFloat();
		velo.y = String(velocity_edit_2->text()).toFloat();
		velo.z = String(velocity_edit_3->text()).toFloat();
		atom_->setVelocity(velo);
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
void AtomSettings::closeClicked()
{
	close();
}


	}
}
