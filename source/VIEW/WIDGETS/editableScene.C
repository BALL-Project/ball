/*    THIS IS EXPERIMENTAL CODE  
 *
 *    WE GIVE NO WARRANTY
 *    
 *    USE AT YOUR OWN RISK!!!!!!
 */

#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qcursor.h>

using std::endl;
using std::ostream;
using std::istream;


namespace BALL
{
	namespace VIEW
	{

EditableScene::EditableScene()
	throw()
	:	Scene(),
		edit_id_(-1)
{
	current_molecule_ = new Molecule();
}

EditableScene::EditableScene(QWidget* parent_widget, const char* name, WFlags w_flags)
	throw()
	: Scene(parent_widget, name, w_flags),
		edit_id_(-1)
{
	current_molecule_ = new Molecule();
}

EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , WFlags w_flags)
	throw()
	: Scene(eScene, parent_widget, name, w_flags),
		edit_id_(-1)
{
	current_molecule_ = new Molecule();
}
	
EditableScene::~EditableScene()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object EditableScene " << this << " of class EditableScene>" << std::endl;
	#endif 
}

void EditableScene::clear()
	throw()
{
	Scene::clear();
}

void EditableScene::initializeWidget(MainControl& main_control)
	throw()
{
	(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

	String hint;
	main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

	hint = "Switch to edit mode";
	edit_id_ =	main_control.insertMenuEntry(
			MainControl::DISPLAY, "&Edit Mode", this, SLOT(editMode_()), CTRL+Key_E, -1, hint);

	Scene::initializeWidget(main_control);

	return;
}


void EditableScene::finalizeWidget(MainControl& main_control)
	throw()
{
  if (current_mode_ == INSERT__MODE)
	{
		main_control.removeMenuEntry(MainControl::DISPLAY, "&Edit Mode", this, SLOT(editMode_()), CTRL+Key_E);
	}
	Scene::finalizeWidget(main_control);	
}


void EditableScene::checkMenu(MainControl& main_control)
	throw()
{
	menuBar()->setItemChecked(edit_id_,(current_mode_ == INSERT__MODE));
	Scene::checkMenu(main_control);
}



void EditableScene::mousePressEvent(QMouseEvent* e)
{
	// This is a TEST... instead of picking mode we will use edit mode later on...
	if (current_mode_ == INSERT__MODE)
	{
		int x_window_pos_old_ = e->x();
		int y_window_pos_old_ = e->y();

		// TEST!!! edit_mode_
	//	Protein* p = new Protein;
		// TEST
		PDBAtom* a = new PDBAtom(PTE[Element::C], "C");
		PDBAtom* b = new PDBAtom(PTE[Element::C], "C");
		a->setPosition(Vector3(e->x(),e->y(),0));
		b->setPosition(Vector3(e->x()-10.,e->y()-10., 0));
		Bond* c = new Bond("first try", *a, *b, Bond::ORDER__DOUBLE);		
		//p->insert(*a);
		//p->insert(*b);
		current_molecule_->insert(*a);
		current_molecule_->insert(*b);
		
Log.info() << "blubb" << std::endl;
		
		CompositeMessage *m = new CompositeMessage(*current_molecule_, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
		notify_(m);
	}
	
	Scene::mousePressEvent(e);
}



void EditableScene::mouseMoveEvent(QMouseEvent *e)
{
	Scene::mouseMoveEvent(e);
}


void EditableScene::mouseReleaseEvent(QMouseEvent *e)
{
	Scene::mouseReleaseEvent(e);
}


void EditableScene::editMode_()
{
	current_mode_ = INSERT__MODE;		
	setCursor(QCursor(Qt::SizeAllCursor));
	CompositeMessage *m = new CompositeMessage(*current_molecule_, CompositeMessage::NEW_MOLECULE);	
	notify_(m);                                                                                    	
}

	}//end of namespace 
} //end of namespace
