// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.C,v 1.6 2003/08/31 18:11:38 amoll Exp $

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/primitiveManager.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/MODELS/cartoonModel.h>
#include <BALL/VIEW/MODELS/ballAndStickModel.h>
#include <BALL/VIEW/MODELS/lineModel.h>
#include <BALL/VIEW/MODELS/surfaceModel.h>
#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>

#include <qcolordialog.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>

namespace BALL
{
	namespace VIEW
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
				precision_(DRAWING_PRECISION_HIGH),
				mode_(DRAWING_MODE_SOLID),
				coloring_method_(COLORING_ELEMENT),
				model_type_(MODEL_BALL_AND_STICK),
				rep_(0)
		{
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
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
			if (inifile.hasEntry("WINDOWS", "Display::custom_color"))
			{
				custom_color_.set(inifile.getValue("WINDOWS", "Display::custom_color"));
				QColor qcolor(custom_color_.getRed(), custom_color_.getGreen(), custom_color_.getBlue());
				color_sample->setBackgroundColor(qcolor);
			}

			if (inifile.hasEntry("WINDOWS", "Display::selected_color"))
			{
				BALL_SELECTED_COLOR.set(inifile.getValue("WINDOWS", "Display::selected_color"));
				QColor qcolor(BALL_SELECTED_COLOR.getRed(), 
											BALL_SELECTED_COLOR.getGreen(), 
											BALL_SELECTED_COLOR.getBlue());
				color_sample_selection->setBackgroundColor(qcolor);
			}

			getEntry_(inifile, "model", model_type_, *model_type_combobox);
			getEntry_(inifile, "precision", precision_, *precision_combobox);
			getEntry_(inifile, "coloring_method", coloring_method_, *coloring_method_combobox);

			coloring_method_combobox->setCurrentItem(coloring_method_);
			precision_combobox->setCurrentItem(precision_);
			model_type_combobox->setCurrentItem(model_type_);
		}

		void DisplayProperties::getEntry_(INIFile& inifile,
																		  const String& key, Index& assign_to, QComboBox& box)
		{
			if (!inifile.hasEntry("WINDOWS", "Display::" + key)) return;
			assign_to = inifile.getValue("WINDOWS", "Display::" + key).toUnsignedInt();
			box.setCurrentItem(assign_to);
		}


		void DisplayProperties::writePreferences(INIFile& inifile)
			throw()
		{
			const QColor& qcolor = color_sample->backgroundColor();
			custom_color_.set(qcolor.red(),
												qcolor.green(),
												qcolor.blue(),
												255 - transparency->value());

			// the display window position
			inifile.insertValue("WINDOWS", "Display::x", String(x()));
			inifile.insertValue("WINDOWS", "Display::y", String(y()));

			// the combobox values
			inifile.insertValue("WINDOWS", "Display::model", model_type_);
			inifile.insertValue("WINDOWS", "Display::precision", precision_);
			inifile.insertValue("WINDOWS", "Display::coloring_method", coloring_method_);
			inifile.insertValue("WINDOWS", "Display::custom_color", custom_color_);
			inifile.insertValue("WINDOWS", "Display::selected_color", BALL_SELECTED_COLOR);
		}


		void DisplayProperties::initializeWidget(MainControl& main_control)
			throw()
		{
			(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

			id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, 
																				 SLOT(showDialog()), CTRL+Key_I);   
		}


		void DisplayProperties::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, 
																											SLOT(showDialog()), CTRL+Key_I);   
		}


		void DisplayProperties::checkMenu(MainControl& main_control)
			throw()
		{
			(main_control.menuBar())->setItemChecked(id_, isVisible());
		}


		void DisplayProperties::showDialog()
		{
			show();
			raise();
		}

		void DisplayProperties::createRepresentationMode()
		{
			rep_ = 0;
			setCaption("create Representation");
			coloring_method_combobox->setCurrentItem(coloring_method_);
			precision_combobox->setCurrentItem(precision_);
			model_type_combobox->setCurrentItem(model_type_);
			apply_button->setEnabled(getMainControl()->getControlSelection().size());
		}

		void DisplayProperties::modifyRepresentationMode()
		{
			setCaption("modify Representation");
			coloring_method_combobox->setCurrentItem(coloring_method_);
			precision_combobox->setCurrentItem(precision_);
			model_type_combobox->setCurrentItem(model_type_);
			apply_button->setEnabled(true);
		}

		void DisplayProperties::selectPrecision(int index)
		{
			if (index > 2)
			{
				throw(InvalidOption(__FILE__, __LINE__, index));
			}
			precision_ = index;
		}

		void DisplayProperties::selectModel(int index)
		{
			if (index > 8)
			{
				throw(InvalidOption(__FILE__, __LINE__, index));
			}

			model_type_ = index;
		}

		void DisplayProperties::selectMode(int index)
		{
			if (index > VIEW::DRAWING_MODE_SOLID)
			{
				throw(InvalidOption(__FILE__, __LINE__, index));
			}

			transparency->setEnabled(index == VIEW::DRAWING_MODE_SOLID);

			mode_ = index;
		}

		void DisplayProperties::selectColoringMethod(int index)
		{
			if (index > 5)
			{
				throw(InvalidOption(__FILE__, __LINE__, index));
			}

			coloring_method_ = index;
		}


		void DisplayProperties::onNotify(Message *message)
			throw()
		{
			// new composite => build graphical representation and notify scene
			if (RTTI::isKindOf<NewMolecularMessage>(*message))
			{
				NewMolecularMessage *composite_message = RTTI::castTo<NewMolecularMessage>(*message);

				// generate graphical representation
				createRepresentation_(composite_message->getComposite());
			}

			if (RTTI::isKindOf<ShowDisplayPropertiesMessage>(*message))
			{
				show();
				return;
			}

			if (RTTI::isKindOf<RepresentationMessage>(*message))
			{
				switch (((RepresentationMessage*) message)->getType())
				{
					case RepresentationMessage::SELECTED:
					{
						rep_ = ((RepresentationMessage*) message)->getRepresentation();
						modifyRepresentationMode();
						return;
					}
					case RepresentationMessage::REMOVE:
					{
						rep_ = 0;
						createRepresentationMode();
						return;
					}

					default:
						return;
				}
			}

			if (RTTI::isKindOf<ControlSelectionMessage>(*message))
			{
				createRepresentationMode();
			}

			// disable apply button if selection is empty
			apply_button->setEnabled(getMainControl()->getControlSelection().size());
		}


		void DisplayProperties::applyButtonClicked()
		{
			// no selection present => return
			if (getMainControl()->getControlSelection().size() == 0 &&
					!rep_)
			{
				return;
			}

			createRepresentation_();

			// update scene
			SceneMessage scene_message;
			scene_message.setType(SceneMessage::REDRAW);
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

			coloring_method_combobox->setCurrentItem(COLORING_CUSTOM);
			coloring_method_ = COLORING_CUSTOM;
			update();
		}

		void DisplayProperties::editSelectionColor()
		{
			color_sample_selection->setBackgroundColor(QColorDialog::getColor(
															color_sample_selection->backgroundColor()));
			const QColor& qcolor = color_sample_selection->backgroundColor();
			BALL_SELECTED_COLOR.set((float)qcolor.red() / 255.0,
															(float)qcolor.green() / 255.0,
															(float)qcolor.blue() / 255.0);
			update();
		}

		// ------------------------------------------------------------------------
		// Model Processor methods
		// ------------------------------------------------------------------------

		void DisplayProperties::createRepresentation_(const Composite* composite)
			throw(InvalidOption)
		{
			MolecularModelProcessor* model_processor;

			switch (model_type_)
			{
				case MODEL_LINES:
					model_processor = new AddLineModel;
					break;
					
				case MODEL_STICK:
					model_processor = new AddBallAndStickModel;
					((AddBallAndStickModel*)model_processor)->enableStickModel();
					break;
					
				case MODEL_BALL_AND_STICK:
					model_processor = new AddBallAndStickModel;
					((AddBallAndStickModel*) model_processor)->enableBallAndStickModel();
					break;
					
				case MODEL_SURFACE:
					model_processor = new AddSurfaceModel;
					break;
					
				case MODEL_VDW:
					model_processor = new AddVanDerWaalsModel;
					break;

				case MODEL_BACKBONE:
					model_processor = new AddBackboneModel;
					break;

				case MODEL_CARTOON:
					model_processor = new AddCartoonModel;
					break;
					
				default:
					throw(InvalidOption(__FILE__, __LINE__, model_type_));
			}

			ColorProcessor* color_processor;

			switch(coloring_method_)
			{
				case COLORING_ELEMENT:
					color_processor = new ElementColorProcessor;
					break;

				case COLORING_RESIDUE_NAME:
					color_processor = new ResidueNameColorProcessor;
					break;

				case COLORING_RESIDUE_TYPE:
					color_processor = new ResidueNameColorProcessor;
					Log.error() << "Not implemented ? " << __FILE__ << "   " << __LINE__ << std::endl;
					break;

				case COLORING_ATOM_CHARGE:
					color_processor = new AtomChargeColorProcessor;
					break;

				case COLORING_CUSTOM:
				{
					color_processor = new CustomColorProcessor;
					QColor qcolor = color_sample->backgroundColor();
					custom_color_.set(qcolor.red(),
														qcolor.green(),
														qcolor.blue(),
														255 - transparency->value());
					color_processor->setDefaultColor(custom_color_);
					break;
				}

				case COLORING_DISTANCE:
					color_processor = new AtomDistanceColorProcessor;
					break;

				default:
					throw(InvalidOption(__FILE__, __LINE__, coloring_method_));
			}
					

			PrimitiveManager& pm = getMainControl()->getPrimitiveManager();
			Representation* rep = 0;
			if (rep_ == 0)
			{
				rep = new Representation(model_type_, precision_, mode_);
			}
			else 
			{
				rep = rep_;
			}

			model_processor->setColorProcessor(color_processor);
			model_connector_.setModelType(model_type_);
			model_processor->registerModelConnector(model_connector_);
			rep->setModelProcessor(model_processor);
			if (transparency->value() != 0)
			{
				rep->setProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
			}
			
			if (!rep_) 
			{	
				pm.insert(*rep);
				
				if (composite != 0)
				{
					rep->getComposites().insert(composite);
				}
				else
				{
					List<Composite*>& selection = getMainControl()->getControlSelection();
					List<Composite*>::Iterator it = selection.begin();
					for (; it != selection.end(); it++)
					{
						rep->getComposites().insert(*it);
					}
				}
			}
			else
			{
				rep_->setModelType(model_type_);
				rep_->setDrawingPrecision(precision_);
				rep_->setDrawingMode(mode_);
			}

			rep->update();

			// no refocus, if a representation already exists
			if (getMainControl()->getPrimitiveManager().getRepresentations().size() == 1)
			{
				CenterCameraMessage* ccmessage = new CenterCameraMessage;
				ccmessage->setDeletable(true);
				ccmessage->setComposite(composite);
				notify_(ccmessage);
			}

			RepresentationMessage* message = new RepresentationMessage;
			message->setDeletable(true);
			if (!rep_)
			{
				message->setType(RepresentationMessage::ADD);
			}
			else
			{
				message->setType(RepresentationMessage::UPDATE);
			}
			message->setRepresentation(rep);
			notify_(message);
		}

	} // namespace VIEW
} // namespace BALL
