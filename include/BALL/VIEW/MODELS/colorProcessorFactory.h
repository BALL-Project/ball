// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_MODELS_COLORPROCESSORFACTORY_H
#define BALL_VIEW_MODELS_COLORPROCESSORFACTORY_H

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
#	include <BALL/VIEW/MODELS/colorProcessor.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
#	include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <vector>

namespace BALL
{
	namespace VIEW
	{
		/** Plain-data override set for ColorProcessorFactory::create.

				This struct carries the user-customizable surface that the legacy
				ColoringSettingsDialog used to read from its color tables, color
				buttons and value sliders when constructing a ColorProcessor
				(ColoringSettingsDialog::applySettingsTo). Relocating these values
				into an explicit value object lets the color-processor construction
				logic live in a free-standing factory that has no dependency on any
				Qt widget, color table or value slider.

				KEY INVARIANT (CONFIG-01, compiled-defaults-show-through): each
				ColorProcessor's constructor already loads its compiled default color
				map. The overrides carried by this struct therefore only LAYER the
				user's customizations ON TOP of those compiled defaults. Every member
				defaults to a "no override" sentinel (an empty container, or a
				per-group @c has_* presence flag left @c false). A default-constructed
				ColoringOverrides consequently means "apply nothing — keep the
				processor's compiled defaults", which is exactly the molecule-load
				default-representation render behavior.
				\see ColorProcessorFactory
				\ingroup ViewModels
		*/
		struct BALL_VIEW_EXPORT ColoringOverrides
		{
			// --- COLORING_ELEMENT (ElementColorProcessor) ----------------------
			/** Per-index element color overrides, keyed by table row position
					(matching the legacy getColors(COLORING_ELEMENT) layout). Empty
					means: keep the ElementColorProcessor compiled defaults. */
			std::vector<ColorRGBA> element_colors;

			// --- COLORING_RESIDUE_NAME (ResidueNameColorProcessor) -------------
			/** Per-residue-name color overrides (residue name -> color). Empty means:
					keep the ResidueNameColorProcessor compiled defaults. */
			StringHashMap<ColorRGBA> residue_name_colors;

			// --- COLORING_RESIDUE_INDEX (ResidueNumberColorProcessor) ----------
			/** Whether the first/middle/last residue-number colors below override. */
			bool has_residue_number_colors = false;
			ColorRGBA residue_number_first_color;
			ColorRGBA residue_number_middle_color;
			ColorRGBA residue_number_last_color;

			// --- COLORING_ATOM_CHARGE (AtomChargeColorProcessor) ---------------
			/** Whether the negative/neutral/positive charge colors below override. */
			bool has_atom_charge_colors = false;
			ColorRGBA atom_charge_negative_color;
			ColorRGBA atom_charge_neutral_color;
			ColorRGBA atom_charge_positive_color;

			// --- COLORING_DISTANCE (AtomDistanceColorProcessor) ----------------
			/** Whether the distance null/max colors + distance + show-selected override. */
			bool has_atom_distance_settings = false;
			ColorRGBA atom_distance_null_color;
			ColorRGBA atom_distance_max_color;
			float atom_distance_distance = 0.0f;
			bool atom_distance_show_selected = false;

			// --- COLORING_OCCUPANCY (OccupancyColorProcessor) ------------------
			/** Whether the minimum/maximum occupancy colors below override. */
			bool has_occupancy_colors = false;
			ColorRGBA occupancy_minimum_color;
			ColorRGBA occupancy_maximum_color;

			// --- COLORING_SECONDARY_STRUCTURE (SecondaryStructureColorProcessor)
			/** Whether the helix/coil/strand/turn colors below override. */
			bool has_secondary_structure_colors = false;
			ColorRGBA secondary_structure_helix_color;
			ColorRGBA secondary_structure_coil_color;
			ColorRGBA secondary_structure_strand_color;
			ColorRGBA secondary_structure_turn_color;

			// --- COLORING_TEMPERATURE_FACTOR (TemperatureFactorColorProcessor) -
			/** Whether the unassigned/min/max temperature-factor colors + max value override. */
			bool has_temperature_factor_settings = false;
			ColorRGBA temperature_factor_unassigned_color;
			ColorRGBA temperature_factor_minimum_color;
			ColorRGBA temperature_factor_maximum_color;
			float temperature_factor_max_value = 0.0f;

			// --- COLORING_FORCES (ForceColorProcessor) -------------------------
			/** Whether the min/max force colors + min/max values below override. */
			bool has_force_settings = false;
			ColorRGBA force_min_color;
			ColorRGBA force_max_color;
			float force_min_value = 0.0f;
			float force_max_value = 0.0f;

			// --- COLORING_RESIDUE_TYPE (ResidueTypeColorProcessor) -------------
			/** Whether the residue-type category colors below override. */
			bool has_residue_type_colors = false;
			ColorRGBA residue_type_basic_color;
			ColorRGBA residue_type_acidic_color;
			ColorRGBA residue_type_aromatic_color;
			ColorRGBA residue_type_polar_color;
			ColorRGBA residue_type_hydrophobic_color;
			ColorRGBA residue_type_other_color;

			// --- COLORING_CHAIN (ChainColorProcessor) --------------------------
			/** Per-chain color overrides. Empty means: keep the ChainColorProcessor
					compiled defaults. */
			std::vector<ColorRGBA> chain_colors;

			// --- COLORING_MOLECULE (MoleculeColorProcessor) --------------------
			/** Per-molecule color overrides. Empty means: keep the
					MoleculeColorProcessor compiled defaults. */
			std::vector<ColorRGBA> molecule_colors;

			// COLORING_CUSTOM (CustomColorProcessor) carries no overrides — the
			// legacy applySettingsTo early-returns for it (it is purely
			// constructor-default-driven).
		};

		/** Headless factory for ColorProcessor instances.

				This factory relocates the color-processor construction logic that
				used to live in ColoringSettingsDialog::createColorProcessor /
				ColoringSettingsDialog::applySettingsTo. It is a pure function of an
				explicit ColoringOverrides value object and therefore has no
				dependency on any Qt widget, color table, color button or slider — it is
				headless-constructible and lives on the production render path for both
				the legacy dialog and the Inspector cells.

				A default-constructed ColoringOverrides produces a processor identical
				to today's compiled-default render output (CONFIG-01).
				\see ColoringOverrides
				\ingroup ViewModels
		*/
		class BALL_VIEW_EXPORT ColorProcessorFactory
		{
			public:

			/** Create a ColorProcessor for the given coloring method.
					Constructs the concrete ColorProcessor matching @p method (which
					already loads its compiled default color map) and then layers the
					user overrides carried by @p overrides on top, reproducing the exact
					behavior of the legacy ColoringSettingsDialog::createColorProcessor.
					An empty (default-constructed) @p overrides leaves the processor at
					its compiled defaults.
					@param method    the coloring method to construct a processor for
					@param overrides the user overrides to layer onto the processor
					@return a newly allocated ColorProcessor (caller takes ownership)
					@throw Exception::InvalidOption for an unknown @p method
			*/
			static ColorProcessor* create(ColoringMethod method, const ColoringOverrides& overrides = ColoringOverrides());
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_COLORPROCESSORFACTORY_H
