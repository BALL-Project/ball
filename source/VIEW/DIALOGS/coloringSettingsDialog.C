// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessorFactory.h>
#include <BALL/KERNEL/PTE.h>

#include <QtWidgets>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QCheckBox>

#include <algorithm>

namespace
{
	// D-05.2: atomic numbers whose all-white override is treated as corruption
	// (carbon, nitrogen, oxygen, phosphorus, sulphur). File-scope so the rule
	// lives in one named place and the parse loop stays readable.
	constexpr BALL::Position KNOWN_COLORED_ATOMIC_NUMBERS[] = {6, 7, 8, 15, 16};

	bool isKnownColoredElement(BALL::Position atomic_number)
	{
		return std::find(std::begin(KNOWN_COLORED_ATOMIC_NUMBERS),
		                 std::end(KNOWN_COLORED_ATOMIC_NUMBERS),
		                 atomic_number) != std::end(KNOWN_COLORED_ATOMIC_NUMBERS);
	}
}

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
						// Bind the symbol-temporary to a named local first so GCC 13's
						// -Wdangling-reference doesn't trip on the inner expression.
						const String symbol = ascii(table->item(p, 0)->text());
						BALL::Element const& e = PTE_::getElement(symbol);

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
					const String symbol = ascii(element_table_->item(p, 0)->text());
					BALL::Element const& e = PTE_::getElement(symbol);

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

			// D-01 + D-06 (Phase 999.4b — Phase 4.1 element-path pattern carried forward to
			// residues): write only residue-color entries that DIFFER from the live compiled
			// ResidueNameColorProcessor defaults. The persisted file no longer contains a
			// shadow copy of the full residue palette, structurally eliminating the bug class
			// where a saved full-table block silently shadows the compiled defaults.
			{
				ResidueNameColorProcessor live_defaults;
				const StringHashMap<ColorRGBA>& defaults_map = live_defaults.getColorMap();

				String overrides_serialized;

				for (Index i = 0; i < residue_table_->rowCount(); ++i)
				{
					String residue_name = ascii(residue_table_->item(i, 0)->text());
					if (residue_name.isEmpty() || residue_name == "---") continue;

					ColorRGBA dialog_color(residue_table_->item(i, 1)->background().color());

					StringHashMap<ColorRGBA>::ConstIterator dit = defaults_map.find(residue_name);

					bool differs = (dit == defaults_map.end()) ||
					               ((String)dialog_color != (String)dit->second);

					if (differs)
					{
						overrides_serialized += residue_name + ":" + (String)dialog_color + ";";
					}
				}

				if (!overrides_serialized.empty())
				{
					// INIFile upsert idiom (Phase 4.1 Issue 1 carry-forward): insertValue no-ops if
					// key exists; setValue no-ops if key absent — the pair covers both call sites.
					if (!inifile.insertValue("COLORING_OPTIONS", "ResidueNameColorOverrides", overrides_serialized))
					{
						inifile.setValue("COLORING_OPTIONS", "ResidueNameColorOverrides", overrides_serialized);
					}
				}
				else
				{
					// No overrides: remove any stale ResidueNameColorOverrides= line from a prior write.
					if (inifile.hasEntry("COLORING_OPTIONS", "ResidueNameColorOverrides"))
					{
						INIFile::LineIterator it  = inifile.getSectionFirstLine("COLORING_OPTIONS");
						INIFile::LineIterator end = inifile.getSectionLastLine("COLORING_OPTIONS").getSectionNextLine();
						for (; it != end; ++it)
						{
							if ((*it).hasPrefix("ResidueNameColorOverrides="))
							{
								inifile.deleteLine(it);
								break;
							}
						}
					}
				}
			}
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

				std::vector<String> tokens;
				overrides_raw.split(tokens, ";");

				for (Position t = 0; t < (Position)tokens.size(); ++t)
				{
					if (tokens[t].isEmpty()) continue;

					std::vector<String> parts;
					tokens[t].split(parts, ":");

					if (parts.size() != 2 || parts[0].isEmpty() || parts[1].isEmpty()) continue;

					// D-05.2 length pre-check: ColorRGBA(const String&) reads 6 or 8 chars without bounds
					// checking in release builds (source/VIEW/DATATYPE/colorRGBA.C:255-282 — undefined
					// behaviour for shorter strings); in BALL_VIEW_DEBUG it throws Exception::InvalidRange.
					// Guard explicitly so a malformed token cannot corrupt or abort.
					if (parts[1].size() != 6 && parts[1].size() != 8)
					{
						Log.warn() << "ColoringSettingsDialog: dropping malformed override token '"
						           << tokens[t] << "' (color hex must be 6 or 8 chars)" << std::endl;
						continue;
					}

					Position atomic_number;
					ColorRGBA override_color;
					try
					{
						atomic_number = parts[0].toUnsignedInt();
						override_color = ColorRGBA(parts[1]);
					}
					catch (const Exception::GeneralException&)
					{
						// CONFIG-01 criterion 2: stale/partial saved color block falls back to compiled defaults.
						// String::toUnsignedInt throws InvalidFormat on non-numeric input; ColorRGBA throws
						// InvalidRange on bad length in BALL_VIEW_DEBUG. Both derive from GeneralException.
						Log.warn() << "ColoringSettingsDialog: dropping unparseable override token '"
						           << tokens[t] << "' (parse exception)" << std::endl;
						continue;
					}

					// D-05.2 all-white sanity check: drop override if RGB is (255,255,255)
					// for a known-colored element — treat as corruption and fall through to
					// the compiled default for that index.
					if (isKnownColoredElement(atomic_number))
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

			// Phase 999.4b residue-path read (mirrors the Phase 4.1 element-path read above).
			// D-05 conflict rule: legacy-discard branch (step 1) runs first so that when both
			// ResidueNames=/ResidueNameColors= and ResidueNameColorOverrides= are present, the
			// legacy keys are dropped and the new key wins — both-present is handled by construction.

			// Step 1 — D-03: silently discard any legacy ResidueNames= line from pre-Phase-999.4b configs.
			if (inifile.hasEntry("COLORING_OPTIONS", "ResidueNames"))
			{
				INIFile::LineIterator it  = const_cast<INIFile&>(inifile).getSectionFirstLine("COLORING_OPTIONS");
				INIFile::LineIterator end = const_cast<INIFile&>(inifile).getSectionLastLine("COLORING_OPTIONS").getSectionNextLine();
				for (; it != end; ++it)
				{
					if ((*it).hasPrefix("ResidueNames="))
					{
						const_cast<INIFile&>(inifile).deleteLine(it);
						Log.info() << "ColoringSettingsDialog: dropping legacy ResidueNames= block (Phase 999.4b migration; compiled defaults will be used for untouched residues)" << std::endl;
						break;
					}
				}
			}
			// Also drop the companion legacy ResidueNameColors= key.
			if (inifile.hasEntry("COLORING_OPTIONS", "ResidueNameColors"))
			{
				INIFile::LineIterator it  = const_cast<INIFile&>(inifile).getSectionFirstLine("COLORING_OPTIONS");
				INIFile::LineIterator end = const_cast<INIFile&>(inifile).getSectionLastLine("COLORING_OPTIONS").getSectionNextLine();
				for (; it != end; ++it)
				{
					if ((*it).hasPrefix("ResidueNameColors="))
					{
						const_cast<INIFile&>(inifile).deleteLine(it);
						break;
					}
				}
			}

			// Step 2 — D-05.1: parse the new ResidueNameColorOverrides= key if present and
			// apply override colors into residue_table_ for each matching residue name. The
			// table has already been populated with compiled defaults in setDefaultValues_();
			// untouched rows therefore retain the compiled defaults (no shadow-default risk).
			//
			// Intentional asymmetry with the Phase 4.1 element path: NO all-white sanity check
			// here. The compiled residue palette contains legitimately-white entries — GLY
			// {255,255,255} and ALA {216,255,255} at standardColorProcessor.C:204-205 — so an
			// all-white override for those residues is INDISTINGUISHABLE from a legitimate user
			// choice. Adding the element-path's white-drop check would corrupt valid user data.
			if (inifile.hasEntry("COLORING_OPTIONS", "ResidueNameColorOverrides"))
			{
				String overrides_raw = inifile.getValue("COLORING_OPTIONS", "ResidueNameColorOverrides");

				std::vector<String> tokens;
				overrides_raw.split(tokens, ";");

				for (Position t = 0; t < (Position)tokens.size(); ++t)
				{
					if (tokens[t].isEmpty()) continue;

					std::vector<String> parts;
					tokens[t].split(parts, ":");

					if (parts.size() != 2 || parts[0].isEmpty() || parts[1].isEmpty()) continue;

					// Phase 4.1 Issue 2 carry-forward: ColorRGBA(const String&) reads 6 or 8 chars
					// without bounds checking in release builds; throws InvalidRange in BALL_VIEW_DEBUG.
					if (parts[1].size() != 6 && parts[1].size() != 8)
					{
						Log.warn() << "ColoringSettingsDialog: dropping malformed residue override token '"
						           << tokens[t] << "' (color hex must be 6 or 8 chars)" << std::endl;
						continue;
					}

					ColorRGBA override_color;
					try
					{
						override_color = ColorRGBA(parts[1]);
					}
					catch (const Exception::GeneralException&)
					{
						Log.warn() << "ColoringSettingsDialog: dropping unparseable residue override token '"
						           << tokens[t] << "' (parse exception)" << std::endl;
						continue;
					}

					// Find the matching row in residue_table_ by name and apply the override color.
					for (Position p = 0; p < (Position)residue_table_->rowCount(); ++p)
					{
						if (ascii(residue_table_->item(p, 0)->text()) == parts[0])
						{
							residue_table_->item(p, 1)->setBackground(override_color.getQColor());
							break;
						}
					}
				}
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


		ColoringOverrides ColoringSettingsDialog::buildColoringOverrides_() const
		{
			// Pack the dialog's current table/color-button/slider state into a
			// headless ColoringOverrides value object. Reads here mirror exactly the
			// widget reads that ColoringSettingsDialog::applySettingsTo performs, so
			// ColorProcessorFactory::create(method, overrides) reproduces the legacy
			// behavior byte-for-byte. Every group is reported as "present", matching
			// the legacy applySettingsTo which always wrote its color-button/slider
			// values onto the processor (the table-based groups are empty-when-empty).
			ColoringOverrides overrides;

			// COLORING_ELEMENT: per-index element colors (getColors layout).
			overrides.element_colors = getColors(COLORING_ELEMENT);

			// COLORING_RESIDUE_NAME: residue name -> color, from residue_table_.
			for (Position p = 0; p < (Position)residue_table_->rowCount(); p++)
			{
				overrides.residue_name_colors[ascii(residue_table_->item(p, 0)->text())] =
					residue_table_->item(p, 1)->background().color();
			}

			// COLORING_RESIDUE_INDEX: first/middle/last residue colors.
			overrides.has_residue_number_colors = true;
			overrides.residue_number_first_color  = first_residue_button->getColor();
			overrides.residue_number_middle_color = middle_residue_button->getColor();
			overrides.residue_number_last_color   = last_residue_button->getColor();

			// COLORING_ATOM_CHARGE: negative/neutral/positive charge colors.
			overrides.has_atom_charge_colors = true;
			overrides.atom_charge_negative_color = negative_charge_button->getColor();
			overrides.atom_charge_neutral_color  = neutral_charge_button->getColor();
			overrides.atom_charge_positive_color = positive_charge_button->getColor();

			// COLORING_DISTANCE: null/max colors + distance slider + show-selected.
			overrides.has_atom_distance_settings = true;
			overrides.atom_distance_null_color = null_distance_button->getColor();
			overrides.atom_distance_max_color  = max_distance_button->getColor();
			overrides.atom_distance_distance   = ((float)max_distance_slider->value()) / 10.0;
			overrides.atom_distance_show_selected = distance_show_selected->isChecked();

			// COLORING_OCCUPANCY: minimum/maximum occupancy colors.
			overrides.has_occupancy_colors = true;
			overrides.occupancy_minimum_color = minimum_o_button->getColor();
			overrides.occupancy_maximum_color = maximum_o_button->getColor();

			// COLORING_SECONDARY_STRUCTURE: helix/coil/strand/turn colors.
			overrides.has_secondary_structure_colors = true;
			overrides.secondary_structure_helix_color  = helix_color_button->getColor();
			overrides.secondary_structure_coil_color   = coil_color_button->getColor();
			overrides.secondary_structure_strand_color = strand_color_button->getColor();
			overrides.secondary_structure_turn_color   = turn_color_button->getColor();

			// COLORING_TEMPERATURE_FACTOR: unassigned/min/max colors + max slider.
			overrides.has_temperature_factor_settings = true;
			overrides.temperature_factor_unassigned_color = unassigned_tf_button->getColor();
			overrides.temperature_factor_minimum_color    = minimum_tf_button->getColor();
			overrides.temperature_factor_maximum_color    = maximum_tf_button->getColor();
			overrides.temperature_factor_max_value        = ((float)max_tf_slider->value()) / 10.0;

			// COLORING_FORCES: min/max colors + min/max value sliders.
			overrides.has_force_settings = true;
			overrides.force_min_color = force_min_color_button->getColor();
			overrides.force_max_color = force_max_color_button->getColor();
			overrides.force_max_value = ((float)force_max_value_slider->value()) / 10.0;
			overrides.force_min_value = ((float)force_min_value_slider->value()) / 10.0;

			// COLORING_RESIDUE_TYPE: residue-type category colors.
			overrides.has_residue_type_colors = true;
			overrides.residue_type_basic_color       = basic_color_button->getColor();
			overrides.residue_type_acidic_color      = acidic_color_button->getColor();
			overrides.residue_type_aromatic_color    = aromatic_color_button->getColor();
			overrides.residue_type_polar_color       = polar_color_button->getColor();
			overrides.residue_type_hydrophobic_color = hydrophobic_color_button->getColor();
			overrides.residue_type_other_color       = other_color_button->getColor();

			// COLORING_CHAIN / COLORING_MOLECULE: per-chain / per-molecule colors.
			overrides.chain_colors    = getColors(COLORING_CHAIN);
			overrides.molecule_colors = getColors(COLORING_MOLECULE);

			return overrides;
		}

		ColorProcessor* ColoringSettingsDialog::createColorProcessor(ColoringMethod method) const
		{
			// Behavior-preserving delegation: pack the dialog's current widget state
			// into a headless ColoringOverrides and hand construction off to the
			// relocated ColorProcessorFactory. The color-processor construction logic
			// now lives in the MODELS layer; the dialog stays fully operational.
			return ColorProcessorFactory::create(method, buildColoringOverrides_());
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
