// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.C,v 1.31 2003/11/02 14:12:24 amoll Exp $
//

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
		id_(-1),
		precision_(DRAWING_PRECISION_HIGH),
		mode_(DRAWING_MODE_SOLID),
		coloring_method_(COLORING_ELEMENT),
		model_type_(MODEL_BALL_AND_STICK),
		rep_(0)
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
}


void DisplayProperties::fetchPreferences(INIFile& inifile)
	throw()
{
	ModularWidget::fetchPreferences(inifile);
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

	if (inifile.hasEntry("WINDOWS", "Display::surface_precision"))
	{
		precision_slider->setValue((Position)inifile.getValue("WINDOWS", "Display::surface_precision").toFloat() * 10);
		custom_precision_button->setChecked(true);
	}
	else
	{ 
		presets_precision_button->setChecked(true);
		precision_slider->setValue(12 * 10);
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
	ModularWidget::writePreferences(inifile);
	const QColor& qcolor = color_sample->backgroundColor();
	custom_color_.set(qcolor.red(),
										qcolor.green(),
										qcolor.blue(),
										255 - (Position) (transparency_slider->value() * 2.55));

	// the combobox values
	inifile.insertValue("WINDOWS", "Display::model", model_type_);
	inifile.insertValue("WINDOWS", "Display::precision", precision_);
	inifile.insertValue("WINDOWS", "Display::surface_precision", (float)precision_slider->value() / (float)10);
	inifile.insertValue("WINDOWS", "Display::coloring_method", coloring_method_);
	inifile.insertValue("WINDOWS", "Display::custom_color", custom_color_);
	inifile.insertValue("WINDOWS", "Display::selected_color", BALL_SELECTED_COLOR);
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
	coloring_method_combobox->setCurrentItem(coloring_method_);
	precision_combobox->setCurrentItem(precision_);
	model_type_combobox->setCurrentItem(model_type_);
	mode_combobox->setCurrentItem(mode_);
	apply_button->setEnabled(getMainControl()->getControlSelection().size());
	checkDrawingPrecision_();
}

void DisplayProperties::modifyRepresentationMode()
{
	setCaption("modify Representation");
	coloring_method_combobox->setCurrentItem(rep_->getColoringType());
	precision_combobox->setCurrentItem(rep_->getDrawingPrecision());
	model_type_combobox->setCurrentItem(rep_->getModelType());
	mode_combobox->setCurrentItem(rep_->getDrawingMode());

	custom_color_ = rep_->getColorProcessor()->getDefaultColor();
	QColor qcolor(custom_color_.getRed(), custom_color_.getGreen(), custom_color_.getBlue());
	color_sample->setBackgroundColor(qcolor);

	transparency_slider->setValue((Size)(rep_->getTransparency() / 2.55));

	checkDrawingPrecision_();

	apply_button->setEnabled(true);
}

void DisplayProperties::selectModel(int index)
{
	if (index >= MODEL_LABEL)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	model_type_ = index;
	checkDrawingPrecision_();
}

void DisplayProperties::selectMode(int index)
{
	if (index > VIEW::DRAWING_MODE_SOLID)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	transparency_slider->setEnabled(index == VIEW::DRAWING_MODE_SOLID);

	mode_ = index;
}

void DisplayProperties::selectColoringMethod(int index)
{
	if (index > COLORING_CUSTOM)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}

	coloring_method_ = index;
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
			
		case MODEL_SE_SURFACE:
			{
				AddSurfaceModel* surface_model = new AddSurfaceModel;
				surface_model->setType(SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE);	
				model_processor = surface_model;
			}
			break;
			
		case MODEL_SA_SURFACE:
			{
				AddSurfaceModel* surface_model = new AddSurfaceModel;
				surface_model->setType(SurfaceProcessor::SOLVENT_ACCESSIBLE_SURFACE);
				model_processor = surface_model;
			}
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
			
		case MODEL_HBONDS:
			model_processor = new HBondModelProcessor;
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

		case COLORING_RESIDUE_INDEX:
			color_processor = new ResidueNumberColorProcessor;
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
			throw(InvalidOption(__FILE__, __LINE__, coloring_method_));
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
		rep = new Representation(model_type_, precision_, mode_);
		rebuild_representation = true;
	}
	else 
	{
		// modify an existing Representation
		// If the represenation has the same ModelProcessor as the currently selected,
		// we dont have to rebuild the geometric objects.
		if (rep_->getModelType() != model_type_ ||
				rep_->getDrawingPrecision() != precision_ ||
				 (custom_precision_button->isChecked() &&
				  rep_->getSurfaceDrawingPrecision() != (float)precision_slider->value() / (float)10)||
				 (!custom_precision_button->isChecked() &&
				  rep_->getSurfaceDrawingPrecision() != -1))
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
		rep_->setModelType(model_type_);
		rep_->setDrawingPrecision(precision_);
		if (custom_precision_button->isChecked())
		{
			rep_->setSurfaceDrawingPrecision((float)precision_slider->value() / (float) 10);
		}
		else
		{
			rep_->setSurfaceDrawingPrecision(-1);
		}
		rep_->setDrawingMode(mode_);
	}

	rep->setColoringType(coloring_method_);
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
}


void DisplayProperties::transparencySliderChanged()
{
	String text = String(transparency_slider->value());
	text += String("%");
	transparency_label->setText(text.c_str());
}

void DisplayProperties::precisionSliderChanged()
{
	String text = String((float)precision_slider->value() / 10.0).trimRight("0");
	if (text.right(1) == ".") text = text + "0";

	custom_precision_label->setText(text.c_str());
	custom_precision_button->setChecked(true);
}

void DisplayProperties::coloringOptionsPressed()
{
}

void DisplayProperties::modelOptionsPressed()
{
}

void DisplayProperties::precisionBoxChanged(int index)
{
	presets_precision_button->setChecked(true);

	if (index > DRAWING_PRECISION_HIGH)
	{
		throw(InvalidOption(__FILE__, __LINE__, index));
	}
	precision_ = index;

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
	if (model_type_ != MODEL_SE_SURFACE && 
			model_type_ != MODEL_SA_SURFACE)
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
			precision_slider->setValue((Position)(rep_->getSurfaceDrawingPrecision() * 10));
		}
		else
		{
			presets_precision_button->setChecked(true);
		}
	}
}


} } // namespaces
