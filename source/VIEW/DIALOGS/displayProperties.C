// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.C,v 1.68 2004/07/05 10:11:46 amoll Exp $
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
#include <qlistbox.h>

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
		advanced_options_modified_(false),
		create_representations_for_new_molecules_(true)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DisplayProperties " << this << std::endl;
#endif
	ModularWidget::registerWidget(this);

	model_type_combobox->clear();
	for (Index p = 0; p < MODEL_LABEL; p++)
	{
		model_type_combobox->insertItem(VIEW::getModelName((VIEW::ModelType)p).c_str());
	}

	coloring_method_combobox->clear();
	for (Index p = 0; p < COLORING_UNKNOWN; p++)
	{
		coloring_method_combobox->insertItem(VIEW::getColoringName((VIEW::ColoringMethod)p).c_str());
	}
}

DisplayProperties::DisplayProperties(const DisplayProperties& /*dp*/)
	throw()
	: DisplayPropertiesData(),
		ModularWidget(*this)
{
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
		color_sample->setBackgroundColor(custom_color_.getQColor());
	}

	if (inifile.hasEntry("REPRESENTATION", "selected_color"))
	{
		BALL_SELECTED_COLOR.set(inifile.getValue("REPRESENTATION", "selected_color"));
		color_sample_selection->setBackgroundColor(BALL_SELECTED_COLOR.getQColor());
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

	if (coloring_settings_ != 0)
	{
		coloring_settings_->fetchPreferences(inifile);
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
	custom_color_.set(qcolor);
	custom_color_.setAlpha(255 - (Position) (transparency_slider->value() * 2.55));

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

	if (coloring_settings_ != 0)
	{
		coloring_settings_->writePreferences(inifile);
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
	preferences.insertPage(model_settings_, "Models");
	coloring_settings_ = new ColoringSettingsDialog(this);
	preferences.insertPage(coloring_settings_, "Model Colors");
}

void DisplayProperties::finalizePreferencesTab(Preferences &preferences)
	throw()
{
	if (model_settings_) 
	{
		preferences.removePage(model_settings_);
		delete model_settings_;
		model_settings_ = 0;
	}
	if (coloring_settings_) 
	{
		preferences.removePage(coloring_settings_);
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
	apply_button->setText("Create");
	apply_button->setEnabled(getMainControl()->getMolecularControlSelection().size());
}

void DisplayProperties::modifyRepresentationMode()
{
	setCaption("modify Representation");
	apply_button->setText("Modify");
	if (rep_->getColoringMethod() != COLORING_UNKNOWN)
	{
		coloring_method_combobox->setCurrentItem(rep_->getColoringMethod());
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
		color_sample->setBackgroundColor(custom_color_.getQColor());
	}

	transparency_slider->setValue((Size)(rep_->getTransparency() / 2.55));

	checkDrawingPrecision_();
	getAdvancedModelOptions_();
	getAdvancedColoringOptions_();

	apply_button->setEnabled(true);
}

void DisplayProperties::selectModel(int index)
{
	if (index >= MODEL_LABEL)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	// enable usage from python
	if (index != model_type_combobox->currentItem())
	{
		model_type_combobox->setCurrentItem(index);
		return;
	}

	checkDrawingPrecision_();
}

void DisplayProperties::selectMode(int index)
{
	if (index > VIEW::DRAWING_MODE_SOLID)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	// enable usage from python
	if (index != mode_combobox->currentItem())
	{
		mode_combobox->setCurrentItem(index);
		return;
	}

	transparency_slider->setEnabled(index == VIEW::DRAWING_MODE_SOLID);
}

void DisplayProperties::selectColoringMethod(int index)
{
	if (index > COLORING_CUSTOM)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}


	// enable usage from python
	if (index != coloring_method_combobox->currentItem())
	{
		coloring_method_combobox->setCurrentItem(index);
		return;
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
		if (composite_message->getType() != CompositeMessage::NEW_MOLECULE ||
				!create_representations_for_new_molecules_) 
		{
			return;
		}
		// generate graphical representation
		List<Composite*> clist;
		clist.push_back(composite_message->getComposite());
		createRepresentation_(clist);
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
				Representation* rep = ((RepresentationMessage*) message)->getRepresentation();
				if (rep->getModelType() >= MODEL_LABEL) return;
				rep_ = rep;
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
		apply_button->setEnabled(getMainControl()->getMolecularControlSelection().size());
		return;
	}

	if (RTTI::isKindOf<CreateRepresentationMessage>(*message))
	{
		CreateRepresentationMessage* crm = (CreateRepresentationMessage*) message;
		if (crm->getComposites().size() == 0) return;
		model_type_combobox->setCurrentItem(crm->getModelType());
		coloring_method_combobox->setCurrentItem(crm->getColoringMethod());
		createRepresentationMode();
		createRepresentation_(crm->getComposites());
	}
}


void DisplayProperties::applyButtonClicked()
{
	// no molecular or representation selection present 
	if (getMainControl()->getMolecularControlSelection().size() == 0 && rep_ == 0)
	{
		return;
	}

	setStatusbarText("building model...");
	createRepresentation_(getMainControl()->getMolecularControlSelection());
}


void DisplayProperties::editColor()
{
	color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
	custom_color_.set(color_sample->backgroundColor());
	update();
}

void DisplayProperties::editSelectionColor()
{
	color_sample_selection->setBackgroundColor(QColorDialog::getColor(
													color_sample_selection->backgroundColor()));
	BALL_SELECTED_COLOR.set(color_sample_selection->backgroundColor());
	update();
}

// ------------------------------------------------------------------------
// Model Processor methods
// ------------------------------------------------------------------------

void DisplayProperties::createRepresentation_(const List<Composite*>& composites)
	throw(InvalidOption)
{
	ModelProcessor* model_processor = 0;

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

	ColorProcessor* color_processor = 0;

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

	if (color_processor != 0)
	{
		coloring_settings_->applySettingsTo(*color_processor);
	}
			

	QColor qcolor = color_sample->backgroundColor();
	custom_color_.set(qcolor);
	custom_color_.setAlpha(255 - (Position)(transparency_slider->value() * 2.55));
	color_processor->setDefaultColor(custom_color_);

	bool rebuild_representation = false;
	Representation* rep = 0;
	if (rep_ == 0)
	{
		// create a new Representation
		rep = new Representation((ModelType)model_type_combobox->currentItem(), 
														 (DrawingPrecision)precision_combobox->currentItem(), 
														 (DrawingMode)mode_combobox->currentItem());
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
			if (rep_->getColorProcessor() != 0)
			{
				coloring_settings_->applySettingsTo(*rep_->getColorProcessor());
			}
		}
		rep = rep_;
	}

	rep->setColorProcessor(color_processor);
	rep->setModelProcessor(model_processor);
	rep->setTransparency(Size((float)transparency_slider->value() * 2.55));
	
	if (rep_ == 0) 
	{	
#ifndef BALL_QT_HAS_THREADS
 		getMainControl()->getPrimitiveManager().insert(*rep);
#endif
		
		List<Composite*>::ConstIterator it = composites.begin();
		for (; it != composites.end(); it++)
		{
			rep->getComposites().insert(*it);
		}
	}
	else
	{
		rep_->setModelType((ModelType)model_type_combobox->currentItem());
		if (custom_precision_button->isChecked())
		{
			rep_->setSurfaceDrawingPrecision(((float)precision_slider->value()) / 10.0);
		}
		else
		{
			rep_->setSurfaceDrawingPrecision(-1);
			rep_->setDrawingPrecision((DrawingPrecision) precision_combobox->currentItem());
		}
		rep_->setDrawingMode((DrawingMode)mode_combobox->currentItem());
	}

	rep->setColoringMethod((ColoringMethod)coloring_method_combobox->currentItem());

	apply_button->setEnabled(false);
	rep->update(rebuild_representation);
	apply_button->setEnabled(true);
	setStatusbarText("Done.");

	// no refocus, if a representation already exists
	bool focus = (getMainControl()->getPrimitiveManager().getRepresentations().size() == 1 && rep_ == 0);

#ifndef BALL_QT_HAS_THREADS	
	setStatusbarText("drawing representation...");
	RepresentationMessage* message = new RepresentationMessage;
	if (rep_ == 0)
	{
		message->setType(RepresentationMessage::ADD);
	}
	else
	{
		message->setType(RepresentationMessage::UPDATE);
	}
	message->setRepresentation(*rep);
	notify_(message);
#endif

	if (focus && composites.size() > 0)
	{
		CompositeMessage* ccmessage = new CompositeMessage;
		ccmessage->setComposite(**composites.begin());
		ccmessage->setType(CompositeMessage::CENTER_CAMERA);
		notify_(ccmessage);
	}

	advanced_options_modified_ = false;
	setStatusbarText("finished drawing");
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

	preferences_->showPage(coloring_settings_);

	switch (coloring_method_combobox->currentItem())
	{
		case COLORING_ELEMENT:
			coloring_settings_->list_box->setCurrentItem(0);
			break;

		case COLORING_RESIDUE_NAME:
			coloring_settings_->list_box->setCurrentItem(2);
			break;

		case COLORING_RESIDUE_INDEX:
			coloring_settings_->list_box->setCurrentItem(1);
			break;

		case COLORING_SECONDARY_STRUCTURE:
			coloring_settings_->list_box->setCurrentItem(7);
			break;

		case COLORING_ATOM_CHARGE:
			coloring_settings_->list_box->setCurrentItem(3);
			break;

		case COLORING_CUSTOM:
			break;

		case COLORING_DISTANCE:
			coloring_settings_->list_box->setCurrentItem(4);
			break;

		case COLORING_TEMPERATURE_FACTOR:
			coloring_settings_->list_box->setCurrentItem(5);
			break;

		case COLORING_OCCUPANCY:
			coloring_settings_->list_box->setCurrentItem(6);
			break;
	}

	preferences_->show();
}

void DisplayProperties::modelOptionsPressed()
{
	if (preferences_ == 0) return;

	preferences_->showPage(model_settings_);

	switch (model_type_combobox->currentItem())
	{
		case MODEL_LINES:
			break;
			
		case MODEL_STICK:
			model_settings_->showPage(0);
			break;
			
		case MODEL_BALL_AND_STICK:
			model_settings_->showPage(1);
			break;
			
		case MODEL_SE_SURFACE:
		case MODEL_SA_SURFACE:
			model_settings_->showPage(3);
			break;
			
		case MODEL_VDW:
			model_settings_->showPage(2);
			break;

		case MODEL_BACKBONE:
			model_settings_->showPage(4);
			break;

		case MODEL_CARTOON:
			model_settings_->showPage(5);
			break;
			
		case MODEL_HBONDS:
			model_settings_->showPage(6);
			break;
	}

	preferences_->show();
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
	if (!modelMuteableByDisplayProperties((ModelType)model_type_combobox->currentItem()))
	{
		return;
	}

	if (!isSurfaceModel((ModelType)model_type_combobox->currentItem()))
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

void DisplayProperties::getAdvancedColoringOptions_()
	throw()
{
	if (rep_ == 0) return;

	ColorProcessor* cp = rep_->getColorProcessor();
	if (cp == 0 ||
		  coloring_method_combobox->currentItem() == COLORING_CUSTOM)
	{
		return;
	}

	coloring_settings_->applySettingsTo(*cp);
}

void DisplayProperties::getAdvancedModelOptions_()
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
			
void DisplayProperties::applyPreferences()
	throw()
{
	advanced_options_modified_ = true;
}

void DisplayProperties::defaultPreferences()
	throw()
{
	if (model_settings_ != 0) model_settings_->setDefaultValues();
	if (coloring_settings_ != 0) coloring_settings_->setDefaultValues();
}

bool DisplayProperties::getSettingsFromString(const String& data)
	throw()
{
	vector<String> fields;
	if (data.split(fields) < 6) return false;
	
	try
	{
		selectMode(fields[0].toUnsignedInt());
		setDrawingPrecision(fields[1].toUnsignedInt());
		setSurfaceDrawingPrecision(fields[2].toFloat());
		selectModel(fields[3].toUnsignedInt());
		selectColoringMethod(fields[4].toUnsignedInt());
		setTransparency((Position)(fields[5].toFloat() / 2.55));
	}
	catch(...)
	{
		return false;
	}

	return true;
	}


	void DisplayProperties::setSurfaceDrawingPrecision(float value)
	{
		precision_slider->setValue((int)(value * 10.0));
	}
			
	void DisplayProperties::setDrawingPrecision(int value)
	{
		precision_combobox->setCurrentItem(value);
	}

	void DisplayProperties::setTransparency(int value)
	{
		transparency_slider->setValue(value);
	}
	
} } // namespaces
