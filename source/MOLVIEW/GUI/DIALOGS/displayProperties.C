// $Id: displayProperties.C,v 1.13.4.11 2002/11/17 18:35:51 amoll Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>

#include <qcolordialog.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>

#define Inherited DisplayPropertiesData

namespace BALL
{
	namespace MOLVIEW
	{

	  InvalidOption::InvalidOption(const char* file, int line, int option)
			throw()
			: Exception::GeneralException(file, line, "Invalid option: ", option)
		{
    }
    
		DisplayProperties::DisplayProperties(QWidget* parent, const char* name)
			throw()
			:	Inherited( parent, name ),
				ModularWidget(name),
				id_(-1),
				select_id_(-1),
				deselect_id_(-1),
				center_camera_id_(-1),
				build_bonds_id_(-1),
				add_hydrogens_id_(-1),
				model_string_static_("stick"),
				model_string_dynamic_("line"),
				precision_string_static_("high"),
				precision_string_dynamic_("high"),
				coloring_method_string_("by element"),
				distance_coloring_(false),
				selection_(),

				view_center_vector_(0,0,0),
				view_direction_(2),
				view_distance_(25),
				address_array_((int)50, (int)-1),

				fragmentdb_(),
				color_calculator_(0),
				model_connector_(),
				
				ball_and_stick_model_static_(),
				ball_and_stick_model_dynamic_(),
				surface_model_static_(),
				surface_model_dynamic_(),
				van_der_waals_model_static_(),
				van_der_waals_model_dynamic_(),
				backbone_model_static_(),
				backbone_model_dynamic_(),
				remove_model_static_(),
				remove_model_dynamic_(),
				line_model_(),
				selector_(),
				deselector_(),
				
				static_base_model_pointer_(0),
				dynamic_base_model_pointer_(0),
				
				element_color_calculator_(),
				residue_name_color_calculator_(),
				atom_charge_color_calculator_(),
				distance_color_calculator_(),
				custom_color_calculator_()
		{
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);

			color_calculator_ = &element_color_calculator_;

			 van_der_waals_model_static_.registerModelConnector(model_connector_);
			 van_der_waals_model_dynamic_.registerModelConnector(model_connector_);
			ball_and_stick_model_static_.registerModelConnector(model_connector_);
			ball_and_stick_model_dynamic_.registerModelConnector(model_connector_);
								line_model_.registerModelConnector(model_connector_);

			// seting up defaults
			ball_and_stick_model_static_.enableStickModel();
			ball_and_stick_model_dynamic_.enableStickModel();

			setValue_(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_HIGH);
			setValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);

			setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
			setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);

			setValue_(ADDRESS__STATIC_DRAWING_MODE, VALUE__DRAWING_MODE_SOLID);
			setValue_(ADDRESS__DYNAMIC_DRAWING_MODE, VALUE__DRAWING_MODE_WIREFRAME);
		}


		DisplayProperties::~DisplayProperties()
			throw()
		{
		}


		void DisplayProperties::fetchPreferences(INIFile& inifile)
			throw()
		{
			// the geometry of the main window
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
				QColor qcolor(custom_color_.getRed(), custom_color_.getGreen(), custom_color_.getBlue());
				color_sample->setBackgroundColor(qcolor);
			}

			if (inifile.hasEntry("WINDOWS", "Display::modeldynamic"))
			{
				model_string_dynamic_ = inifile.getValue("WINDOWS", "Display::modeldynamic").c_str();
				setComboBoxIndex_(model_type_combobox_dynamic, model_string_dynamic_);
				selectModelDynamic(model_string_dynamic_);
			}

			if (inifile.hasEntry("WINDOWS", "Display::modelstatic"))
			{
				model_string_static_ = inifile.getValue("WINDOWS", "Display::modelstatic").c_str();
				setComboBoxIndex_(model_type_combobox_static, model_string_static_);
				selectModelStatic(model_string_static_);
			}
			
			if (inifile.hasEntry("WINDOWS", "Display::precisionstatic"))
			{
				precision_string_static_ = inifile.getValue("WINDOWS", "Display::precisionstatic").c_str();
				setComboBoxIndex_(mode_resolution_combobox_static, precision_string_static_);
				selectPrecisionStatic(precision_string_static_);
			}
				
			if (inifile.hasEntry("WINDOWS", "Display::precisiondynamic"))
			{
				precision_string_dynamic_ = inifile.getValue("WINDOWS", "Display::precisiondynamic").c_str();
				setComboBoxIndex_(mode_resolution_combobox_dynamic, precision_string_dynamic_);
				selectPrecisionDynamic(precision_string_dynamic_);
			}
					
			if (inifile.hasEntry("WINDOWS", "Display::colormethod"))
			{
				coloring_method_string_ = inifile.getValue("WINDOWS", "Display::colormethod").c_str();
				setComboBoxIndex_(coloring_type_combobox, coloring_method_string_);
				selectColoringMethod(coloring_method_string_);
			}
		}


		void DisplayProperties::writePreferences(INIFile& inifile)
			throw()
		{
			// the display window position
			inifile.insertValue("WINDOWS", "Display::x", String(x()));
			inifile.insertValue("WINDOWS", "Display::y", String(y()));

			// the combobox values
			inifile.insertValue("WINDOWS", "Display::modelstatic", model_string_static_.ascii());
			inifile.insertValue("WINDOWS", "Display::modeldynamic", model_string_dynamic_.ascii());
			inifile.insertValue("WINDOWS", "Display::precisionstatic", precision_string_static_.ascii());
			inifile.insertValue("WINDOWS", "Display::precisiondynamic", precision_string_dynamic_.ascii());
			inifile.insertValue("WINDOWS", "Display::colormethod", coloring_method_string_.ascii());
			inifile.insertValue("WINDOWS", "Display::customcolor", custom_color_);
		}


		void DisplayProperties::initializeWidget(MainControl& main_control)
			throw()
		{
			(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

			id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, SLOT(openDialog()), CTRL+Key_I);   
			select_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), CTRL+Key_S);   
			deselect_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), CTRL+Key_D);   
			center_camera_id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "Focus C&amera", this, 
																											 SLOT(centerCamera()), CTRL+Key_A);
			build_bonds_id_ = main_control.insertMenuEntry(MainControl::BUILD, "&Build Bonds", this, 
																										 SLOT(buildBonds()), CTRL+Key_B);
			add_hydrogens_id_ = main_control.insertMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, 
																											 SLOT(addHydrogens()), CTRL+Key_H);
		}


		void DisplayProperties::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, SLOT(openDialog()), CTRL+Key_I);   
			main_control.removeMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), CTRL+Key_S);   
			main_control.removeMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), CTRL+Key_D);   
			main_control.removeMenuEntry(MainControl::DISPLAY, "Focus C&amera", this, SLOT(centerCamera()), CTRL+Key_A);
			main_control.removeMenuEntry(MainControl::BUILD, "&Build Bonds", this, SLOT(buildBonds()), CTRL+Key_B);
			main_control.removeMenuEntry(MainControl::BUILD, "Add &Hydrogens", this, SLOT(addHydrogens()), CTRL+Key_H);
		}


		void DisplayProperties::checkMenu(MainControl& main_control)
			throw()
		{
			(main_control.menuBar())->setItemChecked(id_, isVisible());

			bool selected = true;
			int number_of_selected_objects = 0;

			if (selection_.empty())
			{
				selected = false;
			}
			else
			{
				number_of_selected_objects = selection_.size();
			}

			(main_control.menuBar())->setItemEnabled(select_id_, selected);
			(main_control.menuBar())->setItemEnabled(deselect_id_, selected);
			(main_control.menuBar())->setItemEnabled(add_hydrogens_id_, selected);
			(main_control.menuBar())->setItemEnabled(build_bonds_id_, selected);

			// these menu points for single items only
			(main_control.menuBar()) ->setItemEnabled(center_camera_id_, selected && (number_of_selected_objects == 1));
		}


		void DisplayProperties::openDialog()
		{
			show();
			raise();
			setComboBoxIndex_(coloring_type_combobox, coloring_method_string_);
			setComboBoxIndex_(mode_resolution_combobox_static, precision_string_static_);
			setComboBoxIndex_(mode_resolution_combobox_dynamic, precision_string_dynamic_);
			setComboBoxIndex_(model_type_combobox_static, model_string_static_);
			setComboBoxIndex_(model_type_combobox_dynamic, model_string_dynamic_);
		}


		void DisplayProperties::selectPrecisionStatic(const QString& string)
		{
			precision_string_static_ = string;

			if (string == "low")
			{
				setValue_(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
			}
			else if (string == "medium")
			{
				setValue_(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_MEDIUM);
			}
			else if (string == "high")
			{
				setValue_(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_HIGH);
			}
			else if (string == "ultra")
			{
				setValue_(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_ULTRA);
			}
			else
			{
				Log.error() << "Unknown static precision: " << string << std::endl;
			}
		}

		void DisplayProperties::selectPrecisionDynamic(const QString& string)
		{
			precision_string_dynamic_ = string;

			if (string == "low")
			{
				setValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
			}
			else if (string == "medium")
			{
				setValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_MEDIUM);
			}
			else if (string == "high")
			{
				setValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_HIGH);
			}
			else if (string == "ultra")
			{
				setValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_ULTRA);
			}
			else
			{
				Log.error() << "Unknown dynamic precision: " << string << std::endl;
			}
		}

		void DisplayProperties::selectModelStatic(const QString& string)
		{
			model_string_static_ = string;

			setValue_(ADDRESS__DYNAMIC_DRAWING_MODE, VALUE__DRAWING_MODE_SOLID);

			if (string == "none")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			}
			else if (string == "line")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
			}
			else if (string == "stick")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
			}
			else if (string == "ball and stick")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
			}
			else if (string == "backbone")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_BACKBONE);
			}
			else if (string == "surface")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_SURFACE);
			}
			else if (string == "van der Waals")
			{
				setValue_(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
			}
			else
			{
				Log.error() << "Unknown static model: " << string << std::endl;
			}
		}

		void DisplayProperties::selectModelDynamic(const QString& string)
		{
			model_string_dynamic_ = string;

			setValue_(ADDRESS__STATIC_DRAWING_MODE, VALUE__DRAWING_MODE_SOLID);

			if (string == "none")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			}
			else if (string == "line")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
			}
			else if (string == "stick")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_STICK);
			}
			else if (string == "ball and stick")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
			}
			else if (string == "backbone")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_BACKBONE);
			}
			else if (string == "surface")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_SURFACE);
				setValue_(ADDRESS__DYNAMIC_DRAWING_MODE, VALUE__DRAWING_MODE_DOTS);
			}
			else if (string == "van der Waals")
			{
				setValue_(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
			}
			else
			{
				Log.error() << "Unknown dynamic model: " << string << std::endl;
			}
		}


		void DisplayProperties::selectColoringMethod(const QString& string)
		{
			coloring_method_string_ = string;
			distance_coloring_ = false;

			if (string == "by element")
			{
				setColorCalculator_(COLORCALCULATOR_VALUES__ELEMENT);
			}
			else if (string == "by residue name")
			{
				setColorCalculator_(COLORCALCULATOR_VALUES__RESIDUE_NAME);
			}
			else if (string == "by atom charge")
			{
				setColorCalculator_(COLORCALCULATOR_VALUES__ATOM_CHARGE);
			}
			else if (string == "by atom distance")
			{
				setColorCalculator_(distance_color_calculator_);
				distance_coloring_ = true;
			}
			else if (string == "custom")
			{
				ColorRGBA color;
				QColor qcolor = color_sample->backgroundColor();

				color.set((float)qcolor.red() / 255.0,
									(float)qcolor.green() / 255.0,
									(float)qcolor.blue() / 255.0);

				setColorCalculator_(COLORCALCULATOR_VALUES__CUSTOM, color);
			}
		}


		void DisplayProperties::setColorCalculator_(ColorCalculatorValues values,
																								const ColorRGBA &first_color,
																								const ColorRGBA & /* second_color */,
																								const ColorRGBA & /* third_color */)
		{
			switch (values)
			{
				case COLORCALCULATOR_VALUES__CUSTOM:
					color_calculator_ = &custom_color_calculator_;
					color_calculator_->setDefaultColor(first_color);
					break;

				case COLORCALCULATOR_VALUES__ELEMENT:
					color_calculator_ = &element_color_calculator_;
					break;

				case COLORCALCULATOR_VALUES__RESIDUE_NAME:
					color_calculator_ = &residue_name_color_calculator_;
					break;

				case COLORCALCULATOR_VALUES__ATOM_CHARGE:
					color_calculator_ = &atom_charge_color_calculator_;
					break;
				
				default:
					throw(InvalidOption(__FILE__, __LINE__, values));
			}
		}

		void DisplayProperties::setStatusbarText_(String text)
			throw()
		{
			WindowMessage *window_message = new WindowMessage;
			window_message->setStatusBar(text);
			window_message->setDeletable(true);
			notify_(window_message);
		}

//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------to be moved:
//////////////////////////////////////////////////////////////////////////////////////////
		void DisplayProperties::onNotify(Message *message)
			throw()
		{
			// new composite => build graphical representation and notify scene
			if (RTTI::isKindOf<NewMolecularMessage>(*message))
			{
				NewMolecularMessage *composite_message = RTTI::castTo<NewMolecularMessage>(*message);

				// generate graphical representation
				setupStaticProcessor_();
				setupDynamicProcessor_();
				applyOn_(*(composite_message->getComposite()));

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
			else
			{
				selection_.clear();
			}

			// disable apply button if selection is empty
			if (selection_.empty())
			{
				apply_button->setEnabled(false);
			}
			else
			{
				apply_button->setEnabled(true);
			}
		}


		void DisplayProperties::select()
		{
			if (selection_.size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText_("selecting " + String(selection_.size()) + " objects...");

			int value_static = getValue_(ADDRESS__STATIC_MODEL);
			int value_dynamic = getValue_(ADDRESS__DYNAMIC_MODEL);

			selector_.clear();

			// copy list because the selection_ list can change after a changemessage event
			List<Composite*> temp_selection_ = selection_;
						
			List<Composite*>::ConstIterator list_it = temp_selection_.begin();	
			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);
			for (; list_it != temp_selection_.end(); ++list_it)
			{
				(*list_it)->apply(selector_);

				// mark composite for update
				change_message->setComposite((*list_it));
				notify_(change_message);
			}

			// restore old values
			setValue_(ADDRESS__STATIC_MODEL, value_static);
			setValue_(ADDRESS__DYNAMIC_MODEL, value_dynamic);

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message);

			setStatusbarText_("");
		}


		void DisplayProperties::deselect()
		{
			if (selection_.size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText_("deselecting " + String(selection_.size()) + "objects...");

			int value_static = getValue_(ADDRESS__STATIC_MODEL);
			int value_dynamic = getValue_(ADDRESS__DYNAMIC_MODEL);

			deselector_.clear();

			// copy list because the selection_ list can change after a changemessage event
			List<Composite*> temp_selection_ = selection_;

			List<Composite*>::ConstIterator list_it = temp_selection_.begin();	
			ChangedCompositeMessage* change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);
			for (; list_it != temp_selection_.end(); ++list_it)
			{
				(*list_it)->apply(deselector_);
				// mark composite for update
				change_message->setComposite((*list_it));
				notify_(change_message);
			}

			// restore old values
			setValue_(ADDRESS__STATIC_MODEL, value_static);
			setValue_(ADDRESS__DYNAMIC_MODEL, value_dynamic);

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message);

			setStatusbarText_("");
		}


		void DisplayProperties::centerCamera()
		{
			if (selection_.size() != 1)
			{
				return;
			}

			// use specified object processor for calculating the center
			calculateCenter_(*selection_.front());

			Vector3 view_point = getViewCenter_();

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->setCameraLookAt(view_point);

			view_point.z += getViewDistance_();
			scene_message->setCameraViewPoint(view_point);
			scene_message->setDeletable(true);
			notify_(scene_message);
		}


		void DisplayProperties::buildBonds()
		{
			if (selection_.size() == 0)
			{
				return;
			}

			// notify the main window
			setStatusbarText_("building bonds ...");

			// copy list because the selection_ list can change after a changemessage event
			List<Composite*> temp_selection_ = selection_;
			List<Composite*>::ConstIterator list_it = temp_selection_.begin();	
			
			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);
			
			Size number_of_bonds = 0;
			for (; list_it != temp_selection_.end(); ++list_it)
			{	
				(*list_it)->apply(fragmentdb_.build_bonds);
				number_of_bonds += fragmentdb_.build_bonds.getNumberOfBondsBuilt();
				applyOn_(**list_it);

				// mark composite for update
				change_message->setComposite((*list_it));
				notify_(change_message);
			}

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message);

			setStatusbarText_("");

			Log.info() << "Added " << number_of_bonds << " bonds." << std::endl;
		}


		void DisplayProperties::addHydrogens()
		{
			if (selection_.size() == 0)
			{
				return;
			}

			// notify the main window
			WindowMessage *window_message = new WindowMessage;
			QString message;
			message.sprintf("adding hydrogens ...");
			window_message->setStatusBar(message.ascii());
			window_message->setDeletable(true);
			notify_(window_message);

			// copy list because the selection_ list can change after a changemessage event
			List<Composite*> temp_selection_ = selection_;
			List<Composite*>::ConstIterator list_it = temp_selection_.begin();	

			ChangedCompositeMessage *change_message = new ChangedCompositeMessage;
			change_message->setDeletable(false);

			Size number_of_hydrogens = 0;

			for (; list_it != temp_selection_.end(); ++list_it)
			{	
				(*list_it)->apply(fragmentdb_.add_hydrogens);
				number_of_hydrogens += fragmentdb_.add_hydrogens.getNumberOfInsertedAtoms();
				(*list_it)->apply(fragmentdb_.build_bonds);
				applyOn_(**list_it);	

				// mark composite for update
				change_message->setComposite((*list_it));
				notify_(change_message);
			}

			Log.info() << "added " <<  number_of_hydrogens << " hydrogen atoms." << std::endl;

			// update scene
			SceneMessage *scene_message = new SceneMessage;
			scene_message->updateOnly();
			scene_message->setDeletable(true);
			notify_(scene_message);

			setStatusbarText_("");
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
			setupStaticProcessor_();
			setupDynamicProcessor_();
			for (; list_it != selection_.end(); ++list_it)
			{
				applyOn_(**list_it);

				// move composite pointer to update list for later update
				update_list.push_back(*list_it);
			}

			// perform update of the composites
			list_it = update_list.begin();
			ChangedCompositeMessage change_message;
			for (; list_it != update_list.end(); ++list_it)
			{
				// mark composite for update
				change_message.setComposite((*list_it));
				notify_(change_message);
			}

			// update scene
			SceneMessage scene_message;
			scene_message.updateOnly();
			notify_(scene_message);
			
			setStatusbarText_("");
			hide();
		}


		void DisplayProperties::editColor()
		{
			color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
			ColorRGBA color;
			QColor qcolor = color_sample->backgroundColor();
			color.set((float)qcolor.red() / 255.0,
								(float)qcolor.green() / 255.0,
								(float)qcolor.blue() / 255.0);

			custom_color_ = color;

			setColorCalculator_(COLORCALCULATOR_VALUES__CUSTOM, color);

			coloring_method_string_ = "custom";
			setComboBoxIndex_(coloring_type_combobox, coloring_method_string_);
			selectColoringMethod(coloring_method_string_);
			update();
		}

		void DisplayProperties::editSelectionColor()
		{
			color_sample_selection->setBackgroundColor(QColorDialog::getColor(color_sample_selection->backgroundColor()));
			ColorRGBA color;
			QColor qcolor = color_sample_selection->backgroundColor();
			color.set((float)qcolor.red() / 255.0,
								(float)qcolor.green() / 255.0,
								(float)qcolor.blue() / 255.0);

			BALL_SELECTED_COLOR = color;

			update();
		}

		void DisplayProperties::setComboBoxIndex_(QComboBox* combo_box, QString& item_string)
		{
			bool found = false;

			for (int index = 0; index < combo_box->count(); ++index)
			{
				if (combo_box->text(index) == item_string)
				{
					combo_box->setCurrentItem(index);
					found = true;
				}
			}

			if (!found)
			{
				Log.error() << "Unknown item: " << item_string << std::endl;
				combo_box->setCurrentItem(0);
			}
		}


		// ------------------------------------------------------------------------
		// Model Processor methods
		// ------------------------------------------------------------------------

		void DisplayProperties::applyOn_(Composite& composite)
		{
			// apply static visualization processor
			composite.apply(*static_base_model_pointer_);

			if (getValue_(ADDRESS__STATIC_MODEL) == VALUE__MODEL_BACKBONE)
			{
				return;
			}

			composite.apply(*dynamic_base_model_pointer_);
		}


		// selects and setups the static model processor
		void DisplayProperties::setupStaticProcessor_()
			throw()
		{
			switch (getValue_(ADDRESS__STATIC_MODEL))
			{
				case VALUE__MODEL_LINES:
					static_base_model_pointer_ = &line_model_;
					break;

				case VALUE__MODEL_STICK:
					ball_and_stick_model_static_.enableStickModel();
					static_base_model_pointer_ = &ball_and_stick_model_static_;
					break;
				
				case VALUE__MODEL_BALL_AND_STICK:
					ball_and_stick_model_static_.enableBallAndStickModel();
					static_base_model_pointer_ = &ball_and_stick_model_static_;
					break;
				
				case VALUE__MODEL_BACKBONE:
					static_base_model_pointer_ = &backbone_model_static_;
					break;
				
				case VALUE__MODEL_SURFACE:
					static_base_model_pointer_ = &surface_model_static_;
					break;

				case VALUE__MODEL_VAN_DER_WAALS:
					static_base_model_pointer_ = &van_der_waals_model_static_;
					break;

				case VALUE__MODEL_REMOVE:
					static_base_model_pointer_ = &remove_model_static_;
					break;

				default:
				{
					throw(InvalidOption(__FILE__, __LINE__, getValue_(ADDRESS__STATIC_MODEL)));
				}
			}

			// setup static drawing precision ------------------
			switch (getValue_(ADDRESS__STATIC_DRAWING_PRECISION))
			{
				case VALUE__DRAWING_PRECISION_LOW:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
					break;
					
				case VALUE__DRAWING_PRECISION_MEDIUM:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
					break;

				case VALUE__DRAWING_PRECISION_HIGH:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
					break;
					
				case VALUE__DRAWING_PRECISION_ULTRA:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
					break;
					
				default:
					throw(InvalidOption(__FILE__, __LINE__, getValue_(ADDRESS__STATIC_DRAWING_PRECISION)));
			}
			
			// setup static drawing mode ----------------
			switch (getValue_(ADDRESS__STATIC_DRAWING_MODE))
			{
				case VALUE__DRAWING_MODE_DOTS:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
					break;
					
				case VALUE__DRAWING_MODE_WIREFRAME:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
					break;
					
				case VALUE__DRAWING_MODE_SOLID:
					static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
					break;
					
				default:
					throw(InvalidOption(__FILE__, __LINE__, getValue_(ADDRESS__STATIC_DRAWING_MODE)));
			}
			
			// switch to dynamic model
			static_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_STATIC);

			// register the color calculator ------------------------------------------------
			static_base_model_pointer_->registerColorCalculator(*color_calculator_);

		}


		// selects and setups the dynamic model processor ------------------------------
		void DisplayProperties::setupDynamicProcessor_()
			throw()
		{
			switch (getValue_(ADDRESS__DYNAMIC_MODEL))
			{
				case VALUE__MODEL_LINES:
					dynamic_base_model_pointer_ = &line_model_;
					break;
					
				case VALUE__MODEL_STICK:
					ball_and_stick_model_dynamic_.enableStickModel();
					dynamic_base_model_pointer_ = &ball_and_stick_model_dynamic_;
					break;
					
				case VALUE__MODEL_BALL_AND_STICK:
					ball_and_stick_model_dynamic_.enableBallAndStickModel();
					dynamic_base_model_pointer_ = &ball_and_stick_model_dynamic_;
					break;
					
				case VALUE__MODEL_SURFACE:
					dynamic_base_model_pointer_ = &surface_model_dynamic_;
					break;
					
				case VALUE__MODEL_VAN_DER_WAALS:
					dynamic_base_model_pointer_ = &van_der_waals_model_dynamic_;
					break;
					
				case VALUE__MODEL_REMOVE:
					dynamic_base_model_pointer_ = &remove_model_dynamic_;
					break;

				default:
					throw(InvalidOption(__FILE__, __LINE__, getValue_(ADDRESS__DYNAMIC_MODEL)));
			}

			// switch to dynamic model
			dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			
			// setup dynamic drawing precision ----------
			switch (getValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION))
			{
				case VALUE__DRAWING_PRECISION_LOW:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
					break;
					
				case VALUE__DRAWING_PRECISION_MEDIUM:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
					break;
					
				case VALUE__DRAWING_PRECISION_HIGH:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
					break;
					
				case VALUE__DRAWING_PRECISION_ULTRA:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
					break;
					
				default:
					throw(InvalidOption(__FILE__, __LINE__, getValue_(ADDRESS__DYNAMIC_DRAWING_PRECISION)));
			}
			
			// setup dynamic drawing mode ---------------
			switch (getValue_(ADDRESS__DYNAMIC_DRAWING_MODE))
			{
				case VALUE__DRAWING_MODE_DOTS:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
					break;
					
				case VALUE__DRAWING_MODE_WIREFRAME:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
					break;
					 
				case VALUE__DRAWING_MODE_SOLID:
					dynamic_base_model_pointer_->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
					break;
					
				default:
					throw(InvalidOption(__FILE__, __LINE__, getValue_(ADDRESS__DYNAMIC_DRAWING_MODE)));
			}
			
			// register the color calculator ------------------------------------------------
			dynamic_base_model_pointer_->registerColorCalculator(*color_calculator_);
		}


		void DisplayProperties::calculateCenter_(Composite &composite)
		{
			GeometricCenterProcessor center;
			composite.apply((UnaryProcessor<Atom>&) center);			
					
			setViewCenter_(center.getCenter());
			setViewDirection_(2);

			if (getValue_(ADDRESS__CAMERA_DISTANCE) != -1)
			{
				setViewDistance_(getValue_(ADDRESS__CAMERA_DISTANCE));
			}
			else
			{
				BoundingBoxProcessor bbox;
				composite.apply(bbox);				
				setViewDistance_((bbox.getUpper() - bbox.getLower()).getLength() - center.getCenter().z + 3);
			}
		}


		bool DisplayProperties::checkResidue_(Composite &composite)
		{
			ResidueChecker residue_checker(fragmentdb_);

			if (RTTI::isKindOf<AtomContainer>(composite))
			{
				(RTTI::castTo<AtomContainer>(composite))->apply(residue_checker);
				return residue_checker.getStatus();
			}
			else if (RTTI::isKindOf<System>(composite))
			{
				(RTTI::castTo<System>(composite))->apply(residue_checker);

				return residue_checker.getStatus();				
			} 
			else 
			{
				Log.error() << "ResidueChecker: cannot apply to a " << typeid(composite).name() << " object" << std::endl;
			}
			
			return false;
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.iC>
#		endif

	} // namespace MOLVIEW
} // namespace BALL
