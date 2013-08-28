// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/KERNEL/PTE.h>

#include <QtGui/QColorDialog>
#include <QtGui/QSlider>
#include <QtGui/QLabel>
#include <QtGui/QStackedWidget>
#include <QtGui/QCheckBox>

namespace BALL
{
	namespace VIEW
	{
		ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, Qt::WFlags fl )
			: QWidget(parent, fl),
				Ui_ColoringSettingsDialogData(),
				PreferencesEntry()
		{
			setupUi(this);

			element_table_ ->setObjectName("Elements");
//			residue_table_ ->setObjectName("Residues");
			chain_table_   ->setObjectName("Chains");
			molecule_table_->setObjectName("Molecules");

			setObjectName(name);
			setINIFileSectionName("COLORING_OPTIONS");
			setDefaultValues_();
			setWidgetStackName((String)tr("Model Colors"));
			setWidgetStack(widget_stack);

			// signals and slots connections
			connect( force_max_value_slider, SIGNAL( valueChanged(int) ), this, SLOT( forceMaxValueChanged() ) );
			connect( force_min_value_slider, SIGNAL( valueChanged(int) ), this, SLOT( forceMinValueChanged() ) );
			connect( max_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( maxDistanceChanged() ) );
			connect( max_tf_slider, SIGNAL( valueChanged(int) ), this, SLOT( maxTFChanged() ) );

			registerWidgets_();
		}

		void ColoringSettingsDialog::setDefaultValues_()
		{
			vector<String> 		names;
			vector<ColorRGBA> colors;

			// =============================================================
			// setting element colors
			{
				// create a dummy processor to get the default values
				ElementColorProcessor elp;
				const HashMap<Position, ColorRGBA>& color_hash_map = elp.getColorMap();
				HashMap<Position, ColorRGBA>::ConstIterator it = color_hash_map.begin();

				for(; it != color_hash_map.end(); it++)
				{
					if (it->first == 0) continue;
					names.push_back(PTE[it->first].getSymbol());
					colors.push_back(it->second);
				}

				names.push_back(PTE[0].getSymbol());
				colors.push_back(color_hash_map[0]);
				element_table_->setNamesTitle((String)tr("Element"));
				element_table_->setContent(names, colors);
				names.clear();
				colors.clear();
			}

			// =============================================================
			// setting residue name colors
			// create a dummy processor to get the default values
			{
				ResidueNameColorProcessor rcp;
				const StringHashMap<ColorRGBA>& color_map = rcp.getColorMap();
				StringHashMap<ColorRGBA>::ConstIterator it2 = color_map.begin();

				for(; it2 != color_map.end(); it2++)
				{
					names.push_back(it2->first);
					colors.push_back(it2->second);
				}

				residue_table_->setNamesTitle((String)tr("Residue"));
				residue_table_->setContent(names, colors);
			}
				
			// =============================================================
			getSettings(ResidueNumberColorProcessor());
			getSettings(AtomChargeColorProcessor());
			getSettings(AtomDistanceColorProcessor());
			getSettings(TemperatureFactorColorProcessor());
			getSettings(OccupancyColorProcessor());
			getSettings(SecondaryStructureColorProcessor());
			getSettings(ForceColorProcessor());
			getSettings(ResidueTypeColorProcessor());
			getSettings(ChainColorProcessor());
			getSettings(MoleculeColorProcessor());
		}

		vector<ColorRGBA> ColoringSettingsDialog::getColors(ColoringMethod method) const
		{
			vector<ColorRGBA> colors;
			ColorTable* table = 0;
			switch (method)
			{
				case COLORING_ELEMENT:
				{
					table = element_table_;
					colors.resize(table->rowCount());
                    
					for (Position p=0; p<(Position)table->rowCount(); p++)
					{
						// NOTE: do not remove the scope here; there is a class member Element flying around somewhere...
						BALL::Element const& e = PTE_::getElement(ascii(table->item(p, 0)->text()));
						
						if ((e.getAtomicNumber() > 0) && (e.getAtomicNumber() < table->rowCount()))
						{
							colors[e.getAtomicNumber()] = table->item(p, 1)->backgroundColor();
						}
					}
						
					return colors;
				}
				case COLORING_RESIDUE_INDEX: 	table = residue_table_; break;
				case COLORING_CHAIN: 					table = chain_table_  ; break;
				case COLORING_MOLECULE: 			table = molecule_table_; break;
				default: return colors;
			}
					
			for (Position p = 0; p < (Position)table->rowCount(); p++)
			{
				colors.push_back(table->item(p, 1)->backgroundColor());
			}

			return colors;
		}

		void ColoringSettingsDialog::writePreferenceEntries(INIFile& inifile)
		{
			PreferencesEntry::writePreferenceEntries(inifile);

			if (!inifile.hasSection("COLORING_OPTIONS"))
			{
				inifile.appendSection("COLORING_OPTIONS");
			}

			String residue_names, residue_name_colors;

			for (Index i=0; i<residue_table_->rowCount(); ++i)
			{
				residue_names       += ascii(residue_table_->item(i, 0)->text()) + ";";
				residue_name_colors += (String)(static_cast<ColorRGBA>(residue_table_->item(i, 1)->backgroundColor())) + ";";
			}

			inifile.insertValue("COLORING_OPTIONS", "ResidueNames",      residue_names);
			inifile.insertValue("COLORING_OPTIONS", "ResidueNameColors", residue_name_colors);
		}

		void ColoringSettingsDialog::readPreferenceEntries(INIFile& inifile)
		{
			PreferencesEntry::readPreferenceEntries(inifile);

			if (    inifile.hasEntry("COLORING_OPTIONS", "ResidueNames") 
				   && inifile.hasEntry("COLORING_OPTIONS", "ResidueNameColors"))
			{
				String residue_names       = inifile.getValue("COLORING_OPTIONS", "ResidueNames");
				String residue_name_colors = inifile.getValue("COLORING_OPTIONS", "ResidueNameColors");

				std::vector<String> split_names;
				residue_names.split(split_names);

				std::vector<String> split_colors;
				residue_name_colors.split(split_colors);

				if (split_names.size() != split_colors.size())
				{
					Log.warn() << "ColoringSettingsDialog::fetchPreferences: residue name coloring in inifile is invalid!" << std::endl;
				}

				std::vector<ColorRGBA> split_color_rgba(split_colors.size());
				for (Position i=0; i<split_color_rgba.size(); ++i)
				{
					split_color_rgba[i] = ColorRGBA(split_colors[i]);
				}
				residue_table_->setContent(split_names, split_color_rgba);
			}
		}

		void ColoringSettingsDialog::applySettingsTo(ColorProcessor& cp) const
		{
			if (RTTI::isKindOf<CustomColorProcessor>(cp)) return;

			if (RTTI::isKindOf<ElementColorProcessor>(cp))
			{
				vector<ColorRGBA> colors = getColors(COLORING_ELEMENT);
				for (Position p = 0; p < colors.size(); p++)
				{
					(*(ElementColorProcessor*)&cp).getColorMap()[p] = colors[p];
				}
				return;
			}
			
			if (RTTI::isKindOf<ResidueNameColorProcessor>(cp))
			{
				for (Position p = 0; p < (Position)residue_table_->rowCount(); p++)
				{
					(*(ResidueNameColorProcessor*)&cp).getColorMap()[ascii(residue_table_->item(p,0)->text())] = 
						residue_table_->item(p,1)->backgroundColor();
				}
				return;
			}

			if (RTTI::isKindOf<ResidueNumberColorProcessor>(cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				dp.setFirstColor(first_residue_button->getColor());
				dp.setMiddleColor(middle_residue_button->getColor());
				dp.setLastColor(last_residue_button->getColor());
				return;
			}

			if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				dp.getColors()[0] = negative_charge_button->getColor();
				dp.getColors()[1] = neutral_charge_button->getColor();
				dp.getColors()[2] = positive_charge_button->getColor();
				return;
			}

			if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				dp.setNullDistanceColor(null_distance_button->getColor());
				dp.setMaxDistanceColor(max_distance_button->getColor());
				dp.setDistance(((float)max_distance_slider->value()) / 10.0);
 				dp.setShowSelected(distance_show_selected->isChecked());
				return;
			}

			if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				dp.getColors()[0] = minimum_o_button->getColor();
				dp.getColors()[1] = maximum_o_button->getColor();
				return;
			}

			if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);

				dp.setHelixColor(helix_color_button->getColor());
				dp.setCoilColor(coil_color_button->getColor());
				dp.setStrandColor(strand_color_button->getColor());
				dp.setTurnColor(turn_color_button->getColor());

				return;
			}

			if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				dp.setMinColor(unassigned_tf_button->getColor());
				dp.getColors()[0] = minimum_tf_button->getColor();
				dp.getColors()[1] = maximum_tf_button->getColor();
				dp.setMaxColor(unassigned_tf_button->getColor());
				dp.setMaxValue(((float)max_tf_slider->value()) / 10.0);
				return;
			}

			if (RTTI::isKindOf<ForceColorProcessor>(cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				dp.getColors()[0] = force_min_color_button->getColor();
				dp.getColors()[1] = force_max_color_button->getColor();
				dp.setMaxValue(((float)force_max_value_slider->value()) / 10.0);
				dp.setMinValue(((float)force_min_value_slider->value()) / 10.0);
				return;
			}

			if (RTTI::isKindOf<ResidueTypeColorProcessor>(cp))
			{
				ResidueTypeColorProcessor& dp = (*(ResidueTypeColorProcessor*)&cp);
				dp.setBasicColor(basic_color_button->getColor());
				dp.setAcidicColor(acidic_color_button->getColor());
				dp.setAromaticColor(aromatic_color_button->getColor());
				dp.setPolarColor(polar_color_button->getColor());
				dp.setHydrophobicColor(hydrophobic_color_button->getColor());
				dp.setOtherColor(other_color_button->getColor());
				return;
			}

			if (RTTI::isKindOf<ChainColorProcessor>(cp))
			{
				((ChainColorProcessor*)&cp)->setColors(getColors(COLORING_CHAIN));
				return;
			}

			if (RTTI::isKindOf<MoleculeColorProcessor>(cp))
			{
				((MoleculeColorProcessor*)&cp)->setColors(getColors(COLORING_MOLECULE));
				return;
			}

		}
			
		void ColoringSettingsDialog::maxDistanceChanged()
		{
			String text = String(((float)max_distance_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			max_distance_value_label->setText(text.c_str());
		}
			
		void ColoringSettingsDialog::maxTFChanged()
		{
			String text = String(((float)max_tf_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			max_tf_label->setText(text.c_str());
		}

		void ColoringSettingsDialog::forceMaxValueChanged()
		{
			String text = String(((float)force_max_value_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			force_max_value_label->setText(text.c_str());
		}

		void ColoringSettingsDialog::forceMinValueChanged()
		{
			String text = String(((float)force_min_value_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			force_min_value_label->setText(text.c_str());
		}


		ColorProcessor* ColoringSettingsDialog::createColorProcessor(ColoringMethod method) const
			throw(Exception::InvalidOption)
		{
			ColorProcessor* color_processor = 0;

			switch(method)
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
					color_processor = new CustomColorProcessor;
					break;

				case COLORING_DISTANCE:
					color_processor = new AtomDistanceColorProcessor;
					break;

				case COLORING_TEMPERATURE_FACTOR:
					color_processor = new TemperatureFactorColorProcessor;
					break;

				case COLORING_OCCUPANCY:
					color_processor = new OccupancyColorProcessor;
					break;

				case COLORING_FORCES:
					color_processor = new ForceColorProcessor;
					break;

				case COLORING_RESIDUE_TYPE:
					color_processor = new ResidueTypeColorProcessor;
					break;

				case COLORING_CHAIN:
					color_processor = new ChainColorProcessor;
					break;

				case COLORING_MOLECULE:
					color_processor = new MoleculeColorProcessor;
					break;

				default:
					throw(Exception::InvalidOption(__FILE__, __LINE__, method));
			}

			applySettingsTo(*color_processor);

			return color_processor;
		}

		void ColoringSettingsDialog::getSettings(const ColorProcessor& cp)
		{

			if (RTTI::isKindOf<CustomColorProcessor>(cp))
			{
			} else

			if (RTTI::isKindOf<ElementColorProcessor>(cp))
			{
			} else
			
			if (RTTI::isKindOf<ResidueNameColorProcessor>(cp))
			{
			} else

			if (RTTI::isKindOf<ResidueNumberColorProcessor>(cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				first_residue_button->setColor(dp.getFirstColor());
				middle_residue_button->setColor(dp.getMiddleColor());
				last_residue_button->setColor(dp.getLastColor());
			} else

			if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				negative_charge_button->setColor( dp.getColors()[0]);
				neutral_charge_button->setColor(dp.getColors()[1]);
				positive_charge_button->setColor( dp.getColors()[2]);
			} else

			if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				null_distance_button->setColor( dp.getNullDistanceColor());
				max_distance_button->setColor(dp.getMaxDistanceColor());
				max_distance_slider->setValue((Size)(dp.getDistance() * 10.0));
 				distance_show_selected->setChecked(dp.showSelected());
			} else

			if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				minimum_o_button->setColor(dp.getColors()[0]);
				maximum_o_button->setColor(dp.getColors()[1]);
			} else

			if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);
				helix_color_button->setColor(dp.getHelixColor());
				coil_color_button->setColor(dp.getCoilColor());
				strand_color_button->setColor(dp.getStrandColor());
				turn_color_button->setColor(dp.getTurnColor());
			} else

			if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				unassigned_tf_button->setColor( dp.getDefaultColor());
				minimum_tf_button->setColor(dp.getColors()[0]);
				maximum_tf_button->setColor(dp.getColors()[1]);
				max_tf_slider->setValue((Size)(dp.getMaxValue() * 10.0));
			} else

			if (RTTI::isKindOf<ForceColorProcessor>(cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				force_min_color_button->setColor(dp.getColors()[0]);
				force_max_color_button->setColor(dp.getColors()[1]);
				force_max_value_slider->setValue((Size)(dp.getMaxValue() * 10.0));
				force_min_value_slider->setValue((Size)(dp.getMinValue() * 10.0));
			} else

			if (RTTI::isKindOf<ResidueTypeColorProcessor>(cp))
			{
 				ResidueTypeColorProcessor& dp = (*(ResidueTypeColorProcessor*)&cp);
				acidic_color_button->setColor(dp.getAcidicColor());
				aromatic_color_button->setColor(dp.getAromaticColor());
				basic_color_button->setColor(dp.getBasicColor());
				hydrophobic_color_button->setColor(dp.getHydrophobicColor());
				other_color_button->setColor(dp.getOtherColor());
				polar_color_button->setColor(dp.getPolarColor());
			} else

			if (RTTI::isKindOf<ChainColorProcessor>(cp))
			{
 				ChainColorProcessor& dp = (*(ChainColorProcessor*)&cp);
				vector<String> 		names;
				vector<ColorRGBA> colors;

				for (Position p = 0; p < dp.getColors().size(); p++)
				{
					colors.push_back(dp.getColors()[p]);
					names.push_back(p);
				}

				chain_table_->setNamesTitle((String)tr("Chain"));
				chain_table_->setContent(names, colors);
			} else

			if (RTTI::isKindOf<MoleculeColorProcessor>(cp))
			{
 				MoleculeColorProcessor& dp = (*(MoleculeColorProcessor*)&cp);
				vector<String> 		names;
				vector<ColorRGBA> colors;

				for (Position p = 0; p < dp.getColors().size(); p++)
				{
					colors.push_back(dp.getColors()[p]);
					names.push_back(p);
				}

				molecule_table_->setNamesTitle((String)tr("Molecule"));
				molecule_table_->setContent(names, colors);
			}

		}

		QWidget* ColoringSettingsDialog::getEntryFor(ColoringMethod method)
		{
			switch (method)
			{
				case COLORING_ELEMENT: 							return widget_stack->widget(0);
				case COLORING_RESIDUE_NAME: 				return widget_stack->widget(2);
				case COLORING_RESIDUE_INDEX: 				return widget_stack->widget(1);
				case COLORING_SECONDARY_STRUCTURE: 	return widget_stack->widget(7);
				case COLORING_ATOM_CHARGE: 					return widget_stack->widget(3);
				case COLORING_DISTANCE: 						return widget_stack->widget(4);
				case COLORING_TEMPERATURE_FACTOR: 	return widget_stack->widget(5);
				case COLORING_OCCUPANCY: 						return widget_stack->widget(6);
				case COLORING_FORCES: 							return widget_stack->widget(8);
				case COLORING_RESIDUE_TYPE: 				return widget_stack->widget(9);
				case COLORING_CHAIN: 								return widget_stack->widget(10);
				case COLORING_MOLECULE: 						return widget_stack->widget(11);
				default: break;
			}

			return 0;
		}

  } // namespace VIEW
} // namespace BALL
