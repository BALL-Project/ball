// $Id:

#include <qcolordialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h>

#define Inherited LabelPropertiesData

namespace BALL
{

	namespace MOLVIEW
	{

		LabelProperties::LabelProperties
			(QWidget* parent, const char* name)
			throw()
			:	Inherited( parent, name ),
				ModularWidget(name),
				id_(-1),
				selection_()
		{
			setCaption("Label Properties");

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
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
		}

		void LabelProperties::writePreferences(INIFile& inifile)
			throw()
		{
			//	
			// the label window position
			//
			inifile.setValue
				("WINDOWS", "Label::x", String(x()));
			inifile.setValue
				("WINDOWS", "Label::y", String(y()));

			// 
			// the color value
			// 
			inifile.setValue
				("WINDOWS", "Label::customcolor", custom_color_);
		}

		void LabelProperties::onNotify(Message *message)
			throw()
		{
			// selection => store last selection for later processing
			if (RTTI::isKindOf<MolecularSelectionMessage>(*message))
			{
				MolecularSelectionMessage *selection = RTTI::castTo<MolecularSelectionMessage>(*message);

				selection_ = selection->getSelection();
			}
			else
			{
				// no molecular selection => clear the selection
				selection_.clear();
			}

			// disabled apply button, if selection is empty
			if (selection_.empty())
			{
				apply_button_->setEnabled(false);
			}
			else
			{
				apply_button_->setEnabled(true);
			}
		}

		void LabelProperties::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::DISPLAY)->setCheckable(true);

			id_ = main_control.insertMenuEntry
							(MainControl::DISPLAY, "&Label Properties", this,
							 SLOT(openDialog()), 
							 CTRL+Key_L);   
		}

		void LabelProperties::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry
				(MainControl::DISPLAY, "&Label Properties", this,
				 SLOT(openDialog()), 
				 CTRL+Key_L);   
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

			// create Label and attach it to the first object in the selection
			GLLabel* label = new GLLabel;

			label->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_STATIC);
			label->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			label->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_ALWAYS_FRONT);
			label->setText(label_edit_->text().ascii());
			label->setColor(custom_color_);
			label->setVertex(center);
			
			list_it = selection_.begin();
			(**list_it).appendChild(*label);

			// mark composite for update
			ChangedCompositeMessage change_message;
			change_message.setComposite((*list_it));
			notify_(change_message);

			// update scene
			SceneMessage scene_message;
			scene_message.updateOnly();
			notify_(scene_message);
			
			// clear status bar
			WindowMessage window_message;
			notify_(window_message);
		}

		void LabelProperties::editColor()
		{
			color_sample_
				->setBackgroundColor(QColorDialog::getColor(color_sample_->backgroundColor()));

			ColorRGBA color;
			QColor qcolor = color_sample_->backgroundColor();

			color.set((float)qcolor.red() / 255.0,
								(float)qcolor.green() / 255.0,
								(float)qcolor.blue() / 255.0);

			custom_color_ = color;

			update();
		}

	} // namespace MOLVIEW

} // namespace BALL
