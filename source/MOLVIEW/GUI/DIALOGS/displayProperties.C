// $Id:

#include <qcolordialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h>

#define Inherited DisplayPropertiesData

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

DisplayProperties::DisplayProperties
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name ),
	ModularWidget(name),
	id_(-1),
  model_string_("stick"),
  precision_string_("high"),
  coloring_method_string_("by element"),
	object_processor_(),
	distance_color_calculator_(),
	distance_coloring_(false),
	selection_()
{
	setCaption("Display Settings");

	// register the widget with the MainControl
  ModularWidget::registerWidget(this);
}

DisplayProperties::~DisplayProperties()
{
}

void DisplayProperties::setPreferences(INIFile& inifile) const
{
	//	
	// the display window position
	//
	inifile.setValue
		("WINDOWS", "Display::x", String(x()));
	inifile.setValue
		("WINDOWS", "Display::y", String(y()));

	// 
	// the combobox values
	// 
	inifile.setValue
		("WINDOWS", "Display::model", model_string_.ascii());
	inifile.setValue
		("WINDOWS", "Display::precision", precision_string_.ascii());
	inifile.setValue
		("WINDOWS", "Display::colormethod", coloring_method_string_.ascii());
	inifile.setValue
		("WINDOWS", "Display::customcolor", custom_color_);
}

void DisplayProperties::getPreferences(const INIFile& inifile)
{
	// 
	// the geometry of the main window
	//
	int x_pos = x();
	int y_pos = y();

	if (inifile.hasEntry("WINDOWS", "Display::x"))
	{
		x_pos = inifile.getValue("WINDOWS", "Display::x").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Display::y"))
	{
		y_pos = inifile.getValue("WINDOWS", "Display::y").toInt();
	}

	move(x_pos, y_pos);

	// 
	// the combobox values
	//

	if (inifile.hasEntry("WINDOWS", "Display::customcolor"))
	{
		custom_color_.set(inifile.getValue("WINDOWS", "Display::customcolor"));

		QColor qcolor(custom_color_.red(), 
									custom_color_.green(), 
									custom_color_.blue());

		color_sample->setBackgroundColor(qcolor);
	}

	if (inifile.hasEntry("WINDOWS", "Display::model"))
	{
		model_string_ = inifile.getValue("WINDOWS", "Display::model").c_str();

		setComboBoxIndex_(model_type_combobox_, model_string_);
		
		selectModel(model_string_);
	}
	
	if (inifile.hasEntry("WINDOWS", "Display::precision"))
	{
		precision_string_ = inifile.getValue("WINDOWS", "Display::precision").c_str();

		setComboBoxIndex_(mode_resolution_combobox_, precision_string_);

		selectPrecision(precision_string_);
	}
	
	if (inifile.hasEntry("WINDOWS", "Display::colormethod"))
	{
		coloring_method_string_ = inifile.getValue("WINDOWS", "Display::colormethod").c_str();

		setComboBoxIndex_(coloring_type_combobox_, coloring_method_string_);

		selectColoringMethod(coloring_method_string_);
	}
}


void DisplayProperties::onNotify(Message *message)
{
	// new composite => build graphical representation and notify scene
	if (RTTI::isKindOf<NewMolecularMessage>(*message))
	{
		NewMolecularMessage *composite_message = RTTI::castTo<NewMolecularMessage>(*message);

		// generate graphical representation
		object_processor_.applyOn(*(composite_message->getComposite()));

		// notify tree of the changes
		ChangedMolecularMessage *changed_message = new ChangedMolecularMessage(*composite_message);
		changed_message->setDeletable(true);
		notify_(changed_message);

		// calculate center of the new composite
		Composite *composite = composite_message->getComposite();
		SceneMessage *scene_message = new SceneMessage;
		scene_message->setDeletable(true);

		GeometricCenterProcessor center;
		composite->apply(center);        
		
		scene_message->setCameraLookAt(center.getCenter());
		
		Vector3 view_point = center.getCenter();
		view_point.z = view_point.z + 5;
		scene_message->setCameraViewPoint(view_point);

		// notify scene to perform an update and set the camera to the new object
		notify_(scene_message);
	}
	// selection => store last selection for later processing
	else if (RTTI::isKindOf<MolecularSelectionMessage>(*message))
	{
		MolecularSelectionMessage *selection = RTTI::castTo<MolecularSelectionMessage>(*message);

		selection_ = selection->getSelection();
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

void DisplayProperties::initializeWidget(MainControl& main_control)
{
	(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

	id_ = main_control.insertMenuEntry
		      (MainControl::DISPLAY, "D&isplay Properties", this,
					 SLOT(openDialog()), 
					 CTRL+Key_I);   
}

void DisplayProperties::finalizeWidget(MainControl& main_control)
{
	main_control.removeMenuEntry
		(MainControl::DISPLAY, "D&isplay Properties", this,
		 SLOT(openDialog()), 
		 CTRL+Key_I);   
}

void DisplayProperties::checkMenu(MainControl& main_control)
{
	(main_control.menuBar())->setItemChecked(id_, isVisible());
}

void DisplayProperties::openDialog()
{
	show();
	raise();
}

void DisplayProperties::selectPrecision(const QString& string)
{
	precision_string_ = string;

	if (string == "low")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "medium")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_MEDIUM);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "high")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_HIGH);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "ultra")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_ULTRA);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
}
 
void DisplayProperties::selectModel(const QString& string)
{
	model_string_ = string;

	object_processor_.
		setValue(ADDRESS__DYNAMIC_DRAWING_MODE, 
							 VALUE__DRAWING_MODE_SOLID);

	if (string == "none")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
	}
	else if (string == "line")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "stick")
	{
 		object_processor_.
			setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "ball and stick")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "surface")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_SURFACE);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_SURFACE);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_DRAWING_MODE, 
								 VALUE__DRAWING_MODE_DOTS);
	}
	else if (string == "van der Waals")
	{
		object_processor_.
			setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
		object_processor_.
			setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
}

void DisplayProperties::selectColoringMethod(const QString& string)
{
	coloring_method_string_ = string;

	distance_coloring_ = false;

	if (string == "by element")
	{
		object_processor_.
			setColorCalculator(COLORCALCULATOR_VALUES__ELEMENT);
	}
	else if (string == "by residue name")
	{
		object_processor_.
			setColorCalculator(COLORCALCULATOR_VALUES__RESIDUE_NAME);
	}
	else if (string == "by atom charge")
	{
		object_processor_.
			setColorCalculator(COLORCALCULATOR_VALUES__ATOM_CHARGE);
	}
	else if (string == "by atom distance")
	{
		object_processor_.
			setColorCalculator(distance_color_calculator_);
		
		distance_coloring_ = true;
	}
	else if (string == "custom")
	{
		ColorRGBA color;
		QColor qcolor = color_sample->backgroundColor();

		color.set((float)qcolor.red() / 255.0,
							(float)qcolor.green() / 255.0,
							(float)qcolor.blue() / 255.0);

		object_processor_.
			setColorCalculator(COLORCALCULATOR_VALUES__CUSTOM, color);
	}
}

void DisplayProperties::applyButtonClicked()
{
	// no selection present => return
	if (selection_.empty())
	{
		return;
	}

	// calculate distance color if selected
	if (distance_coloring_)
	{
		distance_color_calculator_.destroy();

		// for each element in the selection => perform generation
		List<Composite*>::Iterator list_it = selection_.begin();
		for (; list_it != selection_.end(); ++list_it)
		{
			(**list_it).apply(*((UnaryProcessor<Composite>*)&distance_color_calculator_));
		}

		distance_color_calculator_.calculateDistances();
	}
	
	List<Composite*> update_list;

	// for each element in the selection => perform generation
	List<Composite*>::ConstIterator list_it = selection_.begin();
	for (; list_it != selection_.end(); ++list_it)
	{
		object_processor_.applyOn(**list_it);

		// move composite pointer to update list for later update
		update_list.push_back(*list_it);
	}

	// perform update of the composites
	list_it = update_list.begin();
	for (; list_it != update_list.end(); ++list_it)
	{
		// mark composite for update
		ChangedCompositeMessage change_message;
		change_message.setComposite((*list_it));
		notify_(change_message);
	}

	// update scene
	SceneMessage scene_message;
	scene_message.updateOnly();
	notify_(scene_message);
	
	// clear status bar
	WindowMessage window_message;
	notify_(window_message);
}

void DisplayProperties::editColor()
{
	color_sample
		->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
	ColorRGBA color;
	QColor qcolor = color_sample->backgroundColor();
 	color.set((float)qcolor.red() / 255.0,
						(float)qcolor.green() / 255.0,
						(float)qcolor.blue() / 255.0);

	custom_color_ = color;

	object_processor_.setColorCalculator(COLORCALCULATOR_VALUES__CUSTOM, color);

	coloring_method_string_ = "custom";
  setComboBoxIndex_(coloring_type_combobox_, coloring_method_string_);
  selectColoringMethod(coloring_method_string_);
	update();
}

void DisplayProperties::setComboBoxIndex_(QComboBox* combo_box, QString& item_string)
{
	int index;
	bool found = false;

	for (index = 0; index < combo_box->count(); ++index)
	{
		if (combo_box->text(index) == item_string)
		{
			combo_box->setCurrentItem(index);

			found = true;
		}
	}

	if (found == false)
	{
		combo_box->setCurrentItem(0);
	}
}

//#		ifdef BALL_NO_INLINE_FUNCTIONS
//#			include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.iC>
//#		endif

	} // namespace MOLVIEW

} // namespace BALL
