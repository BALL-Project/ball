// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include <BALL/STRUCTURE/geometricProperties.h>
#include <qcolordialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <BALL/VIEW/DIALOGS/labelProperties.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/INIFile.h>

namespace BALL
{
	namespace VIEW
	{

		LabelProperties::LabelProperties(QWidget* parent, const char* name)
			throw()
			:	LabelPropertiesData( parent, name ),
				ModularWidget(name),
				id_(-1),
				selection_()
		{
			setCaption("Add Label");

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);

			hide();
		}

		LabelProperties::~LabelProperties()
			throw()
		{
		}

		void LabelProperties::fetchPreferences(INIFile& inifile)
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

				QColor qcolor(custom_color_.getRed(), 
											custom_color_.getGreen(), 
											custom_color_.getBlue());

				color_sample_->setBackgroundColor(qcolor);
			}
			else
			{
				custom_color_.set(ColorRGBA(1.,1.,0.,1.));
				QColor qcolor(custom_color_.getRed(), 
											custom_color_.getGreen(), 
											custom_color_.getBlue());

				color_sample_->setBackgroundColor(qcolor);
	    }			
		}

		void LabelProperties::writePreferences(INIFile& inifile)
			throw()
		{
			// the label window position
			inifile.insertValue("WINDOWS", "Label::x", String(x()));
			inifile.insertValue("WINDOWS", "Label::y", String(y()));

			// the color value
			inifile.insertValue("WINDOWS", "Label::customcolor", custom_color_);
		}

		void LabelProperties::onNotify(Message *message)
			throw()
		{
			// selection => store last selection for later processing
			if (RTTI::isKindOf<ControlSelectionMessage>(*message))
			{
				ControlSelectionMessage* selection = RTTI::castTo<ControlSelectionMessage>(*message);
				selection_ = selection->getSelection();
			}
			else
			{
				// no molecular selection => clear the selection
				selection_.clear();
			}

			// disabled apply button, if selection is empty
			apply_button_->setEnabled(!selection_.empty());
		}

		void LabelProperties::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::DISPLAY)->setCheckable(true);

			id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "Add &Label", this,
																				 SLOT(openDialog()), CTRL+Key_L);   
		}

		void LabelProperties::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "Add &Label", this,
																	 SLOT(openDialog()), CTRL+Key_L);   
		}

		void LabelProperties::checkMenu(MainControl& main_control)
			throw()
		{
			bool selected = (selection_.empty() ? false : true);

			main_control.menuBar()->setItemEnabled(id_, selected);
			main_control.menuBar()->setItemChecked(id_, isVisible());
		}

		void LabelProperties::openDialog()
		{
			show();
			raise();
		}

		void LabelProperties::applyButtonClicked()
		{
			// no selection present => return
			if (selection_.empty())
			{
				return;
			}

			// number of objects
			unsigned long number_of_objects = 0;

			// center processor
			GeometricCenterProcessor center_processor;
			
			// center to which the label will be attached
			Vector3 center(0,0,0);

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
			rep->setModelType(6);
			RepresentationMessage* arm = new RepresentationMessage;
			arm->setRepresentation(rep);
			arm->setType(RepresentationMessage::ADD);
			notify_(arm);
			
			// update scene
			SceneMessage* scene_message = new SceneMessage;
			scene_message->setType(SceneMessage::REDRAW);
			notify_(scene_message);
			
			// clear status bar
			setStatusbarText("");
		}

		void LabelProperties::editColor()
		{
			color_sample_->setBackgroundColor(QColorDialog::getColor(color_sample_->backgroundColor()));
			QColor qcolor = color_sample_->backgroundColor();

			custom_color_.set((float)qcolor.red() / 255.0,
											  (float)qcolor.green() / 255.0,
												(float)qcolor.blue() / 255.0);

			update();
		}

	} // namespace VIEW
} // namespace BALL
