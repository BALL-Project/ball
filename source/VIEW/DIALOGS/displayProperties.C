// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.C,v 1.97.2.21 2005/10/17 00:43:41 amoll Exp $
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
#include <BALL/VIEW/MODELS/forceModel.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>

#include <qmenubar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qradiobutton.h>
#include <qlistbox.h>
#include <qbuttongroup.h>

namespace BALL
{
	namespace VIEW
	{

DisplayProperties::DisplayProperties(QWidget* parent, const char* name)
	throw()
	:	DisplayPropertiesData( parent, name ),
		ModularWidget(name),
		PreferencesEntry(),
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

	createRepresentationMode();

	setINIFileSectionName("REPRESENTATION");
	registerObject_(model_type_combobox);
	registerObject_(precision_combobox);
	registerObject_(precision_slider);
	registerObject_(resolution_group);
	registerObject_(mode_combobox);
	registerObject_(coloring_method_combobox);
	registerObject_(selection_color_label); 
	registerObject_(custom_color_label);
	registerObject_(transparency_slider);
}

DisplayProperties::DisplayProperties(const DisplayProperties& /*dp*/)
	throw()
	: DisplayPropertiesData(),
		ModularWidget(*this),
		PreferencesEntry()
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


void DisplayProperties::initializeWidget(MainControl& main_control)
	throw()
{
	(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

	id_ = insertMenuEntry(MainControl::DISPLAY, "D&isplay Properties", this, 
																		 SLOT(show()), CTRL+Key_I);   
	setMenuHint("Create a new representation or modify an existing one");
	setMenuHelp("displayProperties.html");

	registerWidgetForHelpSystem(this, "displayProperties.html");
}


void DisplayProperties::initializePreferencesTab(Preferences &preferences)
	throw()
{
	preferences_ = &preferences;
	model_settings_ = new ModelSettingsDialog(this, "ModelSettings");
	preferences.insertEntry(model_settings_);
	coloring_settings_ = new ColoringSettingsDialog(this, "ColoringSettings");
	preferences.insertEntry(coloring_settings_);
}

void DisplayProperties::finalizePreferencesTab(Preferences &preferences)
	throw()
{
	if (model_settings_) 
	{
		preferences.removeEntry(model_settings_);
		model_settings_ = 0;
	}
	if (coloring_settings_) 
	{
		preferences.removeEntry(coloring_settings_);
		coloring_settings_ = 0;
	}
}


void DisplayProperties::checkMenu(MainControl& main_control)
	throw()
{
	(main_control.menuBar())->setItemChecked(id_, isVisible());
	(main_control.menuBar())->setItemEnabled(id_, !getMainControl()->compositesAreLocked());
}


void DisplayProperties::show()
{
	checkDrawingPrecision_();
	DisplayPropertiesData::show();
	raise();
}

void DisplayProperties::createRepresentationMode()
{
	rep_ = 0;
	setCaption("create Representation");
	apply_button->setText("Create");
	apply_button->setEnabled(getMainControl()->getMolecularControlSelection().size());

	model_updates_enabled->setChecked(true);
	coloring_updates_enabled->setChecked(true);
}

void DisplayProperties::modifyRepresentationMode(Representation* rep)
{
	if (rep == 0) return;
	
	rep_ = rep;
	
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
		custom_color_label->setBackgroundColor(custom_color_.getQColor());
	}

	transparency_slider->setValue((Size)(rep_->getTransparency() / 2.55));

	coloring_updates_enabled->setChecked(rep_->coloringUpdateEnabled());
	model_updates_enabled->setChecked(rep_->modelUpdateEnabled());

	getAdvancedModelOptions_();
	getAdvancedColoringOptions_();
	checkDrawingPrecision_();

	apply_button->setEnabled(true);
}

void DisplayProperties::selectModel(int index)
{
	if (index >= MODEL_LABEL)
	{
		throw(Exception::InvalidOption(__FILE__, __LINE__, index));
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
		throw(Exception::InvalidOption(__FILE__, __LINE__, index));
	}

	// enable usage from python
	if (index != mode_combobox->currentItem())
	{
		mode_combobox->setCurrentItem(index);
		return;
	}
}

void DisplayProperties::selectColoringMethod(int index)
{
	if (index > COLORING_CUSTOM)
	{
		throw(Exception::InvalidOption(__FILE__, __LINE__, index));
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
		if (!create_representations_for_new_molecules_ ||
				composite_message->getType() != CompositeMessage::NEW_MOLECULE)
		{
			return;
		}

		// generate graphical representation
		createRepresentationMode();
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
				modifyRepresentationMode(rep);
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


void DisplayProperties::apply()
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
	custom_color_.set(chooseColor(custom_color_label));
}

void DisplayProperties::editSelectionColor()
{
	BALL_SELECTED_COLOR.set(chooseColor(selection_color_label));
}

// ------------------------------------------------------------------------
// Model Processor methods
// ------------------------------------------------------------------------
void DisplayProperties::applyModelSettings_(Representation& rep)
{
	ModelType current_type = (ModelType) model_type_combobox->currentItem();
	if (rep.getModelProcessor() == 0 ||
			rep.getModelType() != current_type ||
			!rep.modelUpdateEnabled())
	{
		rep.setModelProcessor(model_settings_->createModelProcessor(current_type));
		rep.setModelType((ModelType)model_type_combobox->currentItem());
	}

	if (custom_precision_button->isChecked())
	{
		rep.setSurfaceDrawingPrecision(((float)precision_slider->value()) / 10.0);
	}
	else
	{
		rep.setDrawingPrecision((DrawingPrecision) precision_combobox->currentItem());
	}

	model_settings_->applySettingsTo(*rep.getModelProcessor());
}


void DisplayProperties::applyColoringSettings_(Representation& rep)
{
	ColoringMethod current_coloring = (ColoringMethod) coloring_method_combobox->currentItem();

 	if (rep.getColorProcessor() == 0 ||
 			rep.getColoringMethod() != current_coloring ||
 			!rep.coloringUpdateEnabled())
	{
		rep.setColorProcessor(coloring_settings_->createColorProcessor(current_coloring));
		rep.setColoringMethod(current_coloring);
	}

	custom_color_.set(custom_color_label->backgroundColor());
	custom_color_.setAlpha(255 - (Position)(transparency_slider->value() * 2.55));

	ColorProcessor* cp = rep.getColorProcessor();
	coloring_settings_->applySettingsTo(*cp);
	cp->setDefaultColor(custom_color_);
}


Representation* DisplayProperties::createRepresentation_(const List<Composite*>& composites)
	throw(Exception::InvalidOption)
{
	bool rebuild_representation = false;
	bool new_representation = (rep_ == 0);

	if (new_representation)
	{
		// create a new Representation
		rep_ = new Representation((ModelType)model_type_combobox->currentItem(), 
														 (DrawingPrecision)precision_combobox->currentItem(), 
														 (DrawingMode)mode_combobox->currentItem());
		rebuild_representation = true;

		if (custom_precision_button->isChecked())
		{
			rep_->setSurfaceDrawingPrecision((float)precision_slider->value() / 10.0);
		}

		List<const Composite*> temp_composites;

		List<Composite*>::ConstIterator it = composites.begin();
		for (; it != composites.end(); it++)
		{
			temp_composites.push_back(*it);
		}
		rep_->setComposites(temp_composites);

		// this is not straight forward, but we have to prevent a second rendering run in the Scene...
		// the insertion into the PrimitiveManager is needed to allow the Representation::update
		getMainControl()->getPrimitiveManager().insert(*rep_, false);
		
		// now we can add the Representation to the GeometricControl
		notify_(new RepresentationMessage(*rep_, RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL));

		// no refocus, if a this is not the only Representation
		if ((getMainControl()->getPrimitiveManager().getRepresentations().size() < 2) && 
				composites.size() > 0)
		{
			CompositeMessage* ccmessage = new CompositeMessage;
			ccmessage->setComposite(**composites.begin());
			ccmessage->setType(CompositeMessage::CENTER_CAMERA);
			notify_(ccmessage);
		}
	}
	else
	{
		rebuild_representation = 
			(rep_->getModelType() != model_type_combobox->currentItem() ||
			 advanced_options_modified_);

		if (custom_precision_button->isChecked())
		{
			// workaround, didnt work right otherwise: (just let it this way)
			rebuild_representation |= 
				(String(rep_->getSurfaceDrawingPrecision()) != 
				 String(((float)precision_slider->value() / 10.0)));
		}
		else
		{
			rebuild_representation |= 
 				(rep_->getDrawingPrecision() != (DrawingPrecision) precision_combobox->currentItem());
		}
	}

	Size transparency = (Size)(transparency_slider->value() * 2.55);

	if (coloring_updates_enabled->isChecked())
	{
		applyColoringSettings_(*rep_);
	}
	else
	{
		if (rep_->getTransparency() != transparency &&
				!model_updates_enabled->isChecked())
		{
			Representation::GeometricObjectList::iterator it = rep_->getGeometricObjects().begin();
			for (; it != rep_->getGeometricObjects().end(); it++)
			{
				if (RTTI::isKindOf<Mesh> (**it))
				{
					Mesh* mesh = dynamic_cast<Mesh*> (*it);

					for (Position p = 0; p < mesh->colors.size(); p++)
					{
						mesh->colors[p].setAlpha(255 - transparency);
					}
				}
				else
				{
					(**it).getColor().setAlpha(255 - transparency);
				}
			}
		}
	}

	if (rebuild_representation && model_updates_enabled->isChecked())
	{
		applyModelSettings_(*rep_);
		advanced_options_modified_ = false;
	}

	rep_->setDrawingMode((DrawingMode)  mode_combobox->currentItem());

	rep_->setTransparency(transparency);

	rep_->enableModelUpdate(model_updates_enabled->isChecked());
	rep_->enableColoringUpdate(coloring_updates_enabled->isChecked());

	apply_button->setEnabled(false);
	rep_->update(rebuild_representation);
	apply_button->setEnabled(true);

	return rep_;
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

	preferences_->showEntry(coloring_settings_->getEntryFor((ColoringMethod) coloring_method_combobox->currentItem()));
	preferences_->show();
}

void DisplayProperties::modelOptionsPressed()
{
	if (preferences_ == 0) return;

	preferences_->showEntry(model_settings_->getEntryFor((ModelType) model_type_combobox->currentItem()));
	preferences_->show();
}

void DisplayProperties::precisionBoxChanged(int index)
{
	presets_precision_button->setChecked(true);

	if (index < DRAWING_PRECISION_LOW ||
			index > DRAWING_PRECISION_ULTRA)
	{
		throw(Exception::InvalidOption(__FILE__, __LINE__, index));
	}

	String label = createFloatString(SurfaceDrawingPrecisions[index], 1);
	presets_precision_label->setText(label.c_str());
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
		precision_slider->setEnabled(model_updates_enabled->isChecked());
		custom_precision_button->setEnabled(true);
		if (rep_ == 0) return;
		
		if (rep_->getSurfaceDrawingPrecision() != -1)
		{
			custom_precision_button->setChecked(true);
			setSurfaceDrawingPrecision(rep_->getSurfaceDrawingPrecision());
		}
		else
		{
			rep_->setSurfaceDrawingPrecision(-1);
			precisionBoxChanged(precision_combobox->currentItem());
		}
	}
}

void DisplayProperties::getAdvancedColoringOptions_()
	throw()
{
	if (rep_ == 0 ||
			rep_->getColorProcessor() == 0 ||
			coloring_method_combobox->currentItem() == COLORING_CUSTOM) 
	{
		return;
	}

	coloring_settings_->getSettings(*rep_->getColorProcessor());
}

void DisplayProperties::getAdvancedModelOptions_()
	throw()
{
	if (rep_ == 0 ||
			rep_->getModelProcessor() == 0) 
	{
		return;
	}
	
	model_settings_->getSettings(*rep_->getModelProcessor());
}
			
void DisplayProperties::applyPreferences()
	throw()
{
	advanced_options_modified_ = true;

	if (rep_ == 0) 
	{
		return;
	}

	const QWidget* current_page = getMainControl()->getPreferences()->currentEntry();

	if (current_page == model_settings_ ||
			current_page == coloring_settings_) 
	{
		apply();
		advanced_options_modified_ = false;
	}
}

bool DisplayProperties::getSettingsFromString(const String& data)
	throw()
{
	vector<String> fields;
	if (data.split(fields) < 6 ||
			fields[0].toUnsignedInt() >= MODEL_LABEL) 
	{
		return false;
	}
	
	try
	{
		selectModel(fields[0].toUnsignedInt());
		selectMode(fields[1].toUnsignedInt());
		setDrawingPrecision(fields[2].toUnsignedInt());
		setSurfaceDrawingPrecision(fields[3].toFloat());
		selectColoringMethod(fields[4].toUnsignedInt());
		setTransparency((Position)(fields[5].toFloat() / 2.55));
		return true;
	}
	catch(...)
	{
	}

	return false;
}


void DisplayProperties::setSurfaceDrawingPrecision(float value)
{
	if (value < 0.1) return;
	precision_slider->setValue((int)(value * 10.0));
	presets_precision_button->setChecked(false);
	custom_precision_button->setChecked(true);
}
		
void DisplayProperties::setDrawingPrecision(int value)
{
	precision_combobox->setCurrentItem(value);
	precisionBoxChanged(value);
	presets_precision_button->setChecked(true);
	custom_precision_button->setChecked(false);
}

void DisplayProperties::setTransparency(int value)
{
	transparency_slider->setValue(value);
}

void DisplayProperties::setCustomColor(const ColorRGBA& color)
{
	custom_color_ = color;
	custom_color_label->setBackgroundColor(custom_color_.getQColor());
}
	
void DisplayProperties::coloringUpdatesChanged()
{
	bool enabled = coloring_updates_enabled->isChecked();

	coloring_method_combobox->setEnabled(enabled);
	custom_button->setEnabled(enabled);
	coloring_options->setEnabled(enabled);
}

void DisplayProperties::modelUpdatesChanged()
{
	bool enabled = model_updates_enabled->isChecked();

	model_type_combobox->setEnabled(enabled);
	model_options->setEnabled(enabled);
	resolution_group->setEnabled(enabled);
}

} } // namespaces
