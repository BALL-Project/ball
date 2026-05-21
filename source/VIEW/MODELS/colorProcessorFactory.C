// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/MODELS/colorProcessorFactory.h>

#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <BALL/COMMON/exception.h>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Per-method override application, relocated verbatim from
			// ColoringSettingsDialog::applySettingsTo (coloringSettingsDialog.C:493-610),
			// with every dialog table/color-button/slider read replaced by the matching
			// ColoringOverrides field. Behavior is preserved exactly: each branch only
			// layers an override when the corresponding ColoringOverrides member is
			// non-empty / its has_* presence flag is set, otherwise the processor's
			// compiled default color map (loaded by its constructor) is left untouched.
			void applyOverridesTo_(ColorProcessor& cp, const ColoringOverrides& overrides)
			{
				if (RTTI::isKindOf<CustomColorProcessor>(&cp)) return;

				if (RTTI::isKindOf<ElementColorProcessor>(&cp))
				{
					const std::vector<ColorRGBA>& colors = overrides.element_colors;
					for (Position p = 0; p < colors.size(); p++)
					{
						(*(ElementColorProcessor*)&cp).getColorMap()[p] = colors[p];
					}
					return;
				}

				if (RTTI::isKindOf<ResidueNameColorProcessor>(&cp))
				{
					ResidueNameColorProcessor& dp = (*(ResidueNameColorProcessor*)&cp);
					for (StringHashMap<ColorRGBA>::ConstIterator it = overrides.residue_name_colors.begin();
					     it != overrides.residue_name_colors.end(); ++it)
					{
						dp.getColorMap()[it->first] = it->second;
					}
					return;
				}

				if (RTTI::isKindOf<ResidueNumberColorProcessor>(&cp))
				{
					if (!overrides.has_residue_number_colors) return;
					ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
					dp.setFirstColor(overrides.residue_number_first_color);
					dp.setMiddleColor(overrides.residue_number_middle_color);
					dp.setLastColor(overrides.residue_number_last_color);
					return;
				}

				if (RTTI::isKindOf<AtomChargeColorProcessor>(&cp))
				{
					if (!overrides.has_atom_charge_colors) return;
					AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
					dp.getColors()[0] = overrides.atom_charge_negative_color;
					dp.getColors()[1] = overrides.atom_charge_neutral_color;
					dp.getColors()[2] = overrides.atom_charge_positive_color;
					return;
				}

				if (RTTI::isKindOf<AtomDistanceColorProcessor>(&cp))
				{
					if (!overrides.has_atom_distance_settings) return;
					AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
					dp.setNullDistanceColor(overrides.atom_distance_null_color);
					dp.setMaxDistanceColor(overrides.atom_distance_max_color);
					dp.setDistance(overrides.atom_distance_distance);
					dp.setShowSelected(overrides.atom_distance_show_selected);
					return;
				}

				if (RTTI::isKindOf<OccupancyColorProcessor>(&cp))
				{
					if (!overrides.has_occupancy_colors) return;
					OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
					dp.getColors()[0] = overrides.occupancy_minimum_color;
					dp.getColors()[1] = overrides.occupancy_maximum_color;
					return;
				}

				if (RTTI::isKindOf<SecondaryStructureColorProcessor>(&cp))
				{
					if (!overrides.has_secondary_structure_colors) return;
					SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);
					dp.setHelixColor(overrides.secondary_structure_helix_color);
					dp.setCoilColor(overrides.secondary_structure_coil_color);
					dp.setStrandColor(overrides.secondary_structure_strand_color);
					dp.setTurnColor(overrides.secondary_structure_turn_color);
					return;
				}

				if (RTTI::isKindOf<TemperatureFactorColorProcessor>(&cp))
				{
					if (!overrides.has_temperature_factor_settings) return;
					TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
					dp.setMinColor(overrides.temperature_factor_unassigned_color);
					dp.getColors()[0] = overrides.temperature_factor_minimum_color;
					dp.getColors()[1] = overrides.temperature_factor_maximum_color;
					dp.setMaxColor(overrides.temperature_factor_unassigned_color);
					dp.setMaxValue(overrides.temperature_factor_max_value);
					return;
				}

				if (RTTI::isKindOf<ForceColorProcessor>(&cp))
				{
					if (!overrides.has_force_settings) return;
					ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
					dp.getColors()[0] = overrides.force_min_color;
					dp.getColors()[1] = overrides.force_max_color;
					dp.setMaxValue(overrides.force_max_value);
					dp.setMinValue(overrides.force_min_value);
					return;
				}

				if (RTTI::isKindOf<ResidueTypeColorProcessor>(&cp))
				{
					if (!overrides.has_residue_type_colors) return;
					ResidueTypeColorProcessor& dp = (*(ResidueTypeColorProcessor*)&cp);
					dp.setBasicColor(overrides.residue_type_basic_color);
					dp.setAcidicColor(overrides.residue_type_acidic_color);
					dp.setAromaticColor(overrides.residue_type_aromatic_color);
					dp.setPolarColor(overrides.residue_type_polar_color);
					dp.setHydrophobicColor(overrides.residue_type_hydrophobic_color);
					dp.setOtherColor(overrides.residue_type_other_color);
					return;
				}

				if (RTTI::isKindOf<ChainColorProcessor>(&cp))
				{
					if (overrides.chain_colors.empty()) return;
					((ChainColorProcessor*)&cp)->setColors(overrides.chain_colors);
					return;
				}

				if (RTTI::isKindOf<MoleculeColorProcessor>(&cp))
				{
					if (overrides.molecule_colors.empty()) return;
					((MoleculeColorProcessor*)&cp)->setColors(overrides.molecule_colors);
					return;
				}
			}
		} // anonymous namespace

		// Relocated verbatim from ColoringSettingsDialog::createColorProcessor
		// (coloringSettingsDialog.C:669-734). Each constructed processor already
		// loads its compiled default color map; applyOverridesTo_ then layers the
		// explicit ColoringOverrides on top (a default-constructed ColoringOverrides
		// leaves the compiled defaults intact — CONFIG-01).
		ColorProcessor* ColorProcessorFactory::create(ColoringMethod method, const ColoringOverrides& overrides)
		{
			ColorProcessor* color_processor = 0;

			switch (method)
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

			applyOverridesTo_(*color_processor, overrides);

			return color_processor;
		}

	} // namespace VIEW
} // namespace BALL
