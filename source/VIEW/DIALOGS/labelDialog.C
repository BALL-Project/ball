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

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qfontdialog.h>
#include <qradiobutton.h>

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

	if (inifile.hasEntry("WINDOWS", "Label::font"))
	{
		font_.fromString(inifile.getValue("WINDOWS", "Label::font").c_str());
	}
	else
	{
		font_ = QFont("Helvetica", 12);
	}

	font_label->setFont(font_);
}

void LabelDialog::writePreferences(INIFile& inifile)
	throw()
{
	// the label window position
	inifile.insertValue("WINDOWS", "Label::x", String(x()));
	inifile.insertValue("WINDOWS", "Label::y", String(y()));

	// the color value
	inifile.insertValue("WINDOWS", "Label::customcolor", custom_color_);

	// the font size
	inifile.insertValue("WINDOWS", "Label::font", font_.toString().ascii());
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
	menuBar()->setItemEnabled(id_, !selection_.empty());
}

void LabelDialog::initializeWidget(MainControl& main_control)
	throw()
{
	main_control.initPopupMenu(MainControl::DISPLAY)->setCheckable(true);

	id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "Add &Label", this,
																		 SLOT(show()), CTRL+Key_L, -1,
																		 "Add a label for selected molecular objects");   
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


Representation* LabelDialog::createOneLabel_()
{
	Representation* rep = getMainControl()->getPrimitiveManager().createRepresentation();

	// number of objects
	Size number_of_objects = 0;

	GeometricCenterProcessor center_processor;
	
	// center to which the label will be attached
	Vector3 center;

	// process all objects in the selection list
	List<Composite*>::ConstIterator list_it = selection_.begin();
	for (; list_it != selection_.end(); ++list_it)
	{
		(*list_it)->apply(center_processor);

		center += center_processor.getCenter();
		++number_of_objects;

		rep->getComposites().insert(*list_it);
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
	label->setFont(font_);

	if (selection_.size() == 1)
	{
		label->setComposite(*selection_.begin());
	}

	rep->insert(*label);

	return rep;
}


Representation* LabelDialog::createMultipleLabels_()
{
	Representation* rep = getMainControl()->getPrimitiveManager().createRepresentation();

	List<Composite*>::ConstIterator list_it = selection_.begin();
	for (; list_it != selection_.end(); ++list_it)
	{
		GeometricCenterProcessor center_processor;
		(*list_it)->apply(center_processor);

		// create Label and Representation
		Label* label = new Label;
		label->setText(label_edit_->text().ascii());
		label->setColor(custom_color_);
		label->setVertex(center_processor.getCenter());
		label->setFont(font_);
		label->setComposite(*list_it);

		rep->insert(*label);
		rep->getComposites().insert(*list_it);
	}

	return rep;
}

void LabelDialog::accept()
{
	// no selection present => return
	if (selection_.empty()) return;

	Representation* rep = 0;

	if (all_items->isChecked())
	{
		rep = createOneLabel_();
	}
	else
	{
		rep = createMultipleLabels_();
	}
	rep->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
	rep->setModelType(MODEL_LABEL);

	/// MainControl::insert(rep) doesnt work here, no idea why !
	RepresentationMessage* arm = new RepresentationMessage;
	arm->setRepresentation(*rep);
	arm->setType(RepresentationMessage::ADD);
	notify_(arm);
	
	setStatusbarText("Label added.");
}

void LabelDialog::editColor()
{
	custom_color_.set(chooseColor(color_sample_));
}

void LabelDialog::addTag()
{
	QString tag;
	if (tag_box->currentText() == "Name")							 	tag = "%N";
	else if (tag_box->currentText() == "Residue ID") 	 	tag = "%I";
	else if (tag_box->currentText() == "Atom Type")			tag = "%T";
	else if (tag_box->currentText() == "Atom Charge") 	tag = "%C";

	label_edit_->setText(label_edit_->text() + tag);
	label_edit_->update();
}

void LabelDialog::fontSelected()
{
	bool ok = true;
	QFont font = QFontDialog::getFont(&ok, font_, 0);

	if (!ok) return;

	font_label->setFont(font);
	font_ = font;
}

void LabelDialog::modeChanged()
{
  tag_box->setEnabled(!all_items->isChecked());
  add_tag_button->setEnabled(!all_items->isChecked());
}

void LabelDialog::textChanged()
{
	apply_button_->setEnabled(label_edit_->text() != "");
}

} } // namespaces
