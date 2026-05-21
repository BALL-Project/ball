// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_MODELS_MODELPROCESSORFACTORY_H
#define BALL_VIEW_MODELS_MODELPROCESSORFACTORY_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#	include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
#	include <BALL/VIEW/KERNEL/common.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Plain-data parameter set for ModelProcessorFactory::create.
				This struct carries every numeric/boolean knob that the legacy
				ModelSettingsDialog used to read from its sliders and checkboxes when
				constructing a ModelProcessor. Relocating these values into an explicit
				value object lets the model-processor construction logic live in a
				free-standing factory that has no dependency on any QWidget.

				Every field is initialized to the legacy dialog's out-of-box default,
				which the dialog itself derived from the default-constructed model
				processor classes (ModelSettingsDialog::setDefaultValues_). A
				default-constructed ModelProcessorParams therefore reproduces the
				dialog's initial state.
				\ingroup ViewModels
		*/
		struct BALL_VIEW_EXPORT ModelProcessorParams
		{
			/// Stick radius used for the pure stick model (AddBallAndStickModel::stick_radius_ default).
			float stick_stick_radius = 0.2f;

			/// Stick radius used for the ball-and-stick model (AddBallAndStickModel::stick_radius_ default).
			float ball_and_stick_stick_radius = 0.2f;

			/// Ball radius used for the ball-and-stick model (AddBallAndStickModel::ball_radius_ default).
			float ball_radius = 0.4f;

			/// Whether dashed bonds are enabled for the ball-and-stick model (AddBallAndStickModel::dashed_bonds_ default).
			bool ball_and_stick_dashed_bonds_enabled = false;

			/// Probe radius for the surface model (AddSurfaceModel::probe_radius_ default).
			float surface_probe_radius = 1.5f;

			/// Van-der-Waals radius scaling factor (AddVanDerWaalsModel::radius_factor_ default).
			float vdw_radius_factor = 1.0f;

			/// Tube radius for the backbone model (AddBackboneModel::tube_radius_ default).
			float tube_radius = 0.4f;

			/// Tube radius for the cartoon model (AddCartoonModel::tube_radius_ default).
			float cartoon_tube_radius = 0.2f;

			/// Helix radius for the cartoon model (AddCartoonModel::helix_radius_ default).
			float cartoon_helix_radius = 2.1f;

			/// Arrow width for the cartoon model (AddCartoonModel::arrow_width_ default).
			float cartoon_arrow_width = 1.0f;

			/// Strand height for the cartoon model (AddCartoonModel::strand_height_ default).
			float cartoon_strand_height = 0.4f;

			/// Strand width for the cartoon model (AddCartoonModel::strand_width_ default).
			float cartoon_strand_width = 2.2f;

			/// Whether the cartoon model renders DNA as a ladder (AddCartoonModel::draw_DNA_as_ladder_ default).
			bool cartoon_dna_ladder_enabled = false;

			/// DNA ladder radius for the cartoon model (AddCartoonModel::DNA_ladder_radius_ default).
			float dna_ladder_radius = 0.8f;

			/// DNA base radius for the cartoon model (AddCartoonModel::DNA_base_radius_ default).
			float dna_base_radius = 0.2f;

			/// DNA helix radius for the cartoon model (AddCartoonModel::DNA_helix_radius_ default).
			float dna_helix_radius = 0.5f;

			/// Whether ribbons are enabled for the cartoon model (AddCartoonModel::draw_ribbon_ default).
			bool ribbons_enabled = true;

			/// Radius for the H-bond model (HBondModelProcessor::radius_ default).
			float hbonds_radius = 0.3f;

			/// Maximum force-vector length for the force model (ForceModel::max_length_ default).
			float force_max_length = 10.0f;

			/// Force-vector scaling for the force model (ForceModel::scaling_ default).
			float force_scaling = 1.1f;

			/// Force-vector offset for the force model (ForceModel::offset_ default).
			float force_offset = 0.2f;

			/// Force-vector base size for the force model (ForceModel::base_size_ default).
			float force_base = 0.2f;
		};

		/** Headless factory for ModelProcessor instances.
				This factory relocates the model-processor construction logic that used
				to live in ModelSettingsDialog::createModelProcessor /
				ModelSettingsDialog::applySettingsTo. It is a pure function of an
				explicit ModelProcessorParams value object and therefore has no
				dependency on any QWidget, slider, or checkbox — it is
				headless-constructible and lives on the production render path for both
				the legacy dialog and the Inspector cells.
				\see ModelProcessorParams
				\ingroup ViewModels
		*/
		class BALL_VIEW_EXPORT ModelProcessorFactory
		{
			public:

			/** Create a ModelProcessor for the given model type.
					Constructs the concrete ModelProcessor matching @p type and applies
					the per-type settings carried by @p params, reproducing the exact
					behavior of the legacy ModelSettingsDialog::createModelProcessor.
					@param type   the model type to construct a processor for
					@param params the settings to bake into the constructed processor
					@return a newly allocated ModelProcessor (caller takes ownership), or
					        NULL for an unknown @p type
			*/
			static ModelProcessor* create(ModelType type, const ModelProcessorParams& params);
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_MODELPROCESSORFACTORY_H
