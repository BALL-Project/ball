// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.C,v 1.13.2.1 2003/01/07 13:21:25 anker Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h>
#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#include <BALL/MOLVIEW/FUNCTOR/objectSelector.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <qcolordialog.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>

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
			:	DisplayPropertiesData( parent, name ),
				ModularWidget(name),
				id_(-1),
				model_string_static_("stick"),
				model_string_dynamic_("line"),
				precision_string_static_("high"),
				precision_string_dynamic_("medium"),
				coloring_method_string_("by element"),
				distance_coloring_(false),

				address_array_((int)50, (int)-1),

				fragmentdb_(),
				color_calculator_(0),
				static_base_model_pointer_(0),
				dynamic_base_model_pointer_(0)
		{
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);

			  van_der_waals_model_static_.registerModelConnector(model_connector_);
			 van_der_waals_model_dynamic_.registerModelConnector(model_connector_);
			 ball_and_stick_model_static_.registerModelConnector(model_connector_);
			ball_and_stick_model_dynamic_.registerModelConnector(model_connector_);
								 line_model_static_.registerModelConnector(model_connector_);
								line_model_dynamic_.registerModelConnector(model_connector_);

			// seting up defaults
			color_calculator_ = &element_color_calculator_;

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

			if (inifile.hasEntry("WINDOWS", "Display::selectedcolor"))
			{
				BALL_SELECTED_COLOR.set(inifile.getValue("WINDOWS", "Display::selectedcolor"));
				QColor qcolor(BALL_SELECTED_COLOR.getRed(), 
											BALL_SELECTED_COLOR.getGreen(), 
											BALL_SELECTED_COLOR.getBlue());
				color_sample_selection->setBackgroundColor(qcolor);
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
			inifile.insertValue("WINDOWS", "Display::selectedcolor", BALL_SELECTED_COLOR);
		}


		void DisplayProperties::initializeWidget(MainControl& main_control)
			throw()
		{
			(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

			id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, 
																				 SLOT(openDialog()), CTRL+Key_I);   
		}


		void DisplayProperties::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, SLOT(openDialog()), CTRL+Key_I);   
		}


		void DisplayProperties::checkMenu(MainControl& main_control)
			throw()
		{
			(main_control.menuBar())->setItemChecked(id_, isVisible());
		}


		void DisplayProperties::openDialog()
		{
			setComboBoxIndex_(coloring_type_combobox, coloring_method_string_);
			setComboBoxIndex_(mode_resolution_combobox_static, precision_string_static_);
			setComboBoxIndex_(mode_resolution_combobox_dynamic, precision_string_dynamic_);
			setComboBoxIndex_(model_type_combobox_static, model_string_static_);
			setComboBoxIndex_(model_type_combobox_dynamic, model_string_dynamic_);
			show();
			raise();
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
			else
			{
				Log.error() << "Unknown dynamic precision: " << string << std::endl;
			}
		}

		
		void DisplayProperties::selectModelStatic(const QString& string)
		{
			model_string_static_ = string;

			setValue_(ADDRESS__STATIC_DRAWING_MODE, VALUE__DRAWING_MODE_SOLID);

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

			setValue_(ADDRESS__DYNAMIC_DRAWING_MODE, VALUE__DRAWING_MODE_SOLID);

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
//				setValue_(ADDRESS__DYNAMIC_DRAWING_MODE, VALUE__DRAWING_MODE_DOTS);
				setValue_(ADDRESS__DYNAMIC_DRAWING_MODE, VALUE__DRAWING_MODE_WIREFRAME);
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
			throw(InvalidOption)
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

				CenterCameraMessage* ccmessage = new CenterCameraMessage;
				ccmessage->setDeletable(true);
				ccmessage->setComposite(composite_message->getComposite());
				notify_(ccmessage);
			}
			else if (RTTI::isKindOf<DrawMessage>(*message))
			{
				DrawMessage* draw_message = RTTI::castTo<DrawMessage>(*message);
				applyOn_(*draw_message->getComposite());
			}

			// disable apply button if selection is empty
			if (MainControl::getMainControl(this)->getControlSelection().size() == 0)
			{
				apply_button->setEnabled(false);
			}
			else
			{
				apply_button->setEnabled(true);
			}
		}


		void DisplayProperties::applyButtonClicked()
		{
			// no selection present => return
			List<Composite*>& selection = MainControl::getMainControl(this)->getControlSelection();
			if (selection.size() == 0)
			{
				return;
			}

			// calculate distance color if selected
			if (distance_coloring_)
			{
				distance_color_calculator_.destroy();

				// for each element in the selection => perform generation
				List<Composite*>::Iterator it = selection.begin();
				for (; it != selection.end(); ++it)
				{
					(**it).apply(*((UnaryProcessor<Composite>*)&distance_color_calculator_));
				}

				distance_color_calculator_.calculateDistances();
			}
			

			// for top elements in the selection => perform generation
			// copy selection list, selection can change while applying models
			List<Composite*> updates = selection;

			setupStaticProcessor_();
			setupDynamicProcessor_();
			ObjectSelector selector;

			ChangedCompositeMessage* ccm = new ChangedCompositeMessage;
			ccm->setDeletable(false);
			const HashSet<Composite*>& picked_objects = MainControl::getMainControl(this)->getSelection();
			List<Composite*>::Iterator updates_it = updates.begin();
			for (; updates_it != updates.end(); updates_it++)
			{
				applyOn_(**updates_it);

				// reselect geometric objects, this is workaround
				if (picked_objects.has(*updates_it))   (*updates_it)->apply(selector);
					
				// perform update of the composites
				ccm->setComposite(*updates_it);
				notify_(ccm);
			
						
			}
				
			// update scene
			SceneMessage scene_message;
			scene_message.updateOnly();
			notify_(scene_message);
			setStatusbarText("");
		}


		void DisplayProperties::editColor()
		{
			color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
			const QColor& qcolor = color_sample->backgroundColor();
			custom_color_.set((float)qcolor.red() / 255.0,
												(float)qcolor.green() / 255.0,
												(float)qcolor.blue() / 255.0);

			setColorCalculator_(COLORCALCULATOR_VALUES__CUSTOM, custom_color_);

			coloring_method_string_ = "custom";
			setComboBoxIndex_(coloring_type_combobox, coloring_method_string_);
			selectColoringMethod(coloring_method_string_);
			update();
		}

		void DisplayProperties::editSelectionColor()
		{
			color_sample_selection->setBackgroundColor(QColorDialog::getColor(color_sample_selection->backgroundColor()));
			const QColor& qcolor = color_sample_selection->backgroundColor();
			BALL_SELECTED_COLOR.set((float)qcolor.red() / 255.0,
															(float)qcolor.green() / 255.0,
															(float)qcolor.blue() / 255.0);
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
			if (getValue_(ADDRESS__STATIC_MODEL) == VALUE__MODEL_SURFACE) composite.apply(remove_model_static_);
			composite.apply(*static_base_model_pointer_);
			
			// this is a workaround to ensure that if we have a dynamic and a static surface, both appear in the list
			if ( (getValue_(ADDRESS__STATIC_MODEL) == VALUE__MODEL_SURFACE) &&
			  	 (getValue_(ADDRESS__DYNAMIC_MODEL) == VALUE__MODEL_SURFACE)    )
			{
				// send the message again
				ChangedCompositeMessage* ccm = new ChangedCompositeMessage;
				ccm->setDeletable(true);
				ccm->setComposite(composite);
				notify_(ccm);
			}	
			
			if (getValue_(ADDRESS__DYNAMIC_MODEL) == VALUE__MODEL_SURFACE) composite.apply(remove_model_dynamic_);
			composite.apply(*dynamic_base_model_pointer_);
		}


		// selects and setups the static model processor
		void DisplayProperties::setupStaticProcessor_()
			throw(InvalidOption)
		{
			switch (getValue_(ADDRESS__STATIC_MODEL))
			{
				case VALUE__MODEL_LINES:
					static_base_model_pointer_ = &line_model_static_;
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
			throw(InvalidOption)
		{
			switch (getValue_(ADDRESS__DYNAMIC_MODEL))
			{
				case VALUE__MODEL_LINES:
					dynamic_base_model_pointer_ = &line_model_dynamic_;
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

				case VALUE__MODEL_BACKBONE:
					dynamic_base_model_pointer_ = &backbone_model_dynamic_;
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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.iC>
#		endif

	} // namespace MOLVIEW
} // namespace BALL
