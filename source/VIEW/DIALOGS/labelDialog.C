// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/DIALOGS/labelDialog.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <qcolordialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

LabelDialog::LabelDialog(QWidget* parent, const char* name)
	throw()
	:	LabelDialogData( parent, name ),
		ModularWidget(name),
		id_(-1),
		selection_()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new LabelDialog " << this << std::endl;
#endif
	setCaption("Add Label");

	// register the widget with the MainControl
	ModularWidget::registerWidget(this);

	hide();
}

LabelDialog::~LabelDialog()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting LabelDialog " << this << std::endl;
#endif
}

void LabelDialog::fetchPreferences(INIFile& inifile)
	throw()
{
	// 
	// the geometry of the main window
	//
	int x_pos = x();
	int y_pos = y();

	if (inifile.hasEntry("WINDOWS", "Label::x"))
	{
		x_pos = inifile.getValue("WINDOWS", "Label::x").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Label::y"))
	{
		y_pos = inifile.getValue("WINDOWS", "Label::y").toInt();
	}

	move(x_pos, y_pos);

	// 
	// the color value
	//

	if (inifile.hasEntry("WINDOWS", "Label::customcolor"))
	{
		custom_color_.set(inifile.getValue("WINDOWS", "Label::customcolor"));
		color_sample_->setBackgroundColor(custom_color_.getQColor());
	}
	else
	{
		custom_color_.set(ColorRGBA(1.,1.,0.,1.));
		color_sample_->setBackgroundColor(custom_color_.getQColor());
	}			
}

void LabelDialog::writePreferences(INIFile& inifile)
	throw()
{
	// the label window position
	inifile.insertValue("WINDOWS", "Label::x", String(x()));
	inifile.insertValue("WINDOWS", "Label::y", String(y()));

	// the color value
	inifile.insertValue("WINDOWS", "Label::customcolor", custom_color_);
}

void LabelDialog::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "LabelDialog " << this << " onNotify " << message << std::endl;
#endif
	// selection => store last selection for later processing
	if (RTTI::isKindOf<ControlSelectionMessage>(*message))
	{
		ControlSelectionMessage* selection = RTTI::castTo<ControlSelectionMessage>(*message);
		selection_ = selection->getSelection();
	}

	// disabled apply button, if selection is empty
	apply_button_->setEnabled(!selection_.empty());
	getMainControl()->menuBar()->setItemEnabled(id_, !selection_.empty());
}

void LabelDialog::initializeWidget(MainControl& main_control)
	throw()
{
	main_control.initPopupMenu(MainControl::DISPLAY)->setCheckable(true);

	id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "Add &Label", this,
																		 SLOT(show()), CTRL+Key_L);   
}

void LabelDialog::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::DISPLAY, "Add &Label", this,
															 SLOT(show()), CTRL+Key_L);   
}

void LabelDialog::show()
{
	LabelDialogData::show();
	raise();
}

void LabelDialog::accept()
{
	// no selection present => return
	if (selection_.empty()) return;

	// number of objects
	Size number_of_objects = 0;

	// center processor
	GeometricCenterProcessor center_processor;
	
	// center to which the label will be attached
	Vector3 center;

	// process all objects in the selection list
	List<Composite*>::Iterator list_it = selection_.begin();
	for (; list_it != selection_.end(); ++list_it)
	{
		(*list_it)->apply(center_processor);

		center += center_processor.getCenter();
		++number_of_objects;
	}

	if (number_of_objects != 0)
	{
		center /= number_of_objects;
	}

	// create Label and Representation
	Label* label = new Label;
	label->setText(label_edit_->text().ascii());
	label->setColor(custom_color_);
	label->setVertex(center);

	Representation* rep = getMainControl()->getPrimitiveManager().createRepresentation();
	rep->insert(*label);
	rep->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
	rep->setModelType(MODEL_LABEL);
	RepresentationMessage* arm = new RepresentationMessage;
	arm->setRepresentation(*rep);
	arm->setType(RepresentationMessage::ADD);
	notify_(arm);
	
	// update scene
	SceneMessage* scene_message = new SceneMessage;
	scene_message->setType(SceneMessage::REDRAW);
	notify_(scene_message);
	
	// clear status bar
	setStatusbarText("Label added.");
}

void LabelDialog::editColor()
{
	color_sample_->setBackgroundColor(QColorDialog::getColor(color_sample_->backgroundColor()));
	QColor qcolor = color_sample_->backgroundColor();

	custom_color_.set(qcolor);
	update();
}

} } // namespaces
