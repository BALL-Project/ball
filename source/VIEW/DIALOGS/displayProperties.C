// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.C,v 1.40 2003/11/18 16:59:12 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/MODELS/cartoonModel.h>
#include <BALL/VIEW/MODELS/ballAndStickModel.h>
#include <BALL/VIEW/MODELS/lineModel.h>
#include <BALL/VIEW/MODELS/surfaceModel.h>
#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
#include <BALL/VIEW/MODELS/HBondModel.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <qcolordialog.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{

DisplayProperties::InvalidOption::InvalidOption(const char* file, int line, int option)
	throw()
	: Exception::GeneralException(file, line, "Invalid option: ", option)
{
}

DisplayProperties::DisplayProperties(QWidget* parent, const char* name)
	throw()
	:	DisplayPropertiesData( parent, name ),
		ModularWidget(name),
		model_settings_(0),
		coloring_settings_(0),
		preferences_(0),
		id_(-1),
		rep_(0),
		advanced_options_modified_(false)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DisplayProperties " << this << std::endl;
#endif
	ModularWidget::registerWidget(this);

	model_type_combobox->clear();
	for (Index p = 0; p < MODEL_LABEL; p++)
	{
		model_type_combobox->insertItem(VIEW::getModelName((VIEW::ModelTypes)p).c_str());
	}

	coloring_method_combobox->clear();
	for (Index p = 0; p < COLORING_UNKNOWN; p++)
	{
		coloring_method_combobox->insertItem(VIEW::getColoringName((VIEW::ColoringMethod)p).c_str());
	}
}


DisplayProperties::~DisplayProperties()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting DisplayProperties " << this << std::endl;
#endif
	
	if (model_settings_ != 0) delete model_settings_;
	if (coloring_settings_ != 0) delete coloring_settings_;
}


void DisplayProperties::fetchPreferences(INIFile& inifile)
	throw()
{
	ModularWidget::fetchPreferences(inifile);
	// 
	// the combobox values
	//
	if (inifile.hasEntry("REPRESENTATION", "custom_color"))
	{
		custom_color_.set(inifile.getValue("REPRESENTATION", "custom_color"));
		QColor qcolor(custom_color_.getRed(), custom_color_.getGreen(), custom_color_.getBlue());
		color_sample->setBackgroundColor(qcolor);
	}

	if (inifile.hasEntry("REPRESENTATION", "selected_color"))
	{
		BALL_SELECTED_COLOR.set(inifile.getValue("REPRESENTATION", "selected_color"));
		QColor qcolor(BALL_SELECTED_COLOR.getRed(), 
									BALL_SELECTED_COLOR.getGreen(), 
									BALL_SELECTED_COLOR.getBlue());
		color_sample_selection->setBackgroundColor(qcolor);
	}

	getEntry_(inifile, "model", *model_type_combobox);
	getEntry_(inifile, "coloring_method", *coloring_method_combobox);
	getEntry_(inifile, "precision", *precision_combobox);

	if (inifile.hasEntry("REPRESENTATION", "surface_precision"))
	{
		custom_precision_button->setChecked(true);
		precision_slider->setValue((Position)
				(inifile.getValue("REPRESENTATION", "surface_precision").toFloat() * 10.0));
	}
	else
	{ 
		precision_slider->setValue(12 * 10);
		presets_precision_button->setChecked(true);
	}

	if (model_settings_ != 0)
	{
		model_settings_->fetchPreferences(inifile);
	}
}

void DisplayProperties::getEntry_(INIFile& inifile, const String& key, QComboBox& box)
{
	if (!inifile.hasEntry("REPRESENTATION", "" + key)) return;
	box.setCurrentItem(inifile.getValue("REPRESENTATION", "" + key).toUnsignedInt());
}


void DisplayProperties::writePreferences(INIFile& inifile)
	throw()
{
	ModularWidget::writePreferences(inifile);
	inifile.appendSection("REPRESENTATION");
	const QColor& qcolor = color_sample->backgroundColor();
	custom_color_.set(qcolor.red(),
										qcolor.green(),
										qcolor.blue(),
										255 - (Position) (transparency_slider->value() * 2.55));

	// the combobox values
	inifile.insertValue("REPRESENTATION", "model", model_type_combobox->currentItem());
	if (presets_precision_button->isChecked())
	{
		inifile.insertValue("REPRESENTATION", "precision", precision_combobox->currentItem());
	}
	else
	{
		inifile.insertValue("REPRESENTATION", "surface_precision", ((float)precision_slider->value()) / 10.0);
	}
	inifile.insertValue("REPRESENTATION", "coloring_method", coloring_method_combobox->currentItem());
	inifile.insertValue("REPRESENTATION", "custom_color", custom_color_);
	inifile.insertValue("REPRESENTATION", "selected_color", BALL_SELECTED_COLOR);

	if (model_settings_ != 0)
	{
		model_settings_->writePreferences(inifile);
	}
}


void DisplayProperties::initializeWidget(MainControl& main_control)
	throw()
{
	(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

	String hint("Create a new representation or modify an existing one");
	id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, 
																		 SLOT(show()), CTRL+Key_I, -1, hint);   
}


void DisplayProperties::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, 
																									SLOT(show()), CTRL+Key_I);   
}

void DisplayProperties::initializePreferencesTab(Preferences &preferences)
	throw()
{
	preferences_ = &preferences;
	model_settings_ = new ModelSettingsDialog(this);
	preferences.insertTab(model_settings_, "Model Options");
	coloring_settings_ = new ColoringSettingsDialog(this);
	preferences.insertTab(coloring_settings_, "Coloring Options");
}

void DisplayProperties::finalizePreferencesTab(Preferences &preferences)
	throw()
{
	if (model_settings_) 
	{
		preferences.removeTab(model_settings_);
		delete model_settings_;
		model_settings_ = 0;
	}
	if (coloring_settings_) 
	{
		preferences.removeTab(coloring_settings_);
		delete coloring_settings_;
		coloring_settings_ = 0;
	}
}


void DisplayProperties::checkMenu(MainControl& main_control)
	throw()
{
	(main_control.menuBar())->setItemChecked(id_, isVisible());
}


void DisplayProperties::show()
{
	DisplayPropertiesData::show();
	raise();
}

void DisplayProperties::createRepresentationMode()
{
	rep_ = 0;
	setCaption("create Representation");
	apply_button->setEnabled(getMainControl()->getControlSelection().size());
}

void DisplayProperties::modifyRepresentationMode()
{
	setCaption("modify Representation");
	if (rep_->getColoringType() != COLORING_UNKNOWN)
	{
		coloring_method_combobox->setCurrentItem(rep_->getColoringType());
	}
	
	precision_combobox->setCurrentItem(rep_->getDrawingPrecision());
	
	if (rep_->getModelType() != MODEL_UNKNOWN)
	{
		model_type_combobox->setCurrentItem(rep_->getModelType());
	}
	mode_combobox->setCurrentItem(rep_->getDrawingMode());

	if (rep_->getColorProcessor() != 0)
	{
		custom_color_ = rep_->getColorProcessor()->getDefaultColor();
		QColor qcolor(custom_color_.getRed(), custom_color_.getGreen(), custom_color_.getBlue());
		color_sample->setBackgroundColor(qcolor);
	}

	transparency_slider->setValue((Size)(rep_->getTransparency() / 2.55));

	checkDrawingPrecision_();
	getAdvancedOptions_();

	apply_button->setEnabled(true);
}

void DisplayProperties::selectModel(int index)
{
	if (index >= MODEL_LABEL)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	checkDrawingPrecision_();
}

void DisplayProperties::selectMode(int index)
{
	if (index > VIEW::DRAWING_MODE_SOLID)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	transparency_slider->setEnabled(index == VIEW::DRAWING_MODE_SOLID);
}

void DisplayProperties::selectColoringMethod(int index)
{
	if (index > COLORING_CUSTOM)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}
}


void DisplayProperties::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "DisplayProperties::onNotify " << message << std::endl;
#endif
	// new molecule => build graphical representation and notify scene
	if (RTTI::isKindOf<CompositeMessage>(*message))
	{
		CompositeMessage *composite_message = RTTI::castTo<CompositeMessage>(*message);
		if (composite_message->getType() != CompositeMessage::NEW_MOLECULE) return;
		// generate graphical representation
		createRepresentation_(composite_message->getComposite());
		return;
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
				if (((RepresentationMessage*) message)->getRepresentation()->getModelType() >= MODEL_LABEL)
				{
					return;
				}
				rep_ = ((RepresentationMessage*) message)->getRepresentation();
				modifyRepresentationMode();
				return;
			}
			case RepresentationMessage::REMOVE:
			{
				createRepresentationMode();
				return;
			}

			default:
				return;
		}
		return;
	}

	if (RTTI::isKindOf<ControlSelectionMessage>(*message))
	{
		createRepresentationMode();
		// disable apply button if selection is empty
		apply_button->setEnabled(getMainControl()->getControlSelection().size());
	}
}


void DisplayProperties::applyButtonClicked()
{
	// no molecular or representation selection present 
	if (getMainControl()->getControlSelection().size() == 0 && rep_ == 0)
	{
		return;
	}

	setStatusbarText("building model...");
	createRepresentation_();
	setStatusbarText("drawing representation...");

	// update scene
	SceneMessage* scene_message = new SceneMessage(SceneMessage::REDRAW);
	notify_(scene_message);
	setStatusbarText("finished drawing");
}


void DisplayProperties::editColor()
{
	color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
	const QColor& qcolor = color_sample->backgroundColor();
	custom_color_.set((float)qcolor.red() / 255.0,
										(float)qcolor.green() / 255.0,
										(float)qcolor.blue() / 255.0);
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
	ModelProcessor* model_processor;

	switch (model_type_combobox->currentItem())
	{
		case MODEL_LINES:
			model_processor = new AddLineModel;
			break;
			
		case MODEL_STICK:
			model_processor = new AddBallAndStickModel;
			((AddBallAndStickModel*)model_processor)->enableStickModel();
			((AddBallAndStickModel*)model_processor)->setStickRadius(model_settings_->getStickStickRadius());
			break;
			
		case MODEL_BALL_AND_STICK:
			model_processor = new AddBallAndStickModel;
			((AddBallAndStickModel*)model_processor)->enableBallAndStickModel();
			((AddBallAndStickModel*)model_processor)->setStickRadius(model_settings_->getBallAndStickStickRadius());
			((AddBallAndStickModel*)model_processor)->setBallRadius(model_settings_->getBallRadius());
			break;
			
		case MODEL_SE_SURFACE:
			{
				AddSurfaceModel* surface_model = new AddSurfaceModel;
				surface_model->setType(SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE);	
				surface_model->setProbeRadius(model_settings_->getSurfaceProbeRadius());
				model_processor = surface_model;
			}
			break;
			
		case MODEL_SA_SURFACE:
			{
				AddSurfaceModel* surface_model = new AddSurfaceModel;
				surface_model->setType(SurfaceProcessor::SOLVENT_ACCESSIBLE_SURFACE);
				surface_model->setProbeRadius(model_settings_->getSurfaceProbeRadius());
				model_processor = surface_model;
			}
			break;
			
		case MODEL_VDW:
			model_processor = new AddVanDerWaalsModel;
			((AddVanDerWaalsModel*) model_processor)->setVDWRadiusFactor(model_settings_->getVDWRadiusFactor());
			break;

		case MODEL_BACKBONE:
			model_processor = new AddBackboneModel;
			((AddBackboneModel*) model_processor)->setTubeRadius(model_settings_->getTubeRadius());
			break;

		case MODEL_CARTOON:
			model_processor = new AddCartoonModel;
			((AddCartoonModel*) model_processor)->setTubeRadius(model_settings_->getCartoonTubeRadius());
			((AddCartoonModel*) model_processor)->setHelixRadius(model_settings_->getCartoonHelixRadius());
			((AddCartoonModel*) model_processor)->setArrowWidth(model_settings_->getCartoonArrowWidth());
			((AddCartoonModel*) model_processor)->setArrowHeight(model_settings_->getCartoonArrowHeight());
			break;
			
		case MODEL_HBONDS:
			model_processor = new HBondModelProcessor;
    	((HBondModelProcessor*) model_processor)->setRadius(model_settings_->getHBondsRadius());
			break;
			
		default:
			throw(InvalidOption(__FILE__, __LINE__, model_type_combobox->currentItem()));
	}

	ColorProcessor* color_processor;

	switch(coloring_method_combobox->currentItem())
	{
		case COLORING_ELEMENT:
			color_processor = new ElementColorProcessor;
			break;

		case COLORING_RESIDUE_NAME:
			color_processor = new ResidueNameColorProcessor;
			break;

		case COLORING_RESIDUE_INDEX:
			color_processor = new ResidueNumberColorProcessor;
			break;

		case COLORING_SECONDARY_STRUCTURE:
			color_processor = new SecondaryStructureColorProcessor;
			break;

		case COLORING_ATOM_CHARGE:
			color_processor = new AtomChargeColorProcessor;
			break;

		case COLORING_CUSTOM:
		{
			color_processor = new CustomColorProcessor;
			break;
		}

		case COLORING_DISTANCE:
			color_processor = new AtomDistanceColorProcessor;
			break;

		case COLORING_TEMPERATURE_FACTOR:
			color_processor = new TemperatureFactorColorProcessor;
			break;

		case COLORING_OCCUPANCY:
			color_processor = new OccupancyColorProcessor;
			break;


		default:
			throw(InvalidOption(__FILE__, __LINE__, coloring_method_combobox->currentItem()));
	}
			

	QColor qcolor = color_sample->backgroundColor();
	custom_color_.set(qcolor.red(),
										qcolor.green(),
										qcolor.blue(),
										255 - (Position)(transparency_slider->value() * 2.55));
	color_processor->setDefaultColor(custom_color_);

	bool rebuild_representation = false;
	PrimitiveManager& pm = getMainControl()->getPrimitiveManager();
	Representation* rep = 0;
	if (rep_ == 0)
	{
		// create a new Representation
		rep = new Representation(model_type_combobox->currentItem(), 
														 precision_combobox->currentItem(), 
														 mode_combobox->currentItem());
		rebuild_representation = true;
		if (custom_precision_button->isChecked())
		{
			rep->setSurfaceDrawingPrecision(((float)precision_slider->value()) / 10.0);
		}
	}
	else 
	{
		// modify an existing Representation
		// If the represenation has the same ModelProcessor as the currently selected,
		// we dont have to rebuild the geometric objects.
		if (rep_->getModelType() != model_type_combobox->currentItem() ||
				rep_->getDrawingPrecision() != precision_combobox->currentItem() ||
				 (custom_precision_button->isChecked() &&
				  rep_->getSurfaceDrawingPrecision() != (float)precision_slider->value() / 10.0)||
				 (!custom_precision_button->isChecked() &&
				  rep_->getSurfaceDrawingPrecision() != -1) ||
				 advanced_options_modified_)
		{
			rebuild_representation = true;
		}
		rep = rep_;
	}

	rep->setColorProcessor(color_processor);
	rep->setModelProcessor(model_processor);
	rep->setTransparency(Size((float)transparency_slider->value() * 2.55));
	
	if (rep_ == 0) 
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
		rep_->setModelType(model_type_combobox->currentItem());
		if (custom_precision_button->isChecked())
		{
			rep_->setSurfaceDrawingPrecision(((float)precision_slider->value()) / 10.0);
		}
		else
		{
			rep_->setSurfaceDrawingPrecision(-1);
			rep_->setDrawingPrecision(precision_combobox->currentItem());
		}
		rep_->setDrawingMode(mode_combobox->currentItem());
	}

	rep->setColoringType(coloring_method_combobox->currentItem());
	rep->update(rebuild_representation);

	// no refocus, if a representation already exists
	bool focus = (getMainControl()->getPrimitiveManager().getRepresentations().size() == 1 && rep_ == 0);

	RepresentationMessage* message = new RepresentationMessage;
	if (rep_ == 0)
	{
		message->setType(RepresentationMessage::ADD);
	}
	else
	{
		message->setType(RepresentationMessage::UPDATE);
	}
	message->setRepresentation(rep);
	notify_(message);

	if (focus)
	{
		CompositeMessage* ccmessage = new CompositeMessage;
		ccmessage->setComposite(composite);
		ccmessage->setType(CompositeMessage::CENTER_CAMERA);
		notify_(ccmessage);
	}

	advanced_options_modified_ = false;
}


void DisplayProperties::transparencySliderChanged()
{
	String text = String(transparency_slider->value());
	text += String("%");
	transparency_label->setText(text.c_str());
}

void DisplayProperties::precisionSliderChanged()
{
	String text = String(((float)precision_slider->value()) / 10.0).trimRight("0");
	if (text.right(1) == ".") text = text + "0";

	custom_precision_label->setText(text.c_str());
	custom_precision_button->setChecked(true);
}

void DisplayProperties::coloringOptionsPressed()
{
	if (preferences_ == 0) return;

	preferences_->show();
	//preferences_->setCurrentPage(preferences_->indexOf(coloring_settings_));
	preferences_->showPage(coloring_settings_);
}

void DisplayProperties::modelOptionsPressed()
{
	if (preferences_ == 0) return;

	preferences_->show();
	//preferences_->setCurrentPage(preferences_->indexOf(model_settings_));
	preferences_->showPage(model_settings_);
}

void DisplayProperties::precisionBoxChanged(int index)
{
	presets_precision_button->setChecked(true);

	if (index > DRAWING_PRECISION_HIGH)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	switch (index)
	{
		case VIEW::DRAWING_PRECISION_LOW:
			presets_precision_label->setText("1.5");
			break;

		case VIEW::DRAWING_PRECISION_MEDIUM:
			presets_precision_label->setText("3.5");
			break;

		case VIEW::DRAWING_PRECISION_HIGH:
			presets_precision_label->setText("6.5");
			break;

		default:
			Log.error() << "Unknown precision in " << __FILE__ << "   " << __LINE__ << std::endl;
	}
}

void DisplayProperties::checkDrawingPrecision_()
	throw()
{
	if (model_type_combobox->currentItem() != MODEL_SE_SURFACE && 
			model_type_combobox->currentItem() != MODEL_SA_SURFACE)
	{
		presets_precision_button->setChecked(true);
		custom_precision_button->setEnabled(false);
		precision_slider->setEnabled(false);
	}
	else
	{
		precision_slider->setEnabled(true);
		custom_precision_button->setEnabled(true);
		if (rep_ != 0 &&
				rep_->getSurfaceDrawingPrecision() != -1)
		{
			custom_precision_button->setChecked(true);
			precision_slider->setValue((Position)(rep_->getSurfaceDrawingPrecision() * 10.0));
		}
		else
		{
			presets_precision_button->setChecked(true);
		}
	}
}

void DisplayProperties::getAdvancedOptions_()
	throw()
{
	if (rep_ == 0) return;
	
	ModelProcessor* mp = rep_->getModelProcessor();

	if (mp == 0) return;
	
	switch (model_type_combobox->currentItem())
	{
		case MODEL_LINES:
			break;
			
		case MODEL_STICK:
			model_settings_->setStickStickRadius(((AddBallAndStickModel*)mp)->getStickRadius());
			break;
			
		case MODEL_BALL_AND_STICK:
			model_settings_->setBallAndStickStickRadius(((AddBallAndStickModel*)mp)->getStickRadius());
			model_settings_->setBallRadius(((AddBallAndStickModel*)mp)->getBallRadius());
			break;
			
		case MODEL_SE_SURFACE:
			{
				model_settings_->setSurfaceProbeRadius(((AddSurfaceModel*)mp)->getProbeRadius());
			}
			break;
			
		case MODEL_SA_SURFACE:
			{
				model_settings_->setSurfaceProbeRadius(((AddSurfaceModel*)mp)->getProbeRadius());
			}
			break;
			
		case MODEL_VDW:
			model_settings_->setVDWRadiusFactor(((AddVanDerWaalsModel*) mp)->getVDWRadiusFactor());
			break;

		case MODEL_BACKBONE:
			model_settings_->setTubeRadius(((AddBackboneModel*) mp)->getTubeRadius());
			break;

		case MODEL_CARTOON:
			model_settings_->setCartoonTubeRadius(((AddCartoonModel*) mp)->getTubeRadius());
			model_settings_->setCartoonHelixRadius(((AddCartoonModel*) mp)->getHelixRadius());
			model_settings_->setCartoonArrowWidth(((AddCartoonModel*) mp)->getArrowWidth());
			model_settings_->setCartoonArrowHeight(((AddCartoonModel*) mp)->getArrowHeight());
			break;
			
		case MODEL_HBONDS:
    	model_settings_->setHBondRadius(((HBondModelProcessor*) mp)->getRadius());
			break;
	}
}
			
void DisplayProperties::applyPreferences(Preferences& /*preferences*/)
	throw()
{
	advanced_options_modified_ = true;
}


} } // namespaces
