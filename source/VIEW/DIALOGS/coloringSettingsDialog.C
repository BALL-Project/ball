// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/KERNEL/PTE.h>

#include <set>

#include <QtWidgets>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QCheckBox>

namespace BALL
{
	namespace VIEW
	{
		ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, Qt::WindowFlags fl )
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
			connect(reset_elements_button, SIGNAL(clicked()), this, SLOT(resetElementColors()));

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
							colors[e.getAtomicNumber()] = table->item(p, 1)->background().color();
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
				colors.push_back(table->item(p, 1)->background().color());
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

			// D-01 + D-06: Compute element-color overrides as a diff against the live
			// compiled ElementColorProcessor defaults. Only entries that differ from the
			// compiled defaults are written under the new "ElementColorOverrides=" key.
			// This makes the bug class structurally impossible: there is no stored shadow
			// copy of the defaults that can drift out of sync with the compiled palette.
			{
				ElementColorProcessor live_defaults;
				const HashMap<Position, ColorRGBA>& defaults_map = live_defaults.getColorMap();

				String overrides_serialized;

				for (Position p = 0; p < (Position)element_table_->rowCount(); ++p)
				{
					BALL::Element const& e = PTE_::getElement(ascii(element_table_->item(p, 0)->text()));

					if (e.getAtomicNumber() == 0 || e.getAtomicNumber() > Element::NUMBER_OF_ELEMENTS)
					{
						continue;
					}

					ColorRGBA dialog_color(element_table_->item(p, 1)->background().color());

					HashMap<Position, ColorRGBA>::ConstIterator dit = defaults_map.find(e.getAtomicNumber());

					bool differs = (dit == defaults_map.end()) ||
					               ((String)dialog_color != (String)dit->second);

					if (differs)
					{
						overrides_serialized += String(e.getAtomicNumber()) + ":" + (String)dialog_color + ";";
					}
				}

				if (!overrides_serialized.empty())
				{
					// INIFile upsert idiom: insertValue no-ops if key exists; setValue no-ops if key absent — the pair covers both.
					if (!inifile.insertValue("COLORING_OPTIONS", "ElementColorOverrides", overrides_serialized))
					{
						inifile.setValue("COLORING_OPTIONS", "ElementColorOverrides", overrides_serialized);
					}
				}
				else
				{
					// No overrides: remove any stale ElementColorOverrides= line from a prior write.
					if (inifile.hasEntry("COLORING_OPTIONS", "ElementColorOverrides"))
					{
						INIFile::LineIterator it  = inifile.getSectionFirstLine("COLORING_OPTIONS");
						INIFile::LineIterator end = inifile.getSectionLastLine("COLORING_OPTIONS").getSectionNextLine();
						for (; it != end; ++it)
						{
							if ((*it).hasPrefix("ElementColorOverrides="))
							{
								inifile.deleteLine(it);
								break;
							}
						}
					}
				}
			}

			String residue_names, residue_name_colors;

			for (Index i=0; i<residue_table_->rowCount(); ++i)
			{
				residue_names       += ascii(residue_table_->item(i, 0)->text()) + ";";
				residue_name_colors += (String)(static_cast<ColorRGBA>(residue_table_->item(i, 1)->background().color())) + ";";
			}

			inifile.insertValue("COLORING_OPTIONS", "ResidueNames",      residue_names);
			inifile.insertValue("COLORING_OPTIONS", "ResidueNameColors", residue_name_colors);
		}

		void ColoringSettingsDialog::readPreferenceEntries(const INIFile& inifile)
		{
			PreferencesEntry::readPreferenceEntries(inifile);

			// D-05 conflict rule: legacy-discard branch (step 1) runs first so that when both
			// Elements= and ElementColorOverrides= are present, the old key is dropped and the
			// new key is parsed — the ordering invariant handles the both-present case by construction.

			// Step 1 — D-03: silently discard any legacy Elements= line from pre-Phase-4.1 configs.
			// This runs unconditionally: if the key is present it is deleted and compiled defaults
			// flow through for all untouched indices (optimistic migration per D-03).
			if (inifile.hasEntry("COLORING_OPTIONS", "Elements"))
			{
				INIFile::LineIterator it  = const_cast<INIFile&>(inifile).getSectionFirstLine("COLORING_OPTIONS");
				INIFile::LineIterator end = const_cast<INIFile&>(inifile).getSectionLastLine("COLORING_OPTIONS").getSectionNextLine();
				for (; it != end; ++it)
				{
					if ((*it).hasPrefix("Elements="))
					{
						// Local const_cast: deleteLine mutates the INIFile (line removal). The base virtual
						// signature uses `const INIFile&` for read-only consumers; legacy-key migration is the
						// one mutation we perform inside the read path (D-03 optimistic discard). All other
						// PreferencesEntry subclasses in VIEW/DIALOGS already use this signature; the const_cast
						// is contained to this single call site.
						const_cast<INIFile&>(inifile).deleteLine(it);
						Log.info() << "ColoringSettingsDialog: dropping legacy Elements= block (Phase 4.1 migration; compiled defaults will be used for untouched indices)" << std::endl;
						break;
					}
				}
			}

			// Step 2 — D-05.1: parse the new ElementColorOverrides= key if present and apply
			// override colors into element_table_ for each atomic number listed.
			if (inifile.hasEntry("COLORING_OPTIONS", "ElementColorOverrides"))
			{
				String overrides_raw = inifile.getValue("COLORING_OPTIONS", "ElementColorOverrides");

				// D-05.2: known-colored elements whose all-white override (#ffffff) is suspicious
				// and should be discarded as likely corruption (carbon/nitrogen/oxygen/phosphorus/sulphur).
				std::set<Position> known_colored;
				known_colored.insert(6);   // carbon
				known_colored.insert(7);   // nitrogen
				known_colored.insert(8);   // oxygen
				known_colored.insert(15);  // phosphorus
				known_colored.insert(16);  // sulphur

				std::vector<String> tokens;
				overrides_raw.split(tokens, ";");

				for (Position t = 0; t < (Position)tokens.size(); ++t)
				{
					if (tokens[t].isEmpty()) continue;

					std::vector<String> parts;
					tokens[t].split(parts, ":");

					if (parts.size() != 2 || parts[0].isEmpty() || parts[1].isEmpty()) continue;

					Position atomic_number = parts[0].toUnsignedInt();
					ColorRGBA override_color(parts[1]);

					// D-05.2 all-white sanity check: drop override if RGB is (255,255,255)
					// for a known-colored element — treat as corruption and fall through to
					// the compiled default for that index.
					if (known_colored.count(atomic_number) > 0)
					{
						ColorUnit white_unit(1.0f);
						if (override_color.getRed()   == white_unit &&
						    override_color.getGreen() == white_unit &&
						    override_color.getBlue()  == white_unit)
						{
							Log.warn() << "ColoringSettingsDialog: dropping suspicious all-white override for atomic number "
							           << atomic_number << " (D-05 sanity check; compiled default will be used)" << std::endl;
							continue;
						}
					}

					// Find the matching row in element_table_ by symbol and apply the override color.
					String symbol = PTE[atomic_number].getSymbol();
					for (Position p = 0; p < (Position)element_table_->rowCount(); ++p)
					{
						if (ascii(element_table_->item(p, 0)->text()) == symbol)
						{
							element_table_->item(p, 1)->setBackground(override_color.getQColor());
							break;
						}
					}
				}
			}

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
            if (RTTI::isKindOf<CustomColorProcessor>(&cp)) return;

            if (RTTI::isKindOf<ElementColorProcessor>(&cp))
			{
				vector<ColorRGBA> colors = getColors(COLORING_ELEMENT);
				for (Position p = 0; p < colors.size(); p++)
				{
					(*(ElementColorProcessor*)&cp).getColorMap()[p] = colors[p];
				}
				return;
			}
			
            if (RTTI::isKindOf<ResidueNameColorProcessor>(&cp))
			{
				for (Position p = 0; p < (Position)residue_table_->rowCount(); p++)
				{
					(*(ResidueNameColorProcessor*)&cp).getColorMap()[ascii(residue_table_->item(p,0)->text())] = 
						residue_table_->item(p,1)->background().color();
				}
				return;
			}

            if (RTTI::isKindOf<ResidueNumberColorProcessor>(&cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				dp.setFirstColor(first_residue_button->getColor());
				dp.setMiddleColor(middle_residue_button->getColor());
				dp.setLastColor(last_residue_button->getColor());
				return;
			}

            if (RTTI::isKindOf<AtomChargeColorProcessor>(&cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				dp.getColors()[0] = negative_charge_button->getColor();
				dp.getColors()[1] = neutral_charge_button->getColor();
				dp.getColors()[2] = positive_charge_button->getColor();
				return;
			}

            if (RTTI::isKindOf<AtomDistanceColorProcessor>(&cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				dp.setNullDistanceColor(null_distance_button->getColor());
				dp.setMaxDistanceColor(max_distance_button->getColor());
				dp.setDistance(((float)max_distance_slider->value()) / 10.0);
 				dp.setShowSelected(distance_show_selected->isChecked());
				return;
			}

            if (RTTI::isKindOf<OccupancyColorProcessor>(&cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				dp.getColors()[0] = minimum_o_button->getColor();
				dp.getColors()[1] = maximum_o_button->getColor();
				return;
			}

            if (RTTI::isKindOf<SecondaryStructureColorProcessor>(&cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);

				dp.setHelixColor(helix_color_button->getColor());
				dp.setCoilColor(coil_color_button->getColor());
				dp.setStrandColor(strand_color_button->getColor());
				dp.setTurnColor(turn_color_button->getColor());

				return;
			}

            if (RTTI::isKindOf<TemperatureFactorColorProcessor>(&cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				dp.setMinColor(unassigned_tf_button->getColor());
				dp.getColors()[0] = minimum_tf_button->getColor();
				dp.getColors()[1] = maximum_tf_button->getColor();
				dp.setMaxColor(unassigned_tf_button->getColor());
				dp.setMaxValue(((float)max_tf_slider->value()) / 10.0);
				return;
			}

            if (RTTI::isKindOf<ForceColorProcessor>(&cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				dp.getColors()[0] = force_min_color_button->getColor();
				dp.getColors()[1] = force_max_color_button->getColor();
				dp.setMaxValue(((float)force_max_value_slider->value()) / 10.0);
				dp.setMinValue(((float)force_min_value_slider->value()) / 10.0);
				return;
			}

            if (RTTI::isKindOf<ResidueTypeColorProcessor>(&cp))
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

            if (RTTI::isKindOf<ChainColorProcessor>(&cp))
			{
				((ChainColorProcessor*)&cp)->setColors(getColors(COLORING_CHAIN));
				return;
			}

            if (RTTI::isKindOf<MoleculeColorProcessor>(&cp))
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

		void ColoringSettingsDialog::resetElementColors()
		{
			// D-02: clear the user's element-color overrides by repopulating
			// element_table_ from a freshly constructed ElementColorProcessor.
			// The next writePreferenceEntries call diffs the table against the
			// same compiled defaults (D-06) and will emit an empty
			// ElementColorOverrides= — restoring the unshadowed compiled palette
			// without requiring the user to delete ~/.BALLView (CONFIG-01 criterion 3).
			ElementColorProcessor elp;
			const HashMap<Position, ColorRGBA>& color_hash_map = elp.getColorMap();
			vector<String> names;
			vector<ColorRGBA> colors;
			HashMap<Position, ColorRGBA>::ConstIterator it = color_hash_map.begin();
			for (; it != color_hash_map.end(); it++)
			{
				if (it->first == 0) continue;
				names.push_back(PTE[it->first].getSymbol());
				colors.push_back(it->second);
			}
			names.push_back(PTE[0].getSymbol());
			colors.push_back(color_hash_map[0]);
			element_table_->setContent(names, colors);
		}


		ColorProcessor* ColoringSettingsDialog::createColorProcessor(ColoringMethod method) const
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

            if (RTTI::isKindOf<CustomColorProcessor>(&cp))
			{
			} else

            if (RTTI::isKindOf<ElementColorProcessor>(&cp))
			{
			} else
			
            if (RTTI::isKindOf<ResidueNameColorProcessor>(&cp))
			{
			} else

            if (RTTI::isKindOf<ResidueNumberColorProcessor>(&cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				first_residue_button->setColor(dp.getFirstColor());
				middle_residue_button->setColor(dp.getMiddleColor());
				last_residue_button->setColor(dp.getLastColor());
			} else

            if (RTTI::isKindOf<AtomChargeColorProcessor>(&cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				negative_charge_button->setColor( dp.getColors()[0]);
				neutral_charge_button->setColor(dp.getColors()[1]);
				positive_charge_button->setColor( dp.getColors()[2]);
			} else

            if (RTTI::isKindOf<AtomDistanceColorProcessor>(&cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				null_distance_button->setColor( dp.getNullDistanceColor());
				max_distance_button->setColor(dp.getMaxDistanceColor());
				max_distance_slider->setValue((Size)(dp.getDistance() * 10.0));
 				distance_show_selected->setChecked(dp.showSelected());
			} else

            if (RTTI::isKindOf<OccupancyColorProcessor>(&cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				minimum_o_button->setColor(dp.getColors()[0]);
				maximum_o_button->setColor(dp.getColors()[1]);
			} else

            if (RTTI::isKindOf<SecondaryStructureColorProcessor>(&cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);
				helix_color_button->setColor(dp.getHelixColor());
				coil_color_button->setColor(dp.getCoilColor());
				strand_color_button->setColor(dp.getStrandColor());
				turn_color_button->setColor(dp.getTurnColor());
			} else

            if (RTTI::isKindOf<TemperatureFactorColorProcessor>(&cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				unassigned_tf_button->setColor( dp.getDefaultColor());
				minimum_tf_button->setColor(dp.getColors()[0]);
				maximum_tf_button->setColor(dp.getColors()[1]);
				max_tf_slider->setValue((Size)(dp.getMaxValue() * 10.0));
			} else

            if (RTTI::isKindOf<ForceColorProcessor>(&cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				force_min_color_button->setColor(dp.getColors()[0]);
				force_max_color_button->setColor(dp.getColors()[1]);
				force_max_value_slider->setValue((Size)(dp.getMaxValue() * 10.0));
				force_min_value_slider->setValue((Size)(dp.getMinValue() * 10.0));
			} else

            if (RTTI::isKindOf<ResidueTypeColorProcessor>(&cp))
			{
 				ResidueTypeColorProcessor& dp = (*(ResidueTypeColorProcessor*)&cp);
				acidic_color_button->setColor(dp.getAcidicColor());
				aromatic_color_button->setColor(dp.getAromaticColor());
				basic_color_button->setColor(dp.getBasicColor());
				hydrophobic_color_button->setColor(dp.getHydrophobicColor());
				other_color_button->setColor(dp.getOtherColor());
				polar_color_button->setColor(dp.getPolarColor());
			} else

            if (RTTI::isKindOf<ChainColorProcessor>(&cp))
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

            if (RTTI::isKindOf<MoleculeColorProcessor>(&cp))
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
